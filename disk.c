
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "disk.h"

static FILE *diskfile;
static unsigned nblocks = 0;
static unsigned nreads = 0;
static unsigned nwrites = 0;

int disk_init(const char *filename, unsigned n) {
    diskfile = fopen(filename, "r+");
    if (diskfile != NULL && n == -1) {
        fseek(diskfile, 0L, SEEK_END);
        n = ftell(diskfile);
        fprintf(stderr, "filesize=%d, %d blocks\n", n, n / DISK_BLOCK_SIZE);
        n = n / DISK_BLOCK_SIZE;
    }
    if (diskfile==NULL)
        diskfile = fopen(filename, "w+");
    if (diskfile==NULL)
        return 0;

    ftruncate(fileno(diskfile), n * DISK_BLOCK_SIZE);

    nblocks = n;
    nreads = 0;
    nwrites = 0;

    return 1;
}

unsigned disk_size() { 
	return nblocks; 
}

static void sanity_check(unsigned blocknum, const void *data) {
    if (blocknum >= nblocks) {
        printf("DISK ERROR: blocknum (%d) is too big!\n", blocknum);
        abort();
    }

    if (!data) {
        printf("DISK ERROR: null data pointer!\n");
        abort();
    }
}

void disk_read(unsigned blocknum, char *data) {
    sanity_check(blocknum, data);

    fseek(diskfile, blocknum * DISK_BLOCK_SIZE, SEEK_SET);

    if (fread(data, DISK_BLOCK_SIZE, 1, diskfile) == 1) {
        nreads++;
    } else {
        printf("ERROR: couldn't access simulated disk: %s\n", strerror(errno));
        abort();
    }
}

void disk_write(unsigned blocknum, const char *data) {
    sanity_check(blocknum, data);

    fseek(diskfile, blocknum * DISK_BLOCK_SIZE, SEEK_SET);
    //printf("write block %d (byte offset %d)\n", blocknum, blocknum * DISK_BLOCK_SIZE);
    if (fwrite(data, DISK_BLOCK_SIZE, 1, diskfile) == 1) {
        nwrites++;
    } else {
        printf("DISK ERROR: couldn't access simulated disk: %s\n", strerror(errno));
        abort();
    }
}

void disk_close() {
    if (diskfile) {
        //printf("%d disk block reads\n", nreads);
        //printf("%d disk block writes\n", nwrites);
        fclose(diskfile);
        diskfile = 0;
    }
}
