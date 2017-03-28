
/* 
 * File:   heap.cpp
 * Author: Nicole Kyriakopoulos
 * 
 * Created on January 23, 2017, 8:37 PM
 */
#include <cstdlib>
#include <iostream>     /* calloc, exit, free */
    
#include "heap.h"

//constructor
heap::heap(int MSize) 
  //  :MaxSize(MSize)
{
	if (MSize<1) {
		throw(-1);
	}
    //array holds the heap elements
    array = (int *) calloc(MSize+1, sizeof(int));
    //box helps access kth element of heap in linear time
    box = (int* ) calloc(MSize+1, sizeof (int));

    MaxSize = MSize;

    Nel=0;
}

//copy constructor
heap::heap(const heap& orig)
    :Nel(orig.Nel), MaxSize(orig.MaxSize), 
     array((int *) calloc(MaxSize+1, sizeof(int))), 
     box((int *) calloc(MaxSize+1, sizeof(int)))
{
 //copies array to new array
    std::copy(orig.array, orig.array+MaxSize, array);
}


//destructor
heap::~heap() {
    free(array);
    free(box);
}

//Does heapify after a max deletion.
void heap::adjust(int a[], int i, int n) {
    int j = 2*1, item = a[i];
    
    while (j<=n) {
        if (j<n && (a[j]<a[j+1])) j++;
        
        if (item >= a[j] ) {
            break;
        }
        a[j/2] = a[j];
        j*=2;
    }
    
    a[j/2] = item;
    
}

//Inserts an item into heap while maintaining heap properties
bool heap::insert(int item){
    int i = ++Nel;
    if (i == MaxSize) {
        std::cout << "heap size exceeded" << std::endl;
        return false;
    }
    
    array[i] = item;
    up(Nel);
    return true;
    
}
//shifts up for inserts
void heap::up(int index) {
    int parent;
    int temp;
    if (index > 1) {
        parent = index/2;
        if (array[parent] < array[index]) {
            temp = array[parent];
            array[parent] = array[index];
            array[index] = temp;
            up(parent);
        }
    }
}

//Deletes maximum (first member) if heap then adjusts the heap to be balanced
bool heap::delMax(int& item) {
    if(!Nel) {
        std::cout << "Heap is empty" << std::endl;
        return false;
    }
    item = array[1];
    array [1] = array[Nel--];
    adjust(array, 1, Nel);
    return true;
}

//Overload << to print heap on cout
std::ostream& operator <<(std::ostream& out, const heap& heap2) {
    out << "Heap Members: {";
    //prints each element separately 
    for(int i=1; i<(heap2.Nel)+1; i++){
        out << heap2.array[i] ; 
        if(i<heap2.Nel) {
            out << ", ";
        }
    }
    out << "}" << std::endl;
    return out;
}

//allow for adding of two heaps (e.g., heap3 = heap1 + heap2
//results in heap3 contacting the element of heap1 and heap2)

heap heap::operator +( const heap& heap2){

    int newMaxSize = MaxSize + heap2.MaxSize;
    //Initialize a heap that will contain the sum
    heap heap3(newMaxSize);
    
    //add heap 1 into heap 3 
    for (int i=1; i<Nel+1; i++) {
        heap3.insert(array[i]);
    }
    //add heap 2 into heap 3
    for (int i=1; i<heap2.Nel+1; i++) {
        heap3.insert(heap2.array[i]);
    }
    return heap3;
}

//Allows for adding another int to a heap
heap heap::operator +(int j) {
//Handles cases where the heap is full
    heap Sum(MaxSize+1);
    for (int i=1; i<Nel+1; i++) {
        Sum.insert(array[i]);
    }
    Sum.insert(j);
    return Sum;
}

//Overloads += for case: heap1 = heap1 + int
void operator +=(heap& heap1, const int i){
    heap1 = (heap1 + i);
}

//Overloads += for case: heap1 = heap1 + heap2 
void operator +=(heap& heap1, const heap heap2) {
    
     heap New = (heap1 + heap2);
     heap1 = New;
}

//Indexes kth largest element in heap

int heap::operator [](int index) {
    
    if(index> Nel || index<1) {
        std::cout << "Out of Bounds access for index" << std::endl;
        throw(-1);
        return(-1);
    }
   
    //Pop the first k elements out of the heap
    for (int i=1; i<index+1; i++){
       box[i] = array[1];
       delMax(*array);
    }
    
    //put them back in and return the kth element of Box aka 
    //kth largest element of array
    
    for (int i=1; i<index+1; i++){
        insert(box[i]);
    }
    
    return (box[index]);        //returns the ith highest number 
}


//Assignment operator
// heap1 = heap2 , or heap1.operator=(heap2)
heap& heap::operator =(const heap& heap2) {

//free previous heap
        free(array);
        free(box);

//allocate memory for next heap
        array = (int* )calloc((heap2.MaxSize)+1, sizeof(int));
        box = (int*)calloc((heap2.MaxSize)+1, sizeof(int));
        
        std::copy(heap2.array, heap2.array+MaxSize, array);
	Nel= heap2.Nel;
        MaxSize = heap2.MaxSize;
       return *this;
}

