#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Paths relative to the script directory
TEST_FILE="$SCRIPT_DIR/test_integration.txt"
PHILO_EXE="$SCRIPT_DIR/../tester"
TSAN_EXE="$SCRIPT_DIR/../philo_tsan"

LOG_DIR="$SCRIPT_DIR/$(date +%y%m%d%H%M%S)/"

# Colors
GREEN="\e[32m"
RED="\e[31m"
BLUE="\e[34m"
RESET="\e[0m"

mkdir -p "$LOG_DIR"
mkdir -p "$LOG_DIR/val_logs"
mkdir -p "$LOG_DIR/tsan_logs"

# 0. Run Unit Tests
UNIT_TEST_EXE="$SCRIPT_DIR/test_runner_c"
if [ -f "$UNIT_TEST_EXE" ]; then
    echo -e "${BLUE}--- Running Unit Tests ---${RESET}"
    # Run in the script dir so it finds tests_atoi.txt etc.
    (cd "$SCRIPT_DIR" && ./test_runner_c &> "$LOG_DIR/unit_test_console.log")
    if [ $? -ne 0 ]; then
        echo -e "❌ ${RED}Unit Tests FAILED. See $LOG_DIR/unit_test_console.log for details.${RESET}"
        exit 1
    fi
    echo -e "✅ ${GREEN}Unit Tests Passed.${RESET}"
else
    echo -e "${RED}Warning: Unit test executable not found at $UNIT_TEST_EXE. Skipping unit tests.${RESET}\n"
fi

ft_run_test() {
    local id="$1"
    local expectation="$2"
    local args="$3"
    
    # Construct log filename: [ID]_[Expectation]_[ArgsHash].txt
    # Create a simple hash/summary of args (replace spaces with underscores)
    local args_slug=$(echo "$args" | tr ' ' '_')
    local log_name=$(printf "%03d_%s_%s.txt" "$id" "$expectation" "$args_slug")
    local log_path="$LOG_DIR$log_name"

    # Determine execution parameters based on expectation
    local timeout_duration=5
    local expected_code=0

    case "$expectation" in
        SURVIVE)
            timeout_duration=5 # Run for 5s, verify it's still alive
            expected_code=124  # Timeout exit code
            ;;
        DIE)
            timeout_duration=5 # Should die quickly
            expected_code=0    # Normal exit after death
            ;;
        FINITE)
            timeout_duration=60 # Allow time to finish eating
            expected_code=0
            ;;
        ERROR)
            timeout_duration=2
            expected_code=1    # Error exit
            ;;
    esac

    echo -e "\n${BLUE}Test $id: Expectation=$expectation Args=$args${RESET}"
    echo "Running: timeout $timeout_duration $PHILO_EXE $args" > "$log_path"

    # 1. Normal Run
    timeout "$timeout_duration" $PHILO_EXE $args >> "$log_path" 2>&1
    local exit_code=$?

    # Verify Result
    if [ $exit_code -eq $expected_code ]; then
        echo -e "✅ ${GREEN}PASS${RESET} (Exit Code: $exit_code)"
    else
        # Special case: DIE might timeout if logic is wrong (Immortality bug)
        if [ "$expectation" == "DIE" ] && [ $exit_code -eq 124 ]; then
             echo -e "❌ ${RED}FAIL${RESET}: Expected DEATH, but Timed Out (Immortality)."
        # Special case: SURVIVE might die (exit 0)
        elif [ "$expectation" == "SURVIVE" ] && [ $exit_code -ne 124 ]; then
             echo -e "❌ ${RED}FAIL${RESET}: Expected SURVIVAL, but exited with $exit_code."
        else
             echo -e "❌ ${RED}FAIL${RESET}: Expected $expected_code, got $exit_code."
        fi
    fi

    # 2. Sanitizers (Only if not an ERROR test)
    if [ "$expectation" != "ERROR" ]; then
        # Valgrind
        if [ "$expectation" == "FINITE" ]; then
             # Only run valgrind on finite tests to avoid infinite loops/timeouts affecting leak check
             echo "   Running Valgrind..."
             timeout 60 valgrind --error-exitcode=137 --leak-check=full $PHILO_EXE $args >> "$LOG_DIR/val_logs/$log_name" 2>&1
             if [ $? -eq 137 ]; then echo -e "   ❌ ${RED}[Valgrind] Memory Errors${RESET}"; else echo -e "   ✅ ${GREEN}[Valgrind] Clean${RESET}"; fi
        fi

        # Helgrind
        echo "   Running Helgrind..."
        # Use shorter timeout for helgrind as it is slow
        timeout 10 valgrind --tool=helgrind --error-exitcode=138 $PHILO_EXE $args >> "$LOG_DIR/hel_logs/$log_name" 2>&1
        local h_code=$?
        if [ $h_code -eq 138 ]; then 
            echo -e "   ❌ ${RED}[Helgrind] Concurrency Errors${RESET}"
        else 
            echo -e "   ✅ ${GREEN}[Helgrind] Clean${RESET}"
        fi

        # TSan
        echo "   Running TSan..."
        TSAN_OPTIONS="exitcode=66" timeout 5 $TSAN_EXE $args >> "$LOG_DIR/tsan_logs/$log_name" 2>&1
        local t_code=$?
        if [ $t_code -eq 66 ]; then 
             echo -e "   ❌ ${RED}[TSan] Data Race Detected${RESET}"
        else 
             echo -e "   ✅ ${GREEN}[TSan] Clean${RESET}"
        fi
    fi
}

if [ ! -f "$TEST_FILE" ]; then
    echo "Error: $TEST_FILE not found."
    exit 1
fi

# Main Loop
i=1
while IFS='|' read -r expectation args || [ -n "$expectation" ]; do
    # Skip empty lines
    if [ -z "$expectation" ]; then continue; fi
    
    ft_run_test "$i" "$expectation" "$args"
    ((i++))
done < "$TEST_FILE"
