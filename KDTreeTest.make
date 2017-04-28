CC = gcc
OBJS = kd_tree_unit_test.o KdTree.o
EXEC = kd_tree_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

	
kd_tree_unit_test.o: $(TESTS_DIR)/kd_tree_unit_test.c $(TESTS_DIR)/unit_test_util.h KdTree.h SPPoint.h SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
KdTree.o: KdTree.c KdTree.h SPPoint.h SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
