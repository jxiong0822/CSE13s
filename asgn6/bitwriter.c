#include "bitwriter.h"

#include "io.h"

#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct BitWriter {
    Buffer *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

struct buffer {
    int fd;
    int offset;
    int num_remaining;
    uint8_t a[BUFFER_SIZE];
};

BitWriter *bit_write_open(const char *filename) {
    //allocate memory for bitwriter object
    BitWriter *buf = calloc(1, sizeof(BitWriter));
    if (buf == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for BitWriter.\n");
    }

    Buffer *underlying_stream = write_open(filename);

    if (underlying_stream == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'.\n", filename);
        free(buf);
        return NULL;
    }

    buf->underlying_stream = underlying_stream;

    return buf;
}

void bit_write_close(BitWriter **pbuf) {
    /*
	if (pbuf == NULL || *pbuf == NULL) {
        	fprintf(stderr, "Error: Invalid BitWriter pointer.\n");
        	return;
        }
        */

    BitWriter *buf = *pbuf;

    if (buf->bit_position > 0) {
        write_uint8(buf->underlying_stream, buf->byte);
    }
    write_close(&(buf->underlying_stream));

    free(buf);
    *pbuf = NULL;
}

void bit_write_bit(BitWriter *buf, uint8_t x) {
    if (buf->bit_position > 7) {
        write_uint8(buf->underlying_stream, buf->byte);
        buf->byte = 0x00;
        buf->bit_position = 0;
    }

    if (x & 1) {
        buf->byte |= (x & 1) << buf->bit_position;
    }

    ++buf->bit_position;
}

void bit_write_uint8(BitWriter *buf, uint8_t x) {
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (x >> i) & 1;
        bit_write_bit(buf, bit);
    }
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i < 16; i++) {
        uint8_t bit = (x >> i) & 1;
        bit_write_bit(buf, bit);
    }
}

void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i < 32; i++) {
        uint8_t bit = (x >> i) & 1;
        bit_write_bit(buf, bit);
    }
}
