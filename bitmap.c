
#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"

#define WORDSZ (8*(int)sizeof(bitmap_t))  // bits in each bitmap word

// bitmap will be represented over an array of bitmap_t defined in bitmap.h

// in the following functions
// b pointes to an bitmap allocated by bitmap_alloc
// n should always be >= 0 and < allocated bits (but is not checked)


void bitmap_set(bitmap_t *b, unsigned n) {
    int word = n / WORDSZ;
    int offset = n % WORDSZ;
    b[word] |= 1 << offset;
}

void bitmap_clear(bitmap_t *b, unsigned n) {
    int word = n / WORDSZ;
    int offset = n % WORDSZ;
    b[word] &= ~(1 << offset);
}

int  bitmap_read(bitmap_t *b, unsigned n) {
    int word = n / WORDSZ;
    int offset = n % WORDSZ;
    return (b[word] >> offset) & 1;
}

bitmap_t * bitmap_alloc(int nbits) {
    printf("bits: %i; word size %u;  allocated %i words\n", nbits, WORDSZ, (nbits+WORDSZ-1)/WORDSZ);
    bitmap_t *b = calloc((nbits+WORDSZ-1)/WORDSZ, sizeof(bitmap_t));  // and sets it to zeros
    return b;
}

void bitmap_free(bitmap_t *b) {
    free(b);
}


// for debuging:

void bitmap_print(bitmap_t *b, unsigned size) {
    int col=0;
    for (int i = 0; i < size; i++) {
        printf("%i", bitmap_read(b, i));
        col++;
        if (col==79) {
            col=0; putchar('\n');
        } else if (col%10 == 0) {
            putchar(' ');
        } 
    }
    putchar('\n');
}
