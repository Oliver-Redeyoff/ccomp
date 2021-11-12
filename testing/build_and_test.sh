cd ..
make
./mycc <<CODE
int test(int x, int y) {
    return x+y;
}

int main(void) {
    int x = 2;
    return 1;
}
CODE

# int test(int x, int y) {
#     return x+y;
# }

# int main(void) {
#     int x, y=3;

#     test(2, 3);

#     return 1;
# }