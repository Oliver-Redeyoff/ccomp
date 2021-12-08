#!/usr/local/bin/zsh

cd ..

make

function run_test {

    echo "Running test ${1}"

    # get code to pass into compiler
    text=$(cat "testing"/"test_cases"/"${1}.in")

    # compile code
    ./mycc "${1}" <<< $text > "testing"/"output"/"${1}-temp.out"

    # remove first line which just contains code which was passed into compiler
    awk NR\>1 "testing"/"output"/"${1}-temp.out" > "testing"/"output"/"${1}.out"
    rm "testing"/"output"/"${1}-temp.out"
}

run_test while_loop;
run_test if_statement;
run_test recursive_factorial;
run_test returned_function;