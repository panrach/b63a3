#include "minheap.h"
#include "graph.h"
#include "graph.c"
#include "minheap.c"
#include "graph_algos.h"
#include "graph_algos.c"
#include <stdbool.h>
#include <assert.h>

void test_prim() {
    // Example usage: Create a graph, add edges, and find MST using Prim's algorithm
    Graph* graph = newGraph(5);

    // Add edges to the graph (example)
    graph->vertices[0]->adjList = newEdgeList(newEdge(0, 1, 2), graph->vertices[0]->adjList);
    printf("poo");
    graph->vertices[1]->adjList = newEdgeList(newEdge(1, 0, 2), graph->vertices[1]->adjList);
    graph->vertices[1]->adjList = newEdgeList(newEdge(1, 2, 3), graph->vertices[1]->adjList);
    graph->vertices[2]->adjList = newEdgeList(newEdge(2, 1, 3), graph->vertices[2]->adjList);
    graph->vertices[2]->adjList = newEdgeList(newEdge(2, 3, 1), graph->vertices[2]->adjList);
    graph->vertices[3]->adjList = newEdgeList(newEdge(3, 2, 1), graph->vertices[3]->adjList);
    graph->vertices[3]->adjList = newEdgeList(newEdge(3, 4, 4), graph->vertices[3]->adjList);
    graph->vertices[4]->adjList = newEdgeList(newEdge(4, 3, 4), graph->vertices[4]->adjList);

    int startVertex = 0;
    Edge* mstEdges = getMSTprim(graph, startVertex);

    // Print the MST edges
    for (int i = 0; i < graph->numVertices - 1; i++) {
        printf("%d", i);
        printEdge(&mstEdges[i]);
    }

    // Free allocated memory
    free(mstEdges);
    deleteGraph(graph);
}

int main() {
    test_prim();
}
