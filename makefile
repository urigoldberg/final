CC = gcc
CPP = g++
#put all your object files here
OBJS = main.o main_aux.o SPImageProc.o SPPoint.o SPBPriorityQueue.o KdTree.o SPConfig.o SPLogger.o
#The executabel filename
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o:  main.cpp main_aux.h SPPoint.h SPBPriorityQueue.h SPLogger.h SPConfig.h KdTree.h SPImageProc.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
main_aux.o: main_aux.h main_aux.cpp main_aux.h SPPoint.h SPBPriorityQueue.h SPLogger.h SPConfig.h KdTree.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h KdTree.h SPPoint.h SPBPriorityQueue.h SPLogger.h main_aux.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPBPriorityQueue.o: SPBPriorityQueue.c SPPoint.h SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c
KdTree.o: KdTree.c KdTree.h SPPoint.h SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o:  SPConfig.c SPConfig.h KdTree.h SPPoint.h SPBPriorityQueue.h SPLogger.h main_aux.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h main_aux.h SPPoint.h SPBPriorityQueue.h SPConfig.h KdTree.h
	$(CC) $(C_COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
