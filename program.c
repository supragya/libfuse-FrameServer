/*
    PiNG12RAW converts 12bit RGGB RAW image into 8 bit PNGs
    Initial Author: Supragya Raj (supragyaraj@gmail.com)
*/
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


// LibFUSE specific details
// Provides with the file where the dump of channel will be available to access, the pathname and the filename
static const char *filepath = "/redbytedump";
static const char *filename = "redbytedump";


// Dictates how many LSBs are lost while conversion from 12bit input to the final outputs
// Generally for 12 bit input has to be converted to 8 bit output. right shift = 4 while left shift = 0
// NOTE: code not extended for 16bit outputs
const int R_SHIFTING_FACTOR = 4;

// Input file location
const char INPUT_FILE[] = "test_image/test.raw12";

// Width and Height of the image to be processes
const int WIDTH = 4096;
const int HEIGHT = 3072;

// The buffers to keep the values of bytes of different channels

char *imageredgr;

// Function declarations (nothing much to look here)
static int getattr_callback(const char *path, struct stat *stbuf);

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
                            off_t offset, struct fuse_file_info *fi);

static int open_callback(const char *path, struct fuse_file_info *fi);

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi);

void preprocess();

// LibFUSE function references to be utilized
static struct fuse_operations fuse_example_operations = {
        .getattr = getattr_callback,
        .open = open_callback,
        .read = read_callback,
        .readdir = readdir_callback,
};

int main(int argc, char *argv[]) {
    imageredgr = (char *) malloc(WIDTH * HEIGHT * 4);
    preprocess();

    // Begin LibFUSE, register the functions with libfuse by sending function references
    return fuse_main(argc, argv, &fuse_example_operations, NULL);
}

void preprocess() {
    uint8_t eightbits[3];
    uint16_t left, right;

    uint8_t left8bit, right8bit;
    // Open the input file and make it ready
    FILE *pFile;
    pFile = fopen(INPUT_FILE, "r");
    if (!pFile) {
        printf("cannot open file\n");
        exit(-1);
    }
    for (unsigned int row = 0; row < HEIGHT; row++) {
        for (unsigned int col = 0; col < WIDTH / 2; col++) {

            //Read 3 eightbit values
            for (int bt = 0; bt < 3; bt++)
                fread(&eightbits[bt], sizeof(eightbits[bt]), 1, pFile);

            //Retrieve left and right side
            left = right = 0;
            left = ((eightbits[0] << 4) | (eightbits[1] & 0x40) >> 4);
            right = ((eightbits[1] & 0x0F) << 8 | (eightbits[2]));

            //Find the bit values
            left8bit = right8bit = 0;
            //Left8bits are : ((eightbits[0]<<4)|(eightbits[1]&0x40)>>4)>>4 so, effectively eightbits[0]
            left8bit = left >> R_SHIFTING_FACTOR;
            //Right8bits are: ((eightbits[1]&0x0F)<<4|(eightbits[2]))>>$ so effectively ((eightbits[1]&0x0F)|(eightbits[2])>>4)
            right8bit = right >> R_SHIFTING_FACTOR;

            //Set the values
            //on the even lines every second sample is a 'red' and on the odd lines every second a 'blue'

            if (row % 2 == 0) {
                imageredgr[((row / 2) * WIDTH / 2 + col)] = (int) left8bit;
//                imagegreen1gr[((row/2)*WIDTH/2 + col)] = (int)right8bit;
            } else {
//                imagegreen2gr[((row/2)*WIDTH/2 + col)] = (int)left8bit;
//                imagebluegr[((row/2)*WIDTH/2 + col)] = (int)right8bit;
            }
        }
    }

    fclose(pFile);
}

// LibFUSE specific functions

// The getattr callback is in charge of reading the metadata of a given path
// this callback is always called before any operation made on the filesystem.
// What we are doing here is simple:

// if the value of path equals to root /, we declare it as a directory and return.
// if the value of path equals to filepath /file, we declare it as a file and explicit its size and then return.
// Otherwise nothing exists at the given path, and we return -ENOENT.
//
// As you can see, we are telling FUSE that the current entry is a file or a directory using the stat struct.
//
// In general, if the entry is a directory, st_mode have to be set to S_IFDIR and st_nlink to 2, while if it’s a file
// st_mode have to be set to S_IFREG (that stands for regular file) and st_nlink to 1.
// Files also require that the st_size (the full file size) is specified.


static int getattr_callback(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    if (strcmp(path, filepath) == 0) {
        stbuf->st_mode = S_IFREG | 0777;
        stbuf->st_nlink = 1;
        stbuf->st_size = WIDTH * HEIGHT * 4;
        return 0;
    }

    return -ENOENT;
}

// The readdir callback has the task of telling FUSE the exact structure of the accessed directory.
// Since at the moment the only available directory is /, this function always return its representation,
// we are doing it by filling buf with the two links for the upper directory ..
// and current directory . and with the only file we have: redbytedump

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
                            off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    filler(buf, filename, NULL, 0);

    return 0;
}

// The open callback is called when the system requests for a file to be opened.
// Since we don’t have real file but only in-memory representations,
// we are going to implement this callback just because is needed for FUSE to work and therefore return 0.

static int open_callback(const char *path, struct fuse_file_info *fi) {
    return 0;
}

// This callback is called when FUSE is reading data from an opened file.
// It should return exactly the number of bytes requested and fill the second argument buf
// with the content of those bytes. As done in the getattr callback,
// here I’m checking if the given path equals to a known one,
// I copy the filecontent into the buf and then return the requested number of bytes.

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi) {

    if (strcmp(path, filepath) == 0) {
        size_t len = WIDTH * HEIGHT * 4;
        if (offset >= len) {
            return 0;
        }

        if (offset + size > len) {
            memcpy(buf, imageredgr + offset, len - offset);
            return len - offset;
        }

        memcpy(buf, imageredgr + offset, size);
        return size;
    }

    return -ENOENT;
}
