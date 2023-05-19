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
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (stack_full(p->vertices)) {
        return;
    }
    if (stack_size(p->vertices) >= 1) {
        uint32_t prev_vertex;
        stack_peek(p->vertices, &prev_vertex);
        uint32_t dist = graph_get_weight(g, prev_vertex, val);
        p->total_weight += dist;
    }
    stack_push(p->vertices, val);
}
//Path remove may have an issue
/*
uint32_t path_remove(Path *p, const Graph *g) {
    if (stack_size(p->vertices) >= 2) {
        uint32_t popped;
        stack_pop(p->vertices, &popped);

        if (stack_size(p->vertices) > 1) {
            uint32_t val;
            uint32_t prev_vertex = stack_peek(p->vertices, &val);

            uint32_t dist = graph_get_weight(g, prev_vertex, popped);
            p->total_weight -= dist;
        } else {
            p->total_weight = 0;
        }
        return popped;
    } else {
        p->total_weight = 0;
        return 0;
    }
}
*/
uint32_t path_remove(Path *p, const Graph *g) {
    if (stack_empty(p->vertices)) {
        return 0;
    }
    uint32_t popped;
    if (stack_size(p->vertices) >= 3) {
        stack_pop(p->vertices, &popped);

        uint32_t prev_vertex;
        stack_peek(p->vertices, &prev_vertex);

        uint32_t dist = graph_get_weight(g, prev_vertex, popped);
        p->total_weight -= dist;
    } else {
        stack_pop(p->vertices, &popped);
        p->total_weight = 0;
    }
    return popped;
}

void path_copy(Path *dst, const Path *src) {
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

/*
void path_print(const Path *p, FILE *outfile, const Graph *g) {
    uint32_t size = stack_size(p->vertices);
    Stack *tempstack = stack_create(size);
    Stack *originalstack = stack_create(size);

    stack_copy(tempstack, p->vertices);
    stack_copy(originalstack, p->vertices);

    while (!stack_empty(tempstack)) {
        uint32_t vertex;
        stack_pop(tempstack, &vertex);
        const char *name = graph_get_vertex_name(g, vertex);
        fprintf(outfile, "%s\n", name);
    }
    stack_copy(p->vertices, originalstack);
    
    stack_free(&tempstack);
    stack_free(&originalstack);

}
*/

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    Stack *tempstack = stack_create(stack_size(p->vertices));
    Stack *originalstack = stack_create(stack_size(p->vertices));

    stack_copy(originalstack, p->vertices);
    printf("Alissa starts at:\n");
    
    uint32_t vertex;
    while (!stack_empty(originalstack)) {
        stack_pop(originalstack, &vertex);
        stack_push(tempstack, vertex);
    }

    //printf("Alissa starts at: \n");

    while (!stack_empty(tempstack)) {
        uint32_t vertex;
        stack_pop(tempstack, &vertex);
        const char *name = graph_get_vertex_name(g, vertex);
        fprintf(outfile, "%s\n", name);
    }

    //stack_copy(p->vertices, originalstack);
    printf("Total Distance: %d\n", path_distance(p));
    stack_free(&tempstack);
    stack_free(&originalstack);
}

/*
void path_print(const Path *p, const Graph *g) {
    uint32_t last = 0;
    uint32_t second_to_last = 0;

    Stack *copy = stack_create(stack_size(p->vertices));
    stack_copy(copy, p->vertices);

    Stack *correct_copy = stack_create(stack_size(p->vertices));

    // Reversing the order.
    for (uint32_t i = 0; i < path_vertices(p); i++) {
        stack_pop(copy, &last);
        stack_push(correct_copy, last);
    }

    printf("Alissa starts at: \n");

    for (uint32_t i = 0; i < path_vertices(p); i++) {
        stack_pop(correct_copy, &last);
        stack_peek(correct_copy, &second_to_last);
        printf("Vertex %d to %d : %s to %s | Weight: %d\n", last, second_to_last,
            graph_get_vertex_name(g, last), graph_get_vertex_name(g, second_to_last),
            graph_get_weight(g, last, second_to_last));
        // printf("%s\n", graph_get_vertex_name(g, last));
    }

    printf("Total Distance: %d\n", path_distance(p));
}
*/
