#include <stdio.h>
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

int main() {
    char x = -7;
    char y = (~x) + 1;

    printf("x: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(x));
    printf("\n");
    printf("y: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(y));
    printf("\n");
    return 1;
}