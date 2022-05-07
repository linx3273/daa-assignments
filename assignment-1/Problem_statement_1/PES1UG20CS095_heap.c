// Name: Ayush Kumar Gupta
// SRN: PES1UG20CS095
#include "heap.h"
#include <stdlib.h>

/*
 ** Do not use global variables.
 ** Mark any functions that you declare in this file that are not in the header
 *as static
 ** Do not use static variables
 ** All occurences of count_ptr indicate that the number of comparison
 ** operations need to be stored in the location pointed by it
 ** The implementation can assume it is initialized to 0.
 */

static void max_heapify(heap_t *heap,int n,int *count_ptr){
    // function will re-arrange the elements in the heap in order to generate a max heap
    int key,i,j;
    for(int k=1;k<=n;++k){
        i=k;
        key = heap->arr[i];        
        j=(i-1)/2;

        while(++(*count_ptr) && (i>0) && (key>heap->arr[j])){
            heap->arr[i] = heap->arr[j];
            i=j;
            j = (i-1)/2;
        }
        heap->arr[i] = key;
    }    
}

// Initialise heap
// Set heap size to 0
// Set heap max_size to paramter max_size
// Allocate memory of max_size to the array
void init_heap(heap_t *heap, int max_size) {
    heap->size = 0;
    heap->max_size = max_size;
    heap->arr = (int *)malloc(sizeof(int)*max_size);
}

// Insert element "key" to heap "heap"
// and store the number of key comparisons made in the
// location pointed to by count_ptr.
void insert(heap_t *heap, int key, int *count_ptr) {
    heap->arr[heap->size] = key;
    ++heap->size;
    max_heapify(heap,heap->size-1,count_ptr);
}

// *Removes and Returns* the maximum element in the heap
// and store the number of key comparisons made in the
// location pointed to by count_ptr.
int extract_max(heap_t *heap, int *count_ptr) {
    int m = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size-1];
    heap->size--;
    max_heapify(heap,heap->size-1,count_ptr);
    return m;
}

// Searches for the element key in the heap
// Returns the element if found, else -1
int search(const heap_t *heap, int key, int *count_ptr) {
    for(int i=0;i<heap->size;i++){
        if(++(*count_ptr) && key == heap->arr[i])
            return key;
    }
    return -1;
}

// Returns the maximum value of the element in the heap
// and store the number of key comparisons made in the
// location pointed to by count_ptr.
int find_max(const heap_t *heap, int *count_ptr) {
    return heap->arr[0];
    //no comparison in this case so the count_ptr will remain 0
}

// Returns the minimum value in the heap
int find_min(const heap_t *heap, int *count_ptr) {
    int min = heap->arr[heap->size/2];
    for(int i=1+heap->size/2; i < heap->size; i++){
        if(++(*count_ptr) && min>heap->arr[i])
            min = heap->arr[i];
    }
    return min;
}

// Clears the heap for reuse
void clear_heap(heap_t *heap) {
    heap->size = 0;
    free(heap->arr);
    heap->arr = (int *)malloc(sizeof(int)*heap->max_size);
}

// Frees all resources acquired to initialize heap
void free_heap(heap_t *heap) {
    heap->size = heap->max_size = 0;
    free(heap->arr);
}
