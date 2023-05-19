#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph *make_graph_from_file(FILE *filename, bool directed) {

    // printf("trying to open file\n");

    FILE *fp = filename;

    if (fp == NULL) {
        fprintf(stderr, "file opened is NULL, stopping program");
        exit(-1);
    }

    // printf("file was opened successfully \n");

    // reading number of vertices
    uint32_t numVertices;
    if (fscanf(fp, "%u\n", &numVertices) != 1) {
        fprintf(stderr, "issue with number of vertices line, stopping program");
        exit(-1);
    }
    Graph *g = graph_create(numVertices, directed);

    // printf("numVertices = %u\n", numVertices);
    // printf("num of vertices were successfully read\n");

    // reading and adding vertices
    for (uint32_t i = 0; i < numVertices; i += 1) {
        char name[99];
        if (fgets(name, sizeof(name), fp) == NULL) {
            fprintf(stderr, "issue while reading a vertex, stopping program");
            exit(-1);
        }
        name[strcspn(name, "\n")] = '\0';
        graph_add_vertex(g, name, i);

        // printf("Vertex added = %s\n", name);
    }

    // printf("vertices were successfully read\n");

    // reading number of edges
    uint32_t numEdges;
    if (fscanf(fp, "%u\n", &numEdges) != 1) {
        fprintf(stderr, "issue with number of edges line, stopping program");
        printf("numedges was found to be %u\n", numEdges);
        exit(-1);
    }

    // printf("num of edges were successfully read\n");

    // reading and adding edges
    for (uint32_t i = 0; i < numEdges; i += 1) {
        uint32_t start, end, weight;
        if (fscanf(fp, "%u %u %u\n", &start, &end, &weight) == 3) {
            graph_add_edge(g, start, end, weight);
        } else {
            fprintf(stderr, "issue while reading a edge, stopping program");
            exit(-1);
        }
    }

    // printf("edges were successfully read\n");
    //graph_print(g);
    return g;
}

void dfs(uint32_t vertex, Graph *g, Path *p, FILE *outputFile, uint32_t *min, Path *min_p) {
    graph_visit_vertex(g, vertex);
    path_add(p, vertex, g);

    if (path_vertices(p) == graph_vertices(g)) {
        if (graph_get_weight(g, vertex, START_VERTEX) != 0) {
            if (path_distance(p) < *min) {
                *min = path_distance(p);
                path_copy(min_p, p);
                //fprintf(outputFile, "new minpath found, min distance is %u\n", min + graph_get_weight(g, vertex, START_VERTEX));

                //path_print(p, outputFile, g);
                //fprintf(outputFile, "Path distance %u\n", path_distance(p));
            }
        }
    } else {
        for (uint32_t i = 0; i < graph_vertices(g); i += 1) {
            if (graph_get_weight(g, vertex, i) > 0 && !graph_visited(g, i)) {
                dfs(i, g, p, outputFile, min, min_p);
            }
        }
    }

    path_remove(p, g);
    graph_unvisit_vertex(g, vertex);
}

int main(int argc, char *argv[]) {
    int ch;
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    bool directed = false;

    while ((ch = getopt(argc, argv, "i:o:dh")) != -1) {
        switch (ch) {
        case 'i':
            //printf("Input file: \"%s\"\n", optarg);
            inputFile = fopen(optarg, "r");
            break;

        case 'o':
            //printf("Output file: \"%s\"\n", optarg);
            outputFile = fopen(optarg, "w");
            break;
        case 'd':
            directed = true;
            //printf("graph is now directed\n");
            break;
        case 'h': printf("put a help here"); break;

        default: printf("put a help here");
        }
    }
    // printf("before graph from file\n");
    // making graph from input file
    // uint32_t numVertices;
    Graph *g = make_graph_from_file(inputFile, directed);
    // printf("after graph from file\n");
    Path *p = path_create(graph_vertices(g));
    //printf("Number of vertices = %u\n", path_vertices(p));
    //printf("Number of vertices of full graph = %u\n", graph_vertices(g));
    Path *min_p = path_create(graph_vertices(g) + 1);
    uint32_t min = UINT32_MAX;

    dfs(START_VERTEX, g, p, outputFile, &min, min_p);

    //print minimum path found
    if (min == UINT32_MAX) {
        printf("No path found! Alissa is lost!\n");
    } else {
        printf("Alissa starts at:\n");

        path_add(min_p, START_VERTEX, g);
        printf("Total Distance: %d\n", path_distance(min_p));
        path_print(min_p, outputFile, g);
    }
    // closing files if necessary
    if (inputFile != stdin) {
        fclose(inputFile);
    }
    if (outputFile != stdout) {
        fclose(outputFile);
    }

    return 0;
}
