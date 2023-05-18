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

void dfs(uint32_t vertex, Graph *g, Path *p, FILE *outputFile) {
  graph_visit_vertex(g, vertex);
  path_add(p, vertex, g);
  
  if (path_vertices(p) == graph_vertices(g)) {
    //path_add(p, 0, g);
    path_print(p, outputFile, g);
    fprintf(outputFile, "Path distance %u\n", path_distance(p));
    printf("Path found and completed\n\n\n");
    return;
  } else {
  //for all of n's edges that have a connection
  	/*
  	 for (uint32_t i = 0; i < graph_vertices(g); i += 1) {
  	 	if (graph_get_weight(g, vertex, i) && !graph_visited(g,i)) {
  	 		const char* vertexName = graph_get_vertex_name(g,i);
  	 		const char* curvertexName = graph_get_vertex_name(g,vertex);
  	 		printf("%s is connected to : %s\n", curvertexName, vertexName);
  	 	}
  	 }
  	*/
	  for (uint32_t i = 0; i < graph_vertices(g); i += 1) {
	  	if (graph_get_weight(g, vertex, i) && !graph_visited(g,i)) {
	  		const char* vertexName = graph_get_vertex_name(g,i);
  	 		printf("Vertex is jumping to : %s\n", vertexName);
	  		dfs(i, g, p, outputFile);
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
            printf("Input file: \"%s\"\n", optarg);
            inputFile = fopen(optarg, "r");
            break;

        case 'o':
            printf("Output file: \"%s\"\n", optarg);
            outputFile = fopen(optarg, "w");
            break;
        case 'd':
            directed = true;
            printf("graph is now directed\n");
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
    Path **validPaths = malloc(graph_vertices(g) * sizeof(Path *));
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        validPaths[i] = path_create(graph_vertices(g));
    }
    //uint32_t numValidPaths = 0;

    dfs(START_VERTEX, g, p, outputFile);

    // iterate through array of validPaths to find one of least length

    //if (numValidPaths == 0) {
    //    printf("No valid paths found.\n");
    //}
    
    /*
    uint32_t current_min = 500;
    Path *current_min_path = path_create(graph_vertices(g));

    for (uint32_t i = 0; i < numValidPaths; i++) {
        if (path_distance(validPaths[i]) <= current_min) {
            path_copy(current_min_path, validPaths[i]);
        }
    }
    // prints out minimum path found

    path_print(current_min_path, outputFile, g);
    
    */
    
    // Free memory for each Path in the validPaths array
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        path_free(&validPaths[i]);
    }

    // Free the validPaths array
    free(validPaths);
    // closing files if necessary
    if (inputFile != stdin) {
        fclose(inputFile);
    }
    if (outputFile != stdout) {
        fclose(outputFile);
    }

    return 0;
}
