cd ..
make
./mycc <<CODE
int x = 1;
CODE

#int main(void) {
#    int x = 5;
#    x = x + 1;
#    return x;
#}