#include "pq.h"

#include "node.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};
struct PriorityQueue {
    ListElement *list;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = calloc(1, sizeof(PriorityQueue));
    if (pq == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for PriorityQueue.\n");
        return NULL;
    }
    pq->list = NULL;

    return pq;
}

void pq_free(PriorityQueue **q) {
    free(*q);
    *q = NULL;
}

bool pq_is_empty(PriorityQueue *q) {
    return q->list == NULL;
}

bool pq_size_is_1(PriorityQueue *q) {
    return (q->list != NULL) && (q->list->next == NULL);
}

bool pq_less_than(Node *n1, Node *n2) {
    if (n1->weight < n2->weight)
        return true;
    if (n1->weight > n2->weight)
        return false;
    return n1->symbol < n2->symbol;
}

void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *e = calloc(1, sizeof(ListElement));
    e->tree = tree;
    e->next = NULL;

    if (pq_is_empty(q)) {
        q->list = e;
        return;
    }

    if (pq_less_than(e->tree, q->list->tree)) {
        e->next = q->list;
        q->list = e;
    } else {
        ListElement *current = q->list;
        while (current->next != NULL && !pq_less_than(e->tree, current->next->tree)) {
            current = current->next;
        }
        // Insert after the current element
        if (current->next != NULL) {
            e->next = current->next;
        }
        current->next = e;
    }
}

bool dequeue(PriorityQueue *q, Node **tree) {
    if (pq_is_empty(q)) {
        return false;
    } else {
        ListElement *e = q->list;
        q->list = q->list->next;
        *tree = e->tree;
        free(e);
        e = NULL;
        return true;
    }
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree, '<', 2);
        e = e->next;
    }
    printf("=============================================\n");
}
