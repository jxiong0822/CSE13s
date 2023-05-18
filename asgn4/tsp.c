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

//uint32_t num_of_vertices;

Graph *make_graph_from_file(FILE *filename, bool directed) {

    //printf("trying to open file\n");

    FILE *fp = filename;

    if (fp == NULL) {
        fprintf(stderr, "file opened is NULL, stopping program");
        exit(-1);
    }

    //printf("file was opened successfully \n");

    //reading number of vertices
    uint32_t numVertices;
    if (fscanf(fp, "%u\n", &numVertices) != 1) {
        fprintf(stderr, "issue with number of vertices line, stopping program");
        exit(-1);
    }
    Graph *g = graph_create(numVertices, directed);
    //num_of_vertices = numVertices;
    //printf("numVertices = %u\n", numVertices);
    //printf("num of vertices were successfully read\n");

    //reading and adding vertices
    for (uint32_t i = 0; i < numVertices; i += 1) {
        char name[99];
        if (fgets(name, sizeof(name), fp) == NULL) {
            fprintf(stderr, "issue while reading a vertex, stopping program");
            exit(-1);
        }
        name[strcspn(name, "\n")] = '\0';
        graph_add_vertex(g, name, i);

        //printf("Vertex added = %s\n", name);
    }

    //printf("vertices were successfully read\n");

    //reading number of edges
    uint32_t numEdges;
    if (fscanf(fp, "%u\n", &numEdges) != 1) {
        fprintf(stderr, "issue with number of edges line, stopping program");
        printf("numedges was found to be %u\n", numEdges);
        exit(-1);
    }

    //printf("num of edges were successfully read\n");

    //reading and adding edges
    for (uint32_t i = 0; i < numEdges; i += 1) {
        uint32_t start, end, weight;
        if (fscanf(fp, "%u %u %u\n", &start, &end, &weight) == 3) {
            graph_add_edge(g, start, end, weight);
        } else {
            fprintf(stderr, "issue while reading a edge, stopping program");
            exit(-1);
        }
    }

    //printf("edges were successfully read\n");
    //graph_print(g);
    return g;
}
/*
void dfs(uint32_t vertex, Graph *g, Path *p, FILE *outputFile, Path **validPaths, uint32_t *numValidPaths) {
	graph_visit_vertex(g, vertex);
	path_add(p, vertex, g);
	
	if (path_vertices(p) == num_of_vertices) {
		//only prints the path if a route home is avaliable
		if (graph_get_weight(g, vertex, START_VERTEX) != 0) {
			path_add(p, START_VERTEX, g);
			//path_print(p, outputFile, g);
			path_copy(validPaths[*numValidPaths], p);
			
			
			//path_print(validPaths[*numValidPaths], outputFile, g);
			//fprintf(outputFile, "\n");
			
			(*numValidPaths) += 1;
			
			//printf("Path distance %u\n",path_distance(p));
			return;
		}
	} else {
	
		for (uint32_t i = 0; i < graph_vertices(g); i += 1) {
			if (graph_get_weight(g, vertex, i) != 0 && !graph_visited(g, i)) {
				dfs(i, g, p, outputFile, validPaths, numValidPaths);
			}
		}
	}
	uint32_t v = path_remove(p, g);
	graph_unvisit_vertex(g, v);
	//path_remove(p, g);
}
*/
void dfs(Graph *g, uint32_t start, Path *p, FILE *outputFile, Path **validPaths,
    uint32_t *numValidPaths) {
    graph_visit_vertex(g, start);
    path_add(p, start, g);

    if (path_vertices(p) == graph_vertices(g)) {
        if (graph_get_weight(g, start, START_VERTEX) != 0) {
            path_add(p, 0, g);
            printf("Path generated with %d vertices requiring distance %d.\n.", path_vertices(p),
                path_distance(p));
            path_copy(validPaths[*numValidPaths], p);
            (*numValidPaths) += 1;
            printf("\n");
            return;
        }
    } else {
        // Run DFS on unvisited, adjacent nodes.
        for (uint32_t i = 0; i < graph_vertices(g); i++) {
            uint32_t weight = graph_get_weight(g, start, i);
            if (weight && !graph_visited(g, i)) {
                dfs(g, i, p, outputFile, validPaths, numValidPaths);
            }
        }
    }

    uint32_t v = path_remove(p, g);
    graph_unvisit_vertex(g, v);
}

int main(int argc, char *argv[]) {
    int ch;
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    bool directed = false;

    while ((ch = getopt(argc, argv, "i:o:dh")) != -1) {
        switch (ch) {
        case 'i':
            printf("Input file: \"%s\"\n", optarg);
            inputFile = fopen(optarg, "r");
            break;

        case 'o':
            printf("Output file: \"%s\"\n", optarg);
            outputFile = fopen(optarg, "a+");
            break;
        case 'd':
            directed = true;
            printf("graph is now directed\n");
            break;
        case 'h': printf("put a help here"); break;

        default: printf("put a help here");
        }
    }
    //printf("before graph from file\n");
    //making graph from input file
    //uint32_t numVertices;
    Graph *g = make_graph_from_file(inputFile, directed);
    //printf("after graph from file\n");
    Path *p = path_create(graph_vertices(g));
    //printf("Number of vertices = %u\n", num_of_vertices);
    Path **validPaths = malloc(graph_vertices(g) * sizeof(Path *));
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        validPaths[i] = path_create(graph_vertices(g));
    }
    uint32_t numValidPaths = 0;

    dfs(g, START_VERTEX, p, outputFile, validPaths, &numValidPaths);

    //iterate through array of validPaths to find one of least length

    if (numValidPaths == 0) {
        printf("No valid paths found.\n");
    }

    uint32_t current_min = UINT32_MAX;
    Path *current_min_path = path_create(graph_vertices(g));

    printf("number of valid paths: %u\n", numValidPaths);
    for (uint32_t i = 0; i < numValidPaths; i += 1) {
        if (path_distance(validPaths[i]) < current_min) {
            current_min = path_distance(validPaths[i]);
            path_copy(current_min_path, validPaths[i]);
            printf("current min is : %u\n", path_distance(current_min_path));
        }
    }
    //prints out minimum path found
    printf("%u\n", path_distance(current_min_path));
    path_print(current_min_path, outputFile, g);

    // Free memory for each Path in the validPaths array
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        path_free(&validPaths[i]);
    }

    // Free the validPaths array
    free(validPaths);
    //closing files if necessary
    if (inputFile != stdin) {
        fclose(inputFile);
    }
    if (outputFile != stdout) {
        fclose(outputFile);
    }

    return 0;
}
