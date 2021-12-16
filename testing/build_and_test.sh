#!/usr/local/bin/zsh

cd ..

make clean
make

NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[1;34m'

echo "\n\n--C compiler\n"
echo "Running tests:\n"

function run_test {

    echo "${BLUE}TEST:${NC} ${1}"

    # get code to pass into compiler
    text=$(cat "testing"/"test_cases"/"${1}.in")

    # compile code > "testing"/"output"/"${1}-temp.out"
    ./mycc "${1}" <<< $text > "testing"/"output"/"${1}-temp.out"

    # remove first line which just contains code which was passed into compiler
    awk NR\>1 "testing"/"output"/"${1}-temp.out" > "testing"/"output"/"${1}.out"
    rm "testing"/"output"/"${1}-temp.out"
    
    expected=$(cat "testing"/"test_cases"/"${1}.out")
    output=$(cat "testing"/"output"/"${1}.out")

    echo "${BLUE}EXPEXTED:${NC} ${expected}"

    echo "${BLUE}OUTPUT:${NC} ${output}"

    expected_file="testing"/"test_cases"/"${1}.out"
    output_file="testing"/"output"/"${1}.out"

    if cmp -s "$output_file" "$expected_file"; then
        echo "${BLUE}RESULT:${NC} ${GREEN}PASSED${NC}"
    else
        echo "${BLUE}RESULT:${NC} ${RED}FAILED${NC}"
    fi
    echo ""

}

run_test operations;
run_test function;
run_test if_statement;
run_test while_loop;
run_test while_loop_continue;
run_test while_loop_break;
run_test while_loop_break_continue;
run_test recursive_factorial;
run_test returned_function;
run_test functional_argument;