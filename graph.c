/*
 * Our graph implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge)
{
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList* head)
{
  while (head != NULL)
  {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex* vertex)
{
  if (vertex == NULL)
  {
    printf("NULL");
  }
  else
  {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph* graph)
{
  if (graph == NULL)
  {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++)
  {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
*********************************************************************/

Edge* newEdge(int fromVertex, int toVertex, int weight)
{
  Edge* edge = (Edge*)calloc(1, sizeof(Edge));
  if (edge == NULL) {
    return NULL;
  }
  edge->fromVertex = fromVertex;
  edge->toVertex = toVertex;
  edge->weight = weight;
  return edge;
}

EdgeList* newEdgeList(Edge* edge, EdgeList* next)
{
  EdgeList* edge_list = (EdgeList*)calloc(1, sizeof(EdgeList));
  if (edge_list == NULL) {
    return edge_list;
  }
  edge_list->edge = edge;
  edge_list->next = next;
  return edge_list;
}

Vertex* newVertex(int id, void* value, EdgeList* adjList)
{
  Vertex* vertex = (Vertex*)calloc(1, sizeof(Vertex));
  if (vertex == NULL) {
    return NULL;
  }
  vertex->id = id;
  vertex->value = value;
  vertex->adjList = adjList;
  return vertex;
}

Graph* newGraph(int numVertices)
{
  // Allocate memory for the graph structure
  Graph* graph = (Graph*)calloc(1, sizeof(Graph));
  if (graph == NULL) {
    return NULL;
  }

  // Initialize the number of vertices and edges
  graph->numVertices = numVertices;
  graph->numEdges = 0;

  // Allocate memory for the array of vertex pointers
  graph->vertices = (Vertex**)calloc(numVertices, sizeof(Vertex*));
  if (graph->vertices == NULL) {
    free(graph);
    return NULL;
  }

  // Initialize each vertex pointer to NULL initially
  for (int i = 0; i < numVertices; ++i) {
    graph->vertices[i] = NULL;
  }

  return graph;
}

void deleteEdgeList(EdgeList* head)
{
  EdgeList* current = head;
	
	while (current != NULL) {
    EdgeList* next = current->next;
		free(current->edge);
    free(current);
		current = next;
	}

  return;
}

void deleteVertex(Vertex* vertex)
{
  deleteEdgeList(vertex->adjList);
  free(vertex);
  return;
}

void deleteGraph(Graph* graph)
{
  for (int i = 0; i < graph->numVertices; i++) {
    deleteVertex(graph->vertices[i]);
  }

  free(graph->vertices);
  free(graph);
}