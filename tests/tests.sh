#!/bin/bash

# Relevant directories
LOG_DIR="$(date +%y%m%d%H%M%S)"
TESTS_DIR="to_be_tested"
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

	MAIN=$1 make val
	test_status=$?
	if [ $test_status -ne 0 ]; then
		echo "Failed to make targets"
		MAIN=$1 make fclean
		exit 1
	fi
	MAIN=$1 make debug
	test_status=$?
	if [ $test_status -ne 0 ]; then
		echo "Failed to make targets"
		MAIN=$1 make fclean
		exit 1
	fi
	MAIN=$1 make
	test_status=$?
	if [ $test_status -ne 0 ]; then
		echo "Failed to make targets"
		MAIN=$1 make fclean
		exit 1
	fi
	make clean
}

ft_check_output()
{
	local	output
	local	expected_output
	local	i=1
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

ft_not_so_long_var()
{
	local	ARG_MAX=$((2048))
	local	i=0
	echo -n > not_so_long_var.txt
	while [ $i -le $ARG_MAX ]; do
		echo "FOO$i=1" >> not_so_long_var.txt
		((i++))
	done
}

ft_many_vars()
{
	i=0
	while [ $i -le 204801 ]; do
		export "FOO$i=1"
		((i++))
	done
}

ft_clean_vars()
{
	i=0
	while [ $i -le 204801 ]; do
		unset "FOO$i=1"
		((i++))
	done
}

ft_test_tokens()
{
	local	DIR="lexing"
	local	output
	local	expected_output
	TESTFILE="$TESTS_DIR/lexing_tests.txt"

	ft_mk_log_dir $DIR
	local	DEBUG_LOG="$DEBUG_DIR/lexing"
	local	VAL_LOG="$VAL_DIR/lexing"
	echo -e "${BLUE}---- Running lexing integration tests ----${RESET}"
	MAIN=$1 make fclean 
	ft_make "$1"
		if [ -f "$TESTFILE" ]; then
		local i=1;

		while read -r expected_output; do

			expected_output=$(eval "{ export $arg && $expected_cmd; } 2>&1")

    		output=$(eval "{ export $arg && ./unit-tests; } 2>&1")
    		status=$?
    		echo "$output" > "$NORMAL_DIR/load_env_log$i.txt"
        	ft_check_output "$output" "$expected_output"
    		((final_status = exec_status + output_status))
    		ft_print_status "$status" "$expected_status" "$i" "|  normal  |"

    		output=$(eval "{ export $arg && ./dmsh; } 2>&1")
    		status=$?
    		echo "$output" > "$DEBUG_LOG$i.txt"
    		ft_check_output "$output" "$expected_output"
    		ft_print_status "$status" "$expected_status" "$i" "|  debug   |"

    		output=$(eval "{ export $arg &&  valgrind -q --leak-check=full --error-exitcode=255  --track-origins=yes -s ./valmsh; } 2>&1")
			status=$?
    		echo "$output" > "$VAL_LOG$i.txt" 
    		ft_check_output "$output" "$expected_output"
    		ft_print_status "$status" "$expected_status" "$i" "| valgrind |"
    
    		echo "------------------------------------------------"
    		((i++))

		done < "$TESTFILE"
	else
		echo "No $TESTFILE found"
	fi
	MAIN="$1" make clean
}

ft_test_load_env()
{
	local	exec_status
	local	DIR="load_env"
	local	arg
	local	arg2
	local	output
	local	status
	local	expected_output
	local	expected_status
	TESTFILE="$TESTS_DIR/load_env_tests.txt"

	ft_mk_log_dir $DIR
	local	DEBUG_LOG="$DEBUG_DIR/load_env"
	local	VAL_LOG="$VAL_DIR/load_env"
	echo -e "${BLUE}---- Running load_env unit tests ----${RESET}"
	MAIN=$1 make fclean 
	ft_make "$1"
	ft_not_so_long_var
	if [ -f "$TESTFILE" ]; then
		local i=1;

		while read -r arg && read -r expected_cmd && read -r expected_status; do

			expected_output=$(eval "{ export $arg && $expected_cmd; } 2>&1")

    		output=$(eval "{ export $arg && ./unit-tests; } 2>&1")
    		status=$?
    		echo "$output" > "$NORMAL_DIR/load_env_log$i.txt"
        	ft_check_output "$output" "$expected_output"
    		((final_status = exec_status + output_status))
    		ft_print_status "$status" "$expected_status" "$i" "|  normal  |"

    		output=$(eval "{ export $arg && ./dmsh; } 2>&1")
    		status=$?
    		echo "$output" > "$DEBUG_LOG$i.txt"
    		ft_check_output "$output" "$expected_output"
    		ft_print_status "$status" "$expected_status" "$i" "|  debug   |"

    		output=$(eval "{ export $arg &&  valgrind -q --leak-check=full --error-exitcode=255  --track-origins=yes -s ./valmsh; } 2>&1")
			status=$?
    		echo "$output" > "$VAL_LOG$i.txt" 
    		ft_check_output "$output" "$expected_output"
    		ft_print_status "$status" "$expected_status" "$i" "| valgrind |"
    
    		echo "------------------------------------------------"
    		((i++))

		done < "$TESTFILE"
	else
		echo "No $TESTFILE found"
	fi
	MAIN="$1" make clean
}

# make -s fclean
echo
echo -e "${BLUE}---- Running static analisys ----${RESET}"
make check
echo
echo -e "${BLUE}---- Running norminette ----${RESET}"
# norminette ../main.c ../libft/ ../parse/ ../signals/ ../variables/ ../readline/ ../exec/\
# ../includes/ > $LOG_DIR/norm_log.txt 2>&1
cat $LOG_DIR/norm_log.txt | grep Error
if [ $? -eq 0 ]; then
	echo -e  "${RED}Norminette not passed${RESET}"
else
	echo -e  "${GREEN}Norminette passed${RESET}"
fi
echo
ft_make ../main.c
# ft_test_load_env "test_load_env.c"
# ft_test_tokens
make clean
