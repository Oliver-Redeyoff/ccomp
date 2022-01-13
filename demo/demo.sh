#!/usr/local/bin/zsh

cd ..

#make clean
make

text=$(cat "demo/demo.in")

./mycc demo <<< $text