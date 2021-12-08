#!/usr/local/bin/zsh

cd ..

make

function run_test {
    text=$(cat "testing"/"test_cases"/"${1}.in")
    ./mycc <<< $text
}

run_test while_loop;
#run_test recursive_factorial;
#run_test returned_function;