#include "graph.h"
#include "path.h"
#include "stack.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void stack_test(void) {
    //printf("Running one item stack test\n");
    // Create a stack that can only hold one item
    Stack *s = stack_create(4);
    // Add 10 to the stack
    assert(stack_push(s, 10));
    //printf("stack has been made, and 10 has been added.\n");
    // make sure that the top of the stack is 10 when we peek!
    uint32_t x = 0;
    assert(stack_peek(s, &x));

    assert(x == 10);

    // make sure the stack is full!
    assert(!stack_full(s));
    x = 2;

    assert(stack_push(s, 20));

    assert(stack_push(s, 30));

    assert(stack_push(s, 40));

    assert(stack_full(s));
    // Make sure we can remove the item from the stack
    stack_pop(s, &x);
    assert(x == 40);

    stack_pop(s, &x);
    assert(x == 30);

    stack_pop(s, &x);
    assert(x == 20);

    stack_pop(s, &x);
    assert(x == 10);

    assert(stack_empty(s));

    // Free up so valgrind passes
    stack_free(&s);
    //printf("One item stack tests complete. Be sure to rerun with valgrind!\n");
}

void graph_test(void) {
    Graph *g = graph_create(10, false);
    graph_add_vertex(g, "Joe's nutsack massage", 0);
    assert(strcmp(graph_get_vertex_name(g, 0), "Joe's nutsack massage") == 0);

    graph_add_vertex(g, "Bob's bbq", 1);
    assert(strcmp(graph_get_vertex_name(g, 1), "Bob's bbq") == 0);

    graph_add_vertex(g, "Henry's bank", 2);

    graph_add_edge(g, 0, 1, 1000);
    graph_add_edge(g, 1, 2, 400);
    graph_add_edge(g, 2, 0, 200);
    assert(graph_get_weight(g, 0, 1) == 1000);

    graph_visit_vertex(g, 1);
    assert(graph_visited(g, 1) == true);

    graph_print(g);

    graph_free(&g);
}

void path_test(void) {
    Graph *g = graph_create(10, false);
    graph_add_vertex(g, "Joe's nutsack massage", 0);
    graph_add_vertex(g, "Bob's bbq", 1);
    graph_add_vertex(g, "Jason's bank", 2);

    graph_add_edge(g, 0, 1, 1000);
    graph_add_edge(g, 1, 2, 200);

    Path *p = path_create(5);

    assert(path_vertices(p) == 0);
    assert(path_distance(p) == 0);

    path_add(p, 0, g);
    path_add(p, 1, g);

    assert(path_distance(p) == 1000);

    path_add(p, 2, g);

    assert(path_distance(p) == 1200);

    path_remove(p, g);

    assert(path_distance(p) == 1000);

    path_free(&p);
}

int main(void) {
    stack_test();
    graph_test();
    path_test();
    return 0;
}
