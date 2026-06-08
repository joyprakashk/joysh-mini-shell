#!/bin/bash
# Test suite for joysh

SHELL_BIN="./bin/joysh"
TEST_DIR="tests/test_files"
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Setup
mkdir -p $TEST_DIR
cd $TEST_DIR
echo "test content" > input.txt
echo "line1" > file1.txt
echo "line2" >> file1.txt

passed=0
failed=0

test_command() {
    echo -n "Testing: $1 ... "
    echo "$1" | $SHELL_BIN > output.txt 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}"
        ((passed++))
    else
        echo -e "${RED}FAIL${NC}"
        ((failed++))
    fi
}

echo "=== joysh Test Suite ==="
echo

# Basic commands
test_command "pwd"
test_command "echo hello world"
test_command "ls"

# Built-ins
test_command "cd /tmp"
test_command "clear"
test_command "history"
test_command "stats"

# Redirection
test_command "echo test > out.txt"
test_command "cat < input.txt"
test_command "echo append >> out.txt"

# Pipes
test_command "ls | grep txt"
test_command "cat file1.txt | grep line1"
test_command "echo hello | cat | cat"

# Background jobs
test_command "sleep 1 &"

# Error handling
test_command "nonexistent_command"

echo
echo "==========================="
echo -e "Tests passed: ${GREEN}$passed${NC}"
echo -e "Tests failed: ${RED}$failed${NC}"
echo "==========================="

# Cleanup
cd ../..
rm -rf $TEST_DIR

exit $failed
