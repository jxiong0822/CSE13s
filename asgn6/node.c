#include "node.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, double weight) {
    Node *n = calloc(1, sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for BitWriter.\n");
    }

    n->symbol = symbol;
    n->weight = weight;
    n->code = 0;
    n->code_length = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void node_free(Node **node) {
    /*
	if (*node == NULL) {
        	fprintf(stderr, "Error: Invalid BitWriter pointer.\n");
        	return;
        }
	*/
    // Free the current node
    free(*node);
    *node = NULL;
}

void node_print_tree(Node *tree, char ch, int indentation) {
    if (tree == NULL)
        return;
    node_print_tree(tree->right, '/', indentation + 3);
    printf("%*cweight = %.0f", indentation + 1, ch, tree->weight);
    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }
    printf("\n");
    node_print_tree(tree->left, '\\', indentation + 3);
}
