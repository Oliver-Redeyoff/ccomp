cd ..
make
./mycc <<CODE
int main(void) {
    int x = 5;
    x = x + 1;
    return x;
}
CODE