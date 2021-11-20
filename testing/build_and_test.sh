#!/usr/local/bin/zsh

cd ..

make

function run_test {
    text=$(cat "testing"/"test_cases"/"${1}.in")

    ./mycc <<< $text

    # out=$(./mycc <<< $text)
    # echo $out
}

run_test test1;

# index=1
# for entry in "testing"/"test_cases"/"test"?".in"
# do
#     text=$(cat "testing"/"test_cases"/"test${index}.in")

#     out=$(./mycc <<< $text)
#     echo $out

#     index=$(($index + 1))
# done