#include "bitwriter.h"
#include "io.h"
#include "node.h"
#include "pq.h"

#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t fill_histogram(Buffer *inbuf, double *histogram) {
    //memset(histogram, 0, 256 * sizeof(double));

    uint8_t filesize = 0;

    uint8_t byte;
    while (read_uint8(inbuf, &byte)) {
        ++histogram[byte];
        printf("%d\n", byte);
        ++filesize;
    }

    ++histogram[0x00];
    ++histogram[0xff];

    printf("filesize is %u\n", filesize);
    return filesize;
}

void count_leaves(Node *node, uint16_t *num_leaves) {
    if (node->left == NULL && node->right == NULL) {
        ++(*num_leaves);
    } else {
        if (node->left != NULL) {
            count_leaves(node->left, num_leaves);
        }
        if (node->right != NULL) {
            count_leaves(node->right, num_leaves);
        }
    }
}

void free_tree(Node *node) {
    if (node->left != NULL) {
        free_tree(node->left);
    }
    if (node->right != NULL) {
        free_tree(node->right);
    }
    node_free(&node);
}

Node *create_tree(double *histogram, uint16_t *num_leaves) {
    PriorityQueue *pq = pq_create();
    *num_leaves = 0;

    for (int i = 0; i < 256; i++) {
        if (histogram[i] > 0) {
            char symbol = i;
            Node *node = node_create(symbol, histogram[i]);
            enqueue(pq, node);
        }
    }

    //printf("printing priority queue:\n");
    //pq_print(pq);

    while (!(pq_size_is_1(pq)) && !(pq_is_empty(pq))) {
        Node *left, *right;

        dequeue(pq, &left);
        dequeue(pq, &right);

        Node *parentNode = node_create(0, left->weight + right->weight);
        //maybe 0 causes segfault

        parentNode->left = left;
        parentNode->right = right;

        enqueue(pq, parentNode);
    }
    Node *papaNode;
    //--(*num_leaves);

    printf("printing priority queue:\n");
    pq_print(pq);

    dequeue(pq, &papaNode);
    count_leaves(papaNode, num_leaves);

    /////////////////////////////////////////not sure if this free breaks the code
    pq_free(&pq);

    return papaNode;
}

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    /*
	Code *code_table = malloc(256 * sizeof(Code));
	for (i = 0; i < 256; i++) {
		code_table[i]->code = 0;
		code_table[i]->code_length = 0;
	}	
	*/
    if (node->right != NULL || node->left != NULL) {
        if (node->left != NULL) {
            fill_code_table(code_table, node->left, code, code_length + 1);
        }
        code |= 1 << code_length;
        if (node->right != NULL) {
            fill_code_table(code_table, node->right, code, code_length + 1);
        }
    } else {
        /* Leaf node: store the Huffman Code. */
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;

        /*
	printf("symbol is %c\n",node->symbol);
	printf("code is %lu\n", code);
	printf("code length is %hhu\n", code_length);
	*/
    }
}

void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left != NULL || node->right != NULL) {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    } else {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    }
}

void huff_compress_file(BitWriter *outbuf, Buffer *inbuf, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);

    //printf("bit writing completed\n");

    uint8_t byte;
    uint64_t code;
    uint8_t code_length;

    //printf("hello there\n");

    while (read_uint8(inbuf, &byte)) {
        //printf("starting loop\n");
        code = code_table[byte].code;
        code_length = code_table[byte].code_length;

        //char letter = byte;
        //printf("Byte: %c, Code: %lx, Code Length: %hhu\n", letter, code, code_length);

        for (uint8_t i = 0; i < code_length; i++) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
    }
    //bit_write_close(&outbuf);
}

int main(int argc, char *argv[]) {
    int ch;
    Buffer *inputFile = NULL;
    Buffer *infile = NULL;
    BitWriter *outputFile = NULL;
    bool igiven = false;
    bool ogiven = false;
    bool helpasked = false;
    opterr = 0;

    while ((ch = getopt(argc, argv, "i:o:h")) != -1) {
        switch (ch) {
        case 'i':
            //printf("Input file: \"%s\"\n", optarg);
            inputFile = read_open(optarg);
            infile = read_open(optarg);
            //printf("image opened with read\n");
            igiven = true;
            break;

        case 'o':
            //printf("Output file: \"%s\"\n", optarg);
            outputFile = bit_write_open(optarg);
            //printf("image opened with write\n");
            ogiven = true;
            break;
        case 'h':
            fprintf(stdout, "Usage: huff -i infile -o outfile\n       huff -h\n");
            helpasked = true;
            break;
        case '?': fprintf(stderr, "huff: unknown or poorly formatted option -%c\n", optopt);
        }
    }
    if (!helpasked) {
        if (!igiven) {
            fprintf(stdout, "huff:  -i option is required\n");
        }
        if (!ogiven) {
            fprintf(stdout, "huff:  -o option is required\n");
        }
        if (!igiven || !ogiven) {
            fprintf(stdout, "this is my Usage: huff -i infile -o outfile\n       huff -h\n");
        }
    }
    if (igiven || ogiven) {
        /*
    	size_t arraySize = 1000000000;  // Adjust the size as per your requirement
	    int* array = (int*)malloc(arraySize * sizeof(int));
	    if (array == NULL) {
		printf("Memory allocation failed. Exiting...\n");
		return 1;
	    }

	    // Initialize the array with 1s
	    for (size_t i = 0; i < arraySize; i++) {
		array[i] = 1;
	    }

	    // Free the allocated memory
	    free(array);
	    printf("Memory deallocated successfully.\n");
    	*/
        //step 1
        double *histogram = calloc(256, sizeof(double));
        uint64_t filesize = fill_histogram(inputFile, histogram);
        //printf("Step 1 completed\n");

        //printf("filesize found to be: %lu\n", filesize);

        //step 2
        uint16_t num_leaves = 0;
        Node *code_tree = create_tree(histogram, &num_leaves);

        //printf("numleaves is %u\n", num_leaves);

        //printf("printing tree\n");
        //node_print_tree(code_tree, '_', 4);
        //printf("Step 2 completed\n");

        //step 3
        Code *code_table = calloc(256, sizeof(Code));
        for (int i = 0; i < 256; i++) {
            code_table[i].code = 0;
            code_table[i].code_length = 0;
        }
        fill_code_table(code_table, code_tree, 0, 0);
        //printf("Step 3 completed\n");

        //step 4
        read_close(&inputFile);
        //read_open(inputFile);
        inputFile = infile;
        //printf("Step 4 completed\n");

        //step 5
        huff_compress_file(outputFile, inputFile, filesize, num_leaves, code_tree, code_table);
        //printf("Step 5 completed\n");

        free(histogram);
        free(code_table);
        free_tree(code_tree);
        bit_write_close(&outputFile);
        read_close(&inputFile);
    }
}
