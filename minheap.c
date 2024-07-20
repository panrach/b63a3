/*
 * Our Priority Queue implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

int idAt(MinHeap* heap, int nodeIndex);
int indexOf(MinHeap* heap, int id);

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Swaps contents of heap->arr[index1] and heap->arr[index2] if both
 * 'index1' and 'index2' are valid indices for minheap 'heap'.
 * Has no effect otherwise.
 */
void swap(MinHeap* heap, int index1, int index2) {  
  if (index1 >= 1 && index1 <= heap->size && index2 >= 1 && index2 <= heap->size) {  
    HeapNode temp = heap->arr[index1];  
    heap->arr[index1] = heap->arr[index2];  
    heap->arr[index2] = temp;  

    // update the index 
    heap->indexMap[idAt(heap, index1)] = index1;
    heap->indexMap[idAt(heap, index2)] = index2;
  }  
}  

/*
 * Returns the index of the parent of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getParentIdx(int nodeIndex) {
  return (nodeIndex)/2;  
}

/*
 * Returns True if 'nodeIndex' is currently a valid index in minheap 'heap'.
 * False otherwise.
 */
bool isValidIndex(MinHeap* heap, int nodeIndex) {
  return nodeIndex >= 1 && nodeIndex <= heap->size;
}

/*
 * Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex) {  
  if (!isValidIndex(heap, nodeIndex)) {  
    return NOTHING; // or any other value indicating an invalid index  
  }  
  return heap->arr[nodeIndex].priority;  
}  

/*
 * Floats up the element at index 'nodeIndex' in minheap 'heap' such that
 * 'heap' is still a minheap.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
void floatUp(MinHeap* heap, int nodeIndex) {  
  // Precondition: nodeIndex is a valid index of minheap 'heap'  
  if (nodeIndex == 1) {  
    return;  
  }  
  
  int parentIndex = getParentIdx(nodeIndex);  
  
  while (nodeIndex > 1 && priorityAt(heap, nodeIndex) < priorityAt(heap, parentIndex)) {  
    // Swap the current node with its parent  
    swap(heap, nodeIndex, parentIndex);  
      
    // Update nodeIndex to the parent's index  
    nodeIndex = parentIndex;  
    parentIndex = getParentIdx(nodeIndex);  
  }  
}

/*
 * Returns the index of the left child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getLeftChildIdx(int nodeIndex) {
  return (nodeIndex * 2);
}

/*
 * Returns the index of the right child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getRightChildIdx(int nodeIndex) {
  return (nodeIndex * 2) + 1;  

}
  
/*
 * Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex) {
  HeapNode* array = heap->arr;
  return array[nodeIndex];
}

/*
 * Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex) {
  HeapNode* array = heap->arr;
  return array[nodeIndex].id;
}

/*
 * Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id) {
  return heap->indexMap[id];
}

void changePriority(MinHeap* heap, int nodeIndex, int newPriority)  
{  
  if (!isValidIndex(heap, nodeIndex)) {  
    return;  
  }  
  
  int oldPriority = priorityAt(heap, nodeIndex);  
  heap->arr[nodeIndex].priority = newPriority;  
  
  if (newPriority < oldPriority) {  
    floatUp(heap, nodeIndex);  
  } else {  
    heapify(heap, nodeIndex);  
  }
}  

/*********************************************************************
 * Required functions
 ********************************************************************/
HeapNode getMin(MinHeap* heap)
{
  HeapNode* array = heap->arr;  
  return array[1];  
}

void heapify(MinHeap* heap, int nodeIndex)
{
  int left = getLeftChildIdx(nodeIndex);  
  int right = getRightChildIdx(nodeIndex);  
  int min = nodeIndex;  
  
  // Determine the smallest among nodeIndex, left child, and right child  
  if (left <= heap->size && priorityAt(heap, left) < priorityAt(heap, min)) {  
    min = left;  
  }  
  if (right <= heap->size && priorityAt(heap, right) < priorityAt(heap, min)) {  
    min = right;  
  }  
  
  if (min != nodeIndex) {  
    swap(heap, nodeIndex, min);  
    heapify(heap, min);
  }  
}

HeapNode extractMin(MinHeap* heap)
{
  HeapNode minNode = heap->arr[ROOT_INDEX];
  swap(heap, ROOT_INDEX, heap->size);
  (heap->indexMap)[minNode.id] = NOTHING;
  heap->size--;
  heapify(heap, 1);
  return minNode;
}

bool insert(MinHeap* heap, int priority, int id)
{
  if (heap->size == heap->capacity || id > heap->capacity) {  
    return false;  
  } 
  heap->indexMap[id] = heap->size + 1;
  heap->size++;  
  heap->arr[heap->size].priority = priority;  
  heap->arr[heap->size].id = id;  
  
  floatUp(heap, heap->size);

  return true;  
}

int getPriority(MinHeap* heap, int id)
{
  int idx = indexOf(heap, id);
  return priorityAt(heap, idx);
}

bool decreasePriority(MinHeap* heap, int id, int newPriority)
{
  int nodeIndex = indexOf(heap, id);
  if (nodeIndex >= 1 && heap->indexMap[id] != NOTHING && priorityAt(heap, nodeIndex) > newPriority) {
    changePriority(heap, nodeIndex, newPriority);
    return true;
  }
  return false;
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap)
{
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 1; i <= heap->size; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, heap->arr[i].priority,
           heap->arr[i].id, i, heap->indexMap[i]);
  printf("\n\n");
}

/***** Memory management (sample solution) **********************************/
MinHeap* newHeap(int capacity)  
{  
  MinHeap* newHeap = (MinHeap*)calloc(1, sizeof(MinHeap));  
  if (newHeap == NULL) {  
    return NULL;  
  }  
  newHeap->arr = (HeapNode*)calloc(capacity + 1, sizeof(HeapNode)); // +1 to accommodate 1-based indexing  
  if (newHeap->arr == NULL) {  
    free(newHeap);  
    return NULL;  
  }  

  newHeap->indexMap = (int*)calloc(capacity + 1, sizeof(int)); // +1 to accommodate 1-based indexing  
  if (newHeap->indexMap == NULL) {  
    free(newHeap);  
    return NULL;  
  }  
  
  newHeap->capacity = capacity;  
  newHeap->size = 0;  
  
  return newHeap;  
}  

void deleteHeap(MinHeap* heap)  
{ 
  if (heap != NULL) {
    free(heap->arr);
    free(heap->indexMap);
    free(heap); 
  }
  return;  
}  