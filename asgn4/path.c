#include "graph.h"
#include "stack.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));

    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    return p;
}

void path_free(Path **pp) {
    if (pp != NULL && *pp != NULL) {
        if ((*pp)->vertices != NULL) {
            stack_free(&((*pp)->vertices));
            (*pp)->vertices = NULL;
        }
        free(*pp);
        *pp = NULL;
    }
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    return (p->total_weight);
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (stack_size(p->vertices) >= 1) {
        uint32_t prev_vertex;
        stack_peek(p->vertices, &prev_vertex);
        uint32_t dist = graph_get_weight(g, prev_vertex, val);
        p->total_weight += dist;
    }
    stack_push(p->vertices, val);
}

uint32_t path_remove(Path *p, const Graph *g) {
    if (stack_size(p->vertices) >= 1) {

        uint32_t popped;
        stack_pop(p->vertices, &popped);

        if (stack_size(p->vertices) >= 1) {
            uint32_t val;
            uint32_t prev_vertex = stack_peek(p->vertices, &val);

            uint32_t dist = graph_get_weight(g, prev_vertex, popped);
            p->total_weight -= dist;
        } else {
            p->total_weight = 0;
        }
        return popped;
    } else {
        return 0;
    }
}

void path_copy(Path *dst, const Path *src) {
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    uint32_t size = stack_size(p->vertices);
    Stack *tempstack = stack_create(size);

    while (!stack_empty(p->vertices)) {
        uint32_t vertex;
        stack_pop(p->vertices, &vertex);
        stack_push(tempstack, vertex);
    }

    while (!stack_empty(tempstack)) {
        uint32_t vertex;
        stack_pop(tempstack, &vertex);
        const char *name = graph_get_vertex_name(g, vertex);
        fprintf(outfile, "%s\n", name);
    }
    stack_free(&tempstack);
}
