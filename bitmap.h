
#ifndef _BITMAP_H
#define _BITMAP_H

typedef unsigned int  bitmap_t;
// bitmap will be represented over an array of unsigned ints

// in the following functions
// b pointes to an bitmap allocated by bitmap_alloc
// n should always be >= 0 and < allocated bits

void bitmap_set(bitmap_t *b, unsigned n);
void bitmap_clear(bitmap_t *b, unsigned n);
int  bitmap_read(bitmap_t *b, unsigned n);
bitmap_t *bitmap_alloc(int nbits) ;
void bitmap_free(bitmap_t *b) ;
void bitmap_print(bitmap_t *b, unsigned size) ;

#endif