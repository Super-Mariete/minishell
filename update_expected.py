import os
import shutil
import glob

TESTS_DIR = "tests"
TEST_PARSING_DIR = "tests/test_parsing"
OUTPUT_DIR = os.path.join(TEST_PARSING_DIR, "output")

def get_latest_log_dir():
    # List directories in tests/
    dirs = [d for d in os.listdir(TESTS_DIR) if os.path.isdir(os.path.join(TESTS_DIR, d))]
    # Filter for numeric (timestamp)
    log_dirs = [d for d in dirs if d.isdigit()]
    if not log_dirs:
        return None
    # Sort and pick last
    log_dirs.sort()
    return os.path.join(TESTS_DIR, log_dirs[-1])

def main():
    log_dir = get_latest_log_dir()
    if not log_dir:
        print("No log directory found")
        return

    parsing_log_dir = os.path.join(log_dir, "parsing")
    if not os.path.exists(parsing_log_dir):
        print(f"No parsing log dir at {parsing_log_dir}")
        return

    print(f"Using logs from {parsing_log_dir}")

    # Iterate case_001 ...
    # We don't know exact count, scan input dir?
    input_files = sorted(glob.glob(os.path.join(TEST_PARSING_DIR, "input", "case_*")))
    
    for i, input_file in enumerate(input_files):
        case_num = i + 1
        log_file = os.path.join(parsing_log_dir, f"parsing_log{case_num}.txt")
        output_file = os.path.join(OUTPUT_DIR, os.path.basename(input_file))
        
        if os.path.exists(log_file):
            # Read log file
            with open(log_file, 'r') as f:
                content = f.read()
            
            # Remove "exit" from content to match tests.sh normalization behavior
            content = content.replace("exit\n", "").replace("exit", "")

            # Write to output file
            with open(output_file, 'w') as f:
                f.write(content)
            
            print(f"Updated {output_file}")
        else:
            print(f"Log file not found for case {case_num}: {log_file}")

if __name__ == "__main__":
    main()
