#include "graph.h"
#include "graph.c"
#include <stdbool.h>
#include <assert.h>

/*
Expected output
(0 -- 1, 2)(1 -- 2, 3)(2 -- 3, 4)(5 -- 2, 9)(0 -- 1, 1)
*/
void test_edge() {
    printEdge(newEdge(0, 1, 2));
    printEdge(newEdge(1, 2, 3));
    printEdge(newEdge(2, 3, 4));
    printEdge(newEdge(5, 2, 9));
    printEdge(newEdge(0, 1, 1));
}

/*
Expected output
(0 -- 1, 2) --> NULL
(1 -- 2, 3) --> (0 -- 1, 2) --> NULL
(2 -- 3, 4) --> (1 -- 2, 3) --> (0 -- 1, 2) --> NULL
*/
void test_newEdgeList() {
    EdgeList* list1 = newEdgeList(newEdge(0, 1, 2), NULL);
    printEdgeList(list1);
    printf("\n");

    EdgeList* list2 = newEdgeList(newEdge(1, 2, 3), list1);
    printEdgeList(list2);
    printf("\n");

    EdgeList* list3 = newEdgeList(newEdge(2, 3, 4), list2);
    printEdgeList(list3);

    free(list1);
    free(list2);
    free(list3);
}

/*
Expected output:
1: (0 -- 1, 2) --> NULL
2: (1 -- 2, 3) --> (0 -- 1, 2) --> NULL
3: (2 -- 3, 4) --> (1 -- 2, 3) --> (0 -- 1, 2) --> NULL
*/
void test_newVertex() {
    EdgeList* list1 = newEdgeList(newEdge(0, 1, 2), NULL);
    EdgeList* list2 = newEdgeList(newEdge(1, 2, 3), list1);
    EdgeList* list3 = newEdgeList(newEdge(2, 3, 4), list2);
    
    printVertex(newVertex(1, NULL, list1));
    printf("\n");
    printVertex(newVertex(2, NULL, list2));
    printf("\n");
    printVertex(newVertex(3, NULL, list3));

    free(list1);
    free(list2);
    free(list3);
}

/*
Expected output:
1: (0 -- 1, 2) --> NULL
2: (1 -- 2, 3) --> (0 -- 1, 2) --> NULL
3: (2 -- 3, 4) --> (1 -- 2, 3) --> (0 -- 1, 2) --> NULL
NULL
NULL
*/
void test_newGraph() {
    EdgeList* list1 = newEdgeList(newEdge(0, 1, 2), NULL);
    EdgeList* list2 = newEdgeList(newEdge(1, 2, 3), list1);
    EdgeList* list3 = newEdgeList(newEdge(2, 3, 4), list2);
    Graph* graph = newGraph(5);
    
    graph->vertices[0] = newVertex(1, NULL, list1);
    graph->vertices[1] = newVertex(2, NULL, list2);
    graph->vertices[2] = newVertex(3, NULL, list3);

    printGraph(graph);
}   

void deleteEdgeList() {
    
}

int main() {
    // test_edge();
    // test_newEdgeList();
    // test_newVertex();
    test_newGraph();
    return 0;
}