/*
 * Graph algorithms.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

/*
 * A structure to keep record of the current running algorithm.
 */
typedef struct records {
  int numVertices;   // total number of vertices in the graph
                     // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap *heap;     // priority queue
  bool *finished;    // finished[id] is true iff vertex id is finished
                     //   i.e. no longer in the PQ
  int *predecessors; // predecessors[id] is the predecessor of vertex id
  Edge *tree;        // keeps edges for the resulting tree
  int numTreeEdges;  // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records *initRecords(Graph *graph, int startVertex);

/*
 * Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap *initHeap(Graph *graph, int startVertex);

/*
 * Returns true iff 'heap' is NULL or is empty.
 */
bool isEmpty(MinHeap *heap) { return heap->size == 0; }

/*
 * Prints the status of all current algorithm data: good for debugging.
 */
void printRecords(Records *records);

/*
 * Add a new edge to records at index ind.
 */
void addTreeEdge(Records *records, int ind, int fromVertex, int toVertex,
                 int weight);

/*
 * Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList *makePath(Edge *distTree, int vertex, int startVertex);

// sets every vertex to infinity except for startVertex and sets up helper vars
void initVars(int numVertices, Edge *mstEdges, bool *inMST, int *parent,
              int *key, int startVertex, MinHeap *minHeap) {
  for (int v = 0; v < numVertices; v++) {
    key[v] = INT_MAX;
    parent[v] = -1;
    inMST[v] = false;
    insert(minHeap, key[v], v);
  }
  key[startVertex] = 0;
  decreasePriority(minHeap, startVertex, key[startVertex]);
  return;
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
Edge *getMSTprim(Graph *graph, int startVertex) {
  int numVertices = graph->numVertices;
  Edge* mstEdges = (Edge *)malloc((numVertices - 1) * sizeof(Edge));
  bool* inMST = (bool *)malloc(numVertices * sizeof(bool));
  int* parent = (int *)malloc(numVertices * sizeof(int));

  // for picking minimum weight
  int* key = (int*)malloc(numVertices * sizeof(int));
  MinHeap *minHeap = newHeap(numVertices);

  if (mstEdges == NULL || inMST == NULL || parent == NULL || key == NULL ||
      minHeap == NULL) {
    return NULL;
  }

  int edgeCount = 0;

  initVars(numVertices, mstEdges, inMST, parent, key, startVertex, minHeap);

  // Prim's
  while (!isEmpty(minHeap)) {
    HeapNode minNode = extractMin(minHeap);
    int u = minNode.id;
    inMST[u] = true;

    EdgeList *adjList = graph->vertices[u]->adjList;

    // add an edge from parent[u] to u
    if (parent[u] != -1) {
      mstEdges[edgeCount].fromVertex = u;
      mstEdges[edgeCount].toVertex = parent[u];
      mstEdges[edgeCount].weight = key[u];
      edgeCount++;
    }

    // for each vertex in min adjacency list,
    EdgeList* current = adjList;
    while (current != NULL) {
      int v = current->edge->toVertex;
      int weight = current->edge->weight;
      if (!inMST[v] && minHeap->indexMap[v] != -1 && weight < key[v]) {
        key[v] = weight;
        decreasePriority(minHeap, v, key[v]);
        parent[v] = u;
      }
      current = current->next;
    }
  }

  free(inMST);
  free(parent);
  free(key);
  deleteHeap(minHeap);

  return mstEdges;
}

Edge *getDistanceTreeDijkstra(Graph *graph, int startVertex) {
  // initialize helper variables
  MinHeap *minHeap = newHeap(graph->numVertices);
  int *distances = (int *)malloc(graph->numVertices * sizeof(int));
  int *parent = (int *)malloc(graph->numVertices * sizeof(int));
  Edge *distanceTree = (Edge *)malloc(graph->numVertices * sizeof(Edge));

  // initialize heap
  for (int i = 0; i < graph->numVertices; ++i) {
    insert(minHeap, INT_MAX, i);
    distances[i] = INT_MAX;
    parent[i] = -1;
  }
  decreasePriority(minHeap, startVertex, 0);
  distances[startVertex] = 0;

  // Dijkstra's Algorithm
  while (!isEmpty(minHeap)) {
    // Extract vertex with minimum distance
    HeapNode minNode = extractMin(minHeap);
    int u = minNode.id;
    EdgeList *adjList = graph->vertices[u]->adjList;

    EdgeList *current = adjList;
    while (current != NULL) {
      Edge *edge = current->edge;
      int v = edge->toVertex;
      int weight = edge->weight;
      int newDist = distances[u] + weight;

      if (newDist < distances[v]) {
        decreasePriority(minHeap, v, newDist);
        distances[v] = distances[u] + weight;
        printf("%d->%d, newDist: %d\n", current->edge->fromVertex, current->edge->toVertex, distances[v]);

        parent[v] = u;
      }

      current = current->next;
    }
  }

  // Construct the distance tree (array of edges)
  for (int i = 0; i < graph->numVertices; ++i) {
    if (i != startVertex) {
      distanceTree[i] = *newEdge(i, parent[i], distances[i]);
    } else {
      // For the start vertex, create a self-loop edge with weight 0
      distanceTree[i] = *newEdge(i, i, 0);
    }
  }

  // no memory
  deleteHeap(minHeap);
  free(distances);
  free(parent);

  return distanceTree;
}

EdgeList **getShortestPaths(Edge *distTree, int numVertices, int startVertex) {
  // Allocate memory for the paths array
  EdgeList **paths = (EdgeList **)malloc(numVertices * sizeof(EdgeList *));

  // Initialize paths for each vertex
  for (int i = 0; i < numVertices; i++) {
    paths[i] = NULL;
  }

  // Reconstruct paths for each vertex based on distTree
  for (int v = 0; v < numVertices; v++) {
    if (v == startVertex) {
      paths[v] = NULL; // No path from startVertex to itself
    } else {
      int currentVertex = v;
      EdgeList *path = NULL;
      // Traverse distTree to reconstruct the path
      while (currentVertex != startVertex) {
        int weight = distTree[currentVertex].weight;
        Edge* edge = newEdge(distTree[currentVertex].fromVertex, distTree[currentVertex].toVertex, weight);
        // totalWeight += edge->weight;
        EdgeList *newEdgeNode = newEdgeList(edge, path);
        path = newEdgeNode;

        // Move to the parent vertex
        currentVertex = distTree[currentVertex].toVertex;
      }

      // Reverse the path to maintain correct order (startVertex to v)
      EdgeList *reversedPath = NULL;
      while (path != NULL) {
        EdgeList *nextEdge = path->next;
        path->next = reversedPath;
        reversedPath = path;
        path = nextEdge;
      }

      // Store the constructed path in paths[v]
      paths[v] = reversedPath;
    }
  }

  // adjust the weights
  for (int v = 0; v < numVertices; v++) {
    EdgeList* current = paths[v];
     
    while (current != NULL) {
      if (current->next != NULL) {
        current->edge->weight -= current->next->edge->weight;
      }
      current = current->next;
    }
  }
  return paths;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records *records) {
  if (records == NULL)
    return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++)
    printEdge(&records->tree[i]);

  printf("... done.\n");
}
