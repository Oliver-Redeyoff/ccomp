#!/usr/local/bin/zsh

index=1
for entry in "test_cases"/"test"?".in"
do
    text=$(cat "test_cases"/"test${index}.in")

    out=$(../mycc <<< $text)
    echo $out

    index=$(($index + 1))
done

# cd ..
# make
# ./mycc <<CODE
# int test(int x, int y) {
#     return x+y;
# }

# int main(void) {
#     void o = "test";
#     int x = 2;
#     return 1;
# }
# CODE