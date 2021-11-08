cd ..
make
./mycc <<CODE
int x;
CODE

#int main(void) {
#    int x = 5;
#    x = x + 1;
#    return x;
#}