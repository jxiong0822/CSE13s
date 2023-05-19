#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

void graph_free(Graph **gp) {
    Graph *g = *gp;
    free(g->visited);
    free(g->names);
    for (uint32_t i = 0; i < g->vertices; ++i) {
        free(g->weights[i]);
    }
    free(g->weights);
    free(g);
    *gp = NULL;
}
/*
uint32_t graph_vertices(const Graph *g) {
    uint32_t numVertices = 0;
    for (uint32_t i = 0; i < g->vertices; i += 1) {
        if (g->names[i] != NULL) {
            numVertices += 1;
        }
    }
    return numVertices;
}
*/
uint32_t graph_vertices(const Graph *g) {
    return g->vertices;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v]) {
        free(g->names[v]);
    }
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return g->names[v];
}

char **graph_get_names(const Graph *g) {
    return g->names;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    g->weights[start][end] = weight;

    if (!(g->directed)) {
        g->weights[end][start] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return g->weights[start][end];
}

void graph_visit_vertex(const Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(Graph *g, uint32_t v) {
    return g->visited[v];
}

void graph_print(const Graph *g) {
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("Vertex %u: %s\n", i, g->names[i]);
        for (uint32_t j = 0; j < g->vertices; j++) {
            uint32_t weight = g->weights[i][j];
            if (weight != 0) {
                const char *connectedname = graph_get_vertex_name(g, j);
                printf("Weight to vertex %u (%s): %u\n", j, connectedname, weight);
            }
        }

        printf("\n");
    }
}
