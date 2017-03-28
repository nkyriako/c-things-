/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Nicole
 *
 * HW 2 Problem 3
 * Created on January 23, 2017, 8:33 PM
 */

#include <cstdlib>
#include <iostream>
#include "heap.h"

/*
 * 
 */
int main(int argc, char** argv) {

//Demonstrates basic constructor usage  (insert)
    std::cout<<"Creating array A, inserting 6 elements"<<std::endl;
    heap A(30);
    A.insert(10);
    A.insert(9);
    A.insert(8);
    A.insert(80);
    A.insert(81);
    A.insert(11);
    std::cout<<A;
    
    std::cout<<"Creating array B, inserting 1 element (30)"<<std::endl;
    heap B(5);
    B.insert(30);
    
 //Demonstrates delMax  
  
    
//Demonstrates implementation of copy constructor and overloaded 
// print operator << 
    std::cout<<"Copying array B, inserting second element to the copy"<<std::endl;
    
    heap Copy{B};
    Copy.insert(200);
    
    
     std::cout << "B: " <<std::endl;            
     std::cout<<B ;                           //Should output 30
     std::cout << "B's copy: " <<std::endl;     
     std::cout << Copy;                        //Should output 30 and 200
     
//Demonstrates + and = operator with 2 heaps
     heap J(5);
     J = Copy + B;
     std::cout<<"Heap J = Copy + B"<<std::endl;
     std::cout<<J;                             //Should output 200, 30, 30
     
//Demonstrates + operator with heap and int
     std::cout<<"generating a new heap consisting of Copy plus another int, 4"
             <<std::endl;
    heap NEW  = Copy + 4;
    std::cout<<NEW;
    std::cout<<"Now inserting 12, 79 into New heap"<<std::endl; 
    NEW.insert(12);
    NEW.insert(79);
    std::cout<<NEW;  //Should output 30 200, 12, and 4
   
     
//[] operator, indexing kth largest element.
    std::cout << "Heap B looks like this: " << std::endl;
    std::cout<<B;
     B.insert(499);
     B.insert(12);
     std::cout << "After inserting 2 more ints: " << std::endl;
     std::cout<<B;  //499, 12, 30
     std::cout << "B's 2nd largest number using []" << std::endl;
     std::cout << B[2] << std::endl;    //30
     std::cout << "B's 3rd largest number using []" << std::endl;
     std::cout << B[3] << std::endl;    //12
     
     
// += operator (heap+=int)
     B+=99;
     std::cout<<"Heap B after B+=99:"<<std::endl;
     std::cout<<B;
     
//+= operator (heap+=heap) 
     std::cout<<"A+=B "<< std::endl;
     A+=B;
     std::cout<<"A: "<<std::endl<<A;
    
//edge cases 
     try{
         //out of bounds
         std::cout<<B[6]<<std::endl;
     }catch(int i) {
         std::cout<<"Error! "<<std::endl;
     }

 //cant have a heap under size 1
    try{

	heap wrong(-3);
  	} catch(int i) {
		std::cout<<"Invalid heap size!"<<std::endl;
	}
      
    return 0;
}



