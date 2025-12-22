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
	((test_status += $?))
	MAIN=$1 make debug
	((test_status += $?))
	make
	((test_status += $?))
	make clean
	if [ $test_status -ne 0 ]; then
		echo "Failed to make targets"
		make fclean
		exit 1
	fi
}

ft_print_status()
{
	if [ $1 -ne $2 ]; then
		echo -e "${RED}Test $4 #$3 init_env $5: failed with status $1${RESET}"
	else
		echo -e "${GREEN}Test $3 #$2 init_env $4: passed with status $1 ${RESET}"
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

ft_get_status()
{
	local	status=0

	read -r status <$1
	if [ $2 -eq $status ]; then
		echo "OK"
	else
		echo "$2"
	fi
}

ft_get_output()
{
	local	output=0

	read -r output <$1
	if [ $2 -eq $output ]; then
		echo "0"
	else
		echo "1"
	fi
}

ft_test_load_env()
{
	local	test_status=0
	local	DIR="load_env"
	local	arg=0
	local	output
	local	status
	TESTFILE="$TESTS_DIR/load_env_tests.txt"


	ft_mk_log_dir $DIR
	local	DEBUG_LOG="$DEBUG_DIR/load_env.txt"
	local	VAL_LOG="$VAL_DIR/load_env.txt"
	echo -e "${BLUE}---- Running load_env unit tests ----${RESET}"
	ft_make "$1"
	if [ -f "$TESTFILE" ]; then
		local	i = 0;
		while read -r arg || [ -n "$arg" ]; do
			$output=$($arg ./unit-tests 2>&1)
			$status=$($arg ./unit-tests > /dev/null 2>&1 && echo $?)
			echo "$output" > "$NORMAL_DIR/load_env_log.txt"
			ft_print_status "$status" "$(ft_get_status "$TESTFILE")" "$i" "|  normal  |"
			$output=$($arg ./dmsh)
			$status=$($arg ./dmsh >/dev/null && echo $?)
			echo $output > "$DEBUG_LOG" 2>&1
			ft_print_status "$status" "$(ft_get_status "$TESTFILE")" "$i" "|  debug   |"
			$output=$(valgrind -s --track-origins=yes ./valmsh 2>&1)
			$status=$(valgrind -s --track-origins=yes ./valmsh > /dev/null 2>&1)
			echo $output > "$VAL_LOG"
			ft_print_status "$status" "$(ft_get_status "$TESTFILE")" "$i" "| valgrind |"
			make fclean
			MAIN="$1" make clean
			echo
			(($i++));
		done < "$TESTFILE"
	else
		echo "No $TESTFILE found"
	fi
}

ft_test_init_env()
{
	local	test_status=0
	local	DIR="init_env"
	TESTFILE="$TESTS_DIR/init_env_tests.txt"


	ft_mk_log_dir $DIR
	local	DEBUG_LOG="$DEBUG_DIR/init_env.txt"
	local	VAL_LOG="$VAL_DIR/init_env.txt"
	local	expected=0
	echo -e "${BLUE}---- Running init_env unit tests ----${RESET}"
	ft_make "$1"
	./unit-tests > "$NORMAL_DIR/init_env_log.txt" 2>&1
	ft_print_status "$?" $expected "" "|  normal  |"
	./dmsh > "$DEBUG_LOG" 2>&1
	ft_print_status "$?" $expected "" "|  debug   |"
	valgrind -s --track-origins=yes ./valmsh > "$VAL_LOG" 2>&1
	ft_print_status "$?" $expected "" "| valgrind |"
	make fclean
	MAIN="$1" make clean
	echo
}

echo
echo -e "${BLUE}---- Running static analisys ----${RESET}"
make check
echo
echo -e "${BLUE}---- Running norminette ----${RESET}"
norminette ../main.c ../libft/ ../parse/ ../signals/ ../init/ ../readline/ \
../includes/ > $LOG_DIR/norm_log.txt 2>&1
cat $LOG_DIR/norm_log.txt | grep Error
if [ $? -eq 0 ]; then
	echo -e  "${RED}Norminette not passed${RESET}"
else
	echo -e  "${GREEN}Norminette passed${RESET}"
fi
echo
ft_test_init_env "test_init_env.c"
ft_test_load_env "test_load_env.c"
