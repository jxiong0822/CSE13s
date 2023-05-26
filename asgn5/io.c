#include "io.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct buffer {
    int fd;
    int offset;

    int num_remaining;
    uint8_t a[BUFFER_SIZE];
};

Buffer *read_open(const char *filename) {
    uint8_t open_return = open(filename, O_RDONLY);
    if (open_return < 0) {
        fprintf(stderr, "error opening file, ending program\n");
        return NULL;
    } else {
        Buffer *b = malloc(sizeof(Buffer));
        b->fd = open_return;
        b->offset = 0;
        b->num_remaining = 0;
        return b;
    }
}

void read_close(Buffer **pbuf) {
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

Buffer *write_open(const char *filename) {
    uint8_t open_return = creat(filename, 0664);
    if (open_return < 0) {
        fprintf(stderr, "error opening file, ending program\n");
        return NULL;
    } else {
        Buffer *b = malloc(sizeof(Buffer));
        b->fd = open_return;
        b->offset = 0;
        b->num_remaining = 0;
        return b;
    }
}

void write_close(Buffer **pbuf) {
    Buffer *b = *pbuf;

    if (b->offset > 0) {
        uint8_t *start = b->a;
        int num_bytes = b->offset;

        do {
            ssize_t rc = write(b->fd, start, num_bytes);
            if (rc < 0) {
                fprintf(stderr, "Error writing buffer to file\n");
                return;
            }
            if (rc == 0) {
                // Handle the case where no bytes were written
                fprintf(stderr, "Error: no bytes written\n");
                return;
            }
            start += rc;
            num_bytes -= rc;
        } while (num_bytes > 0);
        b->offset = 0;
    }

    // Close the file
    if (close(b->fd) < 0) {
        fprintf(stderr, "Error closing file\n");
        return;
    }

    // Free the buffer
    free(b);

    // Set *pbuf to NULL
    *pbuf = NULL;
}

bool read_uint8(Buffer *buf, uint8_t *x) {
    if (buf->num_remaining == 0) {
        ssize_t rc = read(buf->fd, buf->a, sizeof(buf->a));
        if (rc < 0) {
            fprintf(stderr, "error reading uint8_t, ending program\n");
        }
        if (rc == 0) {
            return false;
        }
        buf->num_remaining = rc;
        buf->offset = 0;
    }

    *x = buf->a[buf->offset];
    //^^^^^^^^^^^^^^^^^^^^^^//

    //increment offset
    buf->offset += sizeof(uint8_t);
    //decrement num_remaining
    buf->num_remaining -= sizeof(uint8_t);

    return true;
}

bool read_uint16(Buffer *buf, uint16_t *x) {
    uint8_t byte1 = 0;
    uint8_t byte2 = 0;
    if (!read_uint8(buf, &byte1) || !read_uint8(buf, &byte2)) {
        return false;
    }

    //set *x to byte2
    *x = byte2;
    //left shift *x by 8 bits
    *x <<= 8;
    //ors *x with byte1
    *x |= byte1;
    return true;
}

bool read_uint32(Buffer *buf, uint32_t *x) {
    uint16_t byte1 = 0;
    uint16_t byte2 = 0;
    if (!read_uint16(buf, &byte1) || !read_uint16(buf, &byte2)) {
        return false;
    }

    //set *x to byte2
    *x = byte2;
    //left shift *x by 16 bits
    *x <<= 16;
    //ors *x with byte1
    *x |= byte1;
    return true;
}

void write_uint8(Buffer *buf, uint8_t x) {
    if (buf->offset == BUFFER_SIZE) {
        uint8_t *start = buf->a;
        int num_bytes = buf->offset;

        do {
            ssize_t rc = write(buf->fd, start, num_bytes);
            if (rc < 0) {
                fprintf(stderr, "error writing uint8_t, ending program\n");
                return;
            }

            start += rc;
            // skip past the bytes that were just written
            num_bytes -= rc;
            // how many bytes are left?
        } while (num_bytes > 0);
        buf->offset = 0;
    }
    buf->a[buf->offset] = x;
    buf->offset += sizeof(uint8_t);
}

void write_uint16(Buffer *buf, uint16_t x) {
    uint8_t byte1 = (x & 0xFF);
    uint8_t byte2 = ((x >> 8) & 0xFF);
    write_uint8(buf, byte1);
    write_uint8(buf, byte2);
}

void write_uint32(Buffer *buf, uint32_t x) {
    uint16_t byte1 = (x & 0xFFFF);
    uint16_t byte2 = ((x >> 16) & 0xFFFF);
    write_uint16(buf, byte1);
    write_uint16(buf, byte2);
}
