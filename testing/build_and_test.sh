#!/usr/local/bin/zsh

cd ..

make

function run_test {
    text=$(cat "testing"/"test_cases"/"${1}.in")
    ./mycc <<< $text
}

run_test test1;