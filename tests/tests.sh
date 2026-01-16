#!/bin/bash

# Relevant directories
LOG_DIR="$(pwd)/$(date +%y%m%d%H%M%S)"
NORMAL_DIR=0
VAL_DIR=0
DEBUG_DIR=0

# Colors
GREEN="\e[32m"
RED="\e[31m"
BLUE="\e[34m"
RESET="\e[0m"

mkdir -p "$LOG_DIR"

# Make the executables
ft_make()
{
        local	test_status=0

        make -s re
        test_status=$?
        if [ $test_status -ne 0 ]; then
                echo "Failed to make targets"
                make -s fclean
                exit 1
        fi
        make -s clean
}

ft_check_output()
{
        local	output
        local	expected_output
        local	i=1
        local	ret=0

        while IFS= read -r output && IFS= read -r expected_output <&3; do
                if [ "$output" != "$expected_output" ]; then
                        echo "----------- Error en línea $i ----------- "
                        echo -e "${BLUE}Output real:		'$output'${RESET}"
                        echo -e "Output esperado:	'$expected_output'"
                        ((ret++))
                fi
                ((i++))
        done <<< "$1" 3<<< "$2"
        return $ret
}

ft_check_line_output()
{
        if [ "$1" != "$2" ]; then
                echo "----------- Error en línea $i ----------- "
                echo -e "${BLUE}Output real:		'$1'${RESET}"
                echo -e "Output esperado:	'$2'"
                return 1
        fi
        return 0
}

ft_print_status()
{
        # shellcheck disable=SC2086
        if [ "$1" -ne $2 ]; then
                echo -e "${RED}Test $3 $4: failed with status $1 (expected $2)${RESET}"
        else
                echo -e "${GREEN}Test $3 $4: passed with status $1${RESET}"
        fi
}

ft_mk_log_dir()
{
        NORMAL_DIR="$LOG_DIR/$1/"
        VAL_DIR="$LOG_DIR/$1/val_log"
        DEBUG_DIR="$LOG_DIR/$1/debug_log"
        mkdir -p "$LOG_DIR/$1"
        mkdir -p "$NORMAL_DIR"
        # shellcheck disable=SC2086
        mkdir -p $VAL_DIR
        # shellcheck disable=SC2086
        mkdir -p $DEBUG_DIR
}

ft_test_lexing()
{
        local	DIR="lexing"
        local	output
        local	formatted_output
        local	expected_output
        local	status
        local	expected_status=0
        local	arg
        TESTFILE="test_lexing.txt"

        ft_mk_log_dir $DIR
        local	DEBUG_LOG="$DEBUG_DIR/lexing"
        local	VAL_LOG="$VAL_DIR/lexing"
        echo -e "${BLUE}---- Running lexing integration tests ----${RESET}"
        cd "test_lexing/" || (echo -e "${RED}Can't cd to test_lexing\n${RESET}" && exit)
        make -s fclean
        ft_make
        if [ -f "$TESTFILE" ]; then
                local	i=1;

                while read -r arg && read -r expected_output; do
                        # --- Normal Execution ---
                        output=$(printf "%s\n" "$arg" | ./msh 2>&1)
                        echo "$output" > "$NORMAL_DIR/lexing_log$i.txt"
                        
                        # Normalize output: Remove 'exit', replace newlines with spaces, trim leading/trailing spaces, squeeze spaces
                        formatted_output=$(echo "$output" | sed 's/exit//g' | tr '\n' ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//;s/[[:space:]]\+/ /g')
                        
                        ft_check_line_output "$formatted_output" "$expected_output"
                        status=$?
                        ft_print_status "$status" "$expected_status" "$i" "|  normal  |"
                        
                        # --- Sanitizer Execution ---
                        # Use ASAN_OPTIONS to redirect sanitizer output to a file and ensure full traces
                        local asan_log_prefix="$DEBUG_DIR/asan_log_$i"
                        # Clean up previous logs if any
                        rm -f "${asan_log_prefix}".*
                        
                        export ASAN_OPTIONS="symbolize=1:fast_unwind_on_malloc=0:log_path=$asan_log_prefix"
                        output=$(printf "%s\n" "$arg" | ./san_msh 2>&1)
                        unset ASAN_OPTIONS
                        
                        # Check if ASan generated a log file (implies error/leak)
                        local asan_log_file
                        asan_log_file=$(find "$DEBUG_DIR" -name "asan_log_$i.*" -print -quit)
                        if [ -n "$asan_log_file" ]; then
                                cat "$asan_log_file" > "$DEBUG_LOG$i.san.txt"
                                echo -e "${RED}ASan Error Detected (See logs)${RESET}"
                        else
                                echo "No ASan errors" > "$DEBUG_LOG$i.san.txt"
                        fi
                        echo "$output" >> "$DEBUG_LOG$i.san.txt"
                        
                        formatted_output=$(echo "$output" | sed 's/exit//g' | tr '\n' ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//;s/[[:space:]]\+/ /g')
                        
                        ft_check_line_output "$formatted_output" "$expected_output"
                        status=$?
                        ft_print_status "$status" "$expected_status" "$i" "|  debug   |"
                        
                        # --- Valgrind Execution ---
                        # Valgrind logs to file; stdout/stderr captured in output
                        output=$(printf "%s\n" "$arg" | valgrind --log-file="$VAL_LOG$i.valgrind.txt" --leak-check=full --error-exitcode=255 --track-origins=yes -s --show-leak-kinds=all --suppressions=../../readline.supp ./val_msh 2>&1)
                        val_status=$?
                        if [ $val_status -eq 255 ]; then
                                echo -e "${RED}Valgrind Error/Leak Detected (See logs)${RESET}"
                        fi
                        cat "$VAL_LOG$i.valgrind.txt" >> "$VAL_LOG$i.txt"
                        rm "$VAL_LOG$i.valgrind.txt"
                        
                        formatted_output=$(echo "$output" | sed 's/exit//g' | tr '\n' ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//;s/[[:space:]]\+/ /g')
                        ft_check_line_output "$formatted_output" "$expected_output"
                        status=$?
                        ft_print_status "$status" "$expected_status" "$i" "| valgrind |"
                        
                        echo "------------------------------------------------"
                        ((i++))
                done < "$TESTFILE"
        else
                echo "No $TESTFILE found"
        fi
        make -s fclean
        cd ..
}

# make -s fclean
echo
echo -e "${BLUE}---- Running static analysis ----${RESET}"
cppcheck --enable=warning,style,performance,portability \
         --language=c \
         --std=c11 \
         --template='{file}:{line},{severity},{id},{message}' \
         --suppress=missingIncludeSystem \
         --error-exitcode=1 \
         --check-level=exhaustive \
         ../minishell.c ../libft/ ../parsing/ ../exec/ | grep ","
echo
echo -e "${BLUE}---- Running norminette ----${RESET}"
norminette ../minishell.c ../libft/ ../parsing/ ../exec/ > "$LOG_DIR"/norm_log.txt 2>&1
cat "$LOG_DIR"/norm_log.txt | grep Error
if [ $? -eq 0 ]; then
        echo -e  "${RED}Norminette not passed${RESET}"
else
        echo -e  "${GREEN}Norminette passed${RESET}"
fi
echo
# ft_make ../main.c
ft_test_lexing
