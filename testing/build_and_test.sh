#!/usr/local/bin/zsh

cd ..

make

NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[1;34m'

cat << EndOfMessage
____________________________________________________________________
|                                                 _ _              |
|               ___      ___ ___  _ __ ___  _ __ (_) | ___ _ __    |
|  _____ _____ / __|    / __/ _ \| '_   _ \| '_ \| | |/ _ \ '__|   |
| |_____|_____| (__    | (_| (_) | | | | | | |_) | | |  __/ |      |
|              \___|    \___\___/|_| |_| |_| .__/|_|_|\___|_|      |
|                                          |_|                     |
|  by Oliver Redeyoff                                              |
|__________________________________________________________________|

EndOfMessage

function run_test {

    echo "${BLUE}TEST:${NC} ${1}"

    # get code to pass into compiler
    text=$(cat "testing"/"test_cases"/"${1}.in")

    # compile code > "testing"/"output"/"${1}-temp.out"
    ./mycc "${1}" <<< $text

    # # remove first line which just contains code which was passed into compiler
    # awk NR\>1 "testing"/"output"/"${1}-temp.out" > "testing"/"output"/"${1}.out"
    # rm "testing"/"output"/"${1}-temp.out"
    
    # output=$(cat "testing"/"output"/"${1}.out")
    # echo "${BLUE}OUTPUT:${NC} ${output}"

    # output="testing"/"output"/"${1}.out"
    # expected="testing"/"test_cases"/"${1}.out"

    # if cmp -s "$output" "$expected"; then
    #     echo "${BLUE}RESULT:${NC} ${GREEN}PASSED${NC}"
    # else
    #     echo "${BLUE}RESULT:${NC} ${RED}FAILED${NC}"
    # fi
    # echo ""

}

# run_test operations;
run_test while_loop;
# run_test if_statement;
# run_test recursive_factorial;
# run_test returned_function;
# run_test functional_argument;