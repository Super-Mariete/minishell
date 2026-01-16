# !/bin/bash --posix
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
        local   test_status=0

        make val
        test_status=$?
        if [ $test_status -ne 0 ]; then
                echo "Failed to make targets"
                LEXING=$2 MAIN=$1 make fclean
                exit 1
        fi
        make debug
        test_status=$?
        if [ $test_status -ne 0 ]; then
                echo "Failed to make targets"
                LEXING=$2 MAIN=$1 make fclean
                exit 1
        fi
        make
        test_status=$?
        if [ $test_status -ne 0 ]; then
                echo "Failed to make targets"
                LEXING=$2 MAIN=$1 make fclean
                exit 1
        fi
        make clean
}

ft_check_output()
{
        local   output
        local   expected_output
        local   i=1
        local ret=0

        while IFS= read -r output && IFS= read -r expected_output <&3; do
        if [ "$output" != "$expected_output" ]; then
            echo "----------- Error en línea $i -----------"
            echo -e "${BLUE}Output real:      '$output'${RESET}"
            echo -e "Output esperado:  '$expected_output'"
                        ((ret++))
        fi
        ((i++))
    done <<< "$1" 3<<< "$2"
        return $ret
}

ft_check_line_output()
{
    if [ "$1" != "$2" ]; then
                echo "----------- Error en línea $i -----------"
                echo -e "${BLUE}Output real:      '$1'${RESET}"
                echo -e "Output esperado:  '$2'"
                return 1
        fi
        return 0
}

ft_print_status()
{
        if [ $1 -ne $2 ]; then
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
        mkdir -p $NORMAL_DIR
        mkdir -p $VAL_DIR
        mkdir -p $DEBUG_DIR
}

ft_test_lexing()
{
        local   DIR="lexing"
        local   output
        local   expected_output
        local   status
        local   expected_output
        local   expected_status=0
        local   arg
        TESTFILE="test_lexing.txt"

        ft_mk_log_dir $DIR
        local   DEBUG_LOG="$DEBUG_DIR/lexing"
        local   VAL_LOG="$VAL_DIR/lexing"
        echo -e "${BLUE}---- Running lexing integration tests ----${RESET}"
        cd "test_lexing/"
        make fclean
        ft_make "$1" "$2"
        if [ -f "$TESTFILE" ]; then
                local i=1;

                while read -r arg && read -r expected_output; do
                output=$(printf "%s\n" "$arg" | ./unit-tests 2>&1)
                echo "$output" > "$NORMAL_DIR/lexing_log$i.txt"
                ft_check_line_output "$output" "$expected_output"
                status=$?
                ((final_status = exec_status + output_status))
                ft_print_status "$status" "$expected_status" "$i" "|  normal  |"

                output=$(printf "%s\n" "$arg" | ./dmsh 2>&1)
                echo "$output" > "$DEBUG_LOG$i.san.txt"
                ft_check_line_output "$output" "$expected_output"
                status=$?
                ft_print_status "$status" "$expected_status" "$i" "|  debug   |"

                        # Valgrind: Log to file to separate tool output from program output (stderr)
                        output=$(printf "%s\n" "$arg" | valgrind --log-file="$VAL_LOG$i.valgrind.txt" --leak-check=full --error-exitcode=255 --track-origins=yes -s ./valmsh 2>&1)
                        cat "$VAL_LOG$i.valgrind.txt" >> "$VAL_LOG$i.txt"
                        rm "$VAL_LOG$i.valgrind.txt"
                        ft_check_line_output "$output" "$expected_output"
                        status=$?
                        ft_print_status "$status" "$expected_status" "$i" "| valgrind |"
                echo "------------------------------------------------"
                ((i++))

                done < "$TESTFILE"
        else
                echo "No $TESTFILE found"
        fi
        make fclean
        cd ..
}

# make -s fclean
echo
echo -e "${BLUE}---- Running static analisys ----${RESET}"
make check
echo
echo -e "${BLUE}---- Running norminette ----${RESET}"
norminette ../main.c ../libft/ ../parser/ ../signals/ ../variables/ ../readline/ ../exec/\
../includes/ > $LOG_DIR/norm_log.txt 2>&1
cat $LOG_DIR/norm_log.txt | grep Error
if [ $? -eq 0 ]; then
        echo -e  "${RED}Norminette not passed${RESET}"
else
        echo -e  "${GREEN}Norminette passed${RESET}"
fi
echo
# ft_make ../main.c
ft_test_tokens "test_lexing.c" "lexing.c"