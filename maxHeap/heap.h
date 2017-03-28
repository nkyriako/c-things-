/* 
 * File:   heap.h
 * Author: Nicole
 *
 * Created on January 23, 2017, 8:37 PM
 */

#ifndef HEAP_H
#define HEAP_H

class heap {

private:
    int  Nel;
    int  MaxSize;
    int * array; 
    int * box;

    void adjust (int a[], int i, int n);
    friend std::ostream& operator<<(std::ostream& out, const heap& heap2);
    friend void operator+=(heap& heap1, const int i);
    friend void operator+=(heap& heap1, const heap heap2);
    
public:
    explicit heap(int MSize);                  //Constructor
    heap(const heap& orig);                    //Copy Constructor
    virtual ~heap();                           //Destructor
    bool insert (int item);
    bool delMax(int &item);
    void up(int index);                        //shifts up for inserts
    
    
    //Operators
    heap operator+(const heap &heap2);
    heap operator+(int j);
    int operator[](int index);
    heap& operator =(const heap& heap2);

    
};

#endif /* HEAP_H */

