OBJS = main3.o heap.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -Wall -c $(DEBUG)
LFLAGS = -std=c++11 -Wall $(DEBUG)


#put rest of main functions in all to compile everything
all:main3

main3 : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main3

heap.o : heap.h heap.cpp 
	$(CC) $(CFLAGS) heap.cpp

main3.o : 
	$(CC) $(CFLAGS) main3.cpp



clean:
	rm -f *.o main3


tar:
	tar cfv main3.tar main3.h main3.cpp heap.h heap.cpp 

program3:
	main3

	
