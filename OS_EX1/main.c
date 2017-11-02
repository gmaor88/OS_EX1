#include <stdlib.h>
#include <stdio.h>

#define NUM_OF_VERTICES_IN_TRIANGLE 3
#define NUM_OF_VERTICES_IN_QUAD 4
#define INPUT_STRING_LEN 16
#define True 1
#define False 0

typedef struct _vertex {
	float x, y;
} Vertex;

typedef struct _triangle {
	struct _vertex vertices[NUM_OF_VERTICES_IN_TRIANGLE];
} Triangle;

typedef struct _quad {
	struct _vertex vertices[NUM_OF_VERTICES_IN_QUAD];
} Quad;

typedef struct listNode {
	long long unsigned polygon;
	struct listNode* next;
}ListNode;

typedef struct list {
	ListNode* head;
	ListNode* tail;
}List;


void analize_and_exec(long long unsigned poligon);
long long unsigned get_poligon_from_user();
void populate_function_array();
void init_poligon_list();
void update_continue_flag(long long unsigned poligon);
void free_poligon_list(ListNode* currentNode);


void add_polygon(long long unsigned poligon);
void perimeter(long long unsigned poligon);
void area(long long unsigned poligon);
void print_polygon(long long unsigned poligon);
void do_current(long long unsigned poligon);
void do_all(long long unsigned poligon);


void(*function_array[6]);
List* poligon_list;

void main() {
	long long unsigned poligon;
	int continue_flag = 1;

	populate_function_array();
	init_poligon_list();

	while (continue_flag)
	{
		poligon = get_poligon_from_user();
		analize_and_exec(poligon);
		
		update_continue_flag(poligon);
	}

	free_poligon_list(poligon_list->head);
}


void analize_and_exec(long long unsigned poligon) {

}

long long unsigned get_poligon_from_user() {
	char input_string[INPUT_STRING_LEN];
	long long unsigned poligon = 0;

	for (size_t i = 0; i < INPUT_STRING_LEN; i++)
	{
		scanf("%s", input_string[i]);
	}

	sscanf(input_string, "%x", &poligon);

	return poligon;
}

void populate_function_array() {
	function_array[0] = add_polygon;
	function_array[1] = perimeter;
	function_array[2] = area;
	function_array[3] = print_polygon;
	function_array[4] = do_current;
	function_array[5] = do_all;
}

void init_poligon_list() {
	poligon_list = (List*)malloc(sizeof(List));
	ListNode* dummy = (ListNode*)malloc(sizeof(ListNode));
	
	poligon_list->head = poligon_list->tail = dummy;
}

void update_continue_flag(long long unsigned poligon) {
	long long unsigned mask = 1;

	if (poligon & mask) {
		return True;
	}

	return False;
}

void free_poligon_list(ListNode* currentNode) {
	if (currentNode->next) {
		free_poligon_list(currentNode->next);
	}
	
	free(currentNode);
}


/* add new polygon to the list*/
void add_polygon(long long unsigned poligon) {

}

/* calculate and print the perimeter */
void perimeter(long long unsigned poligon) {

}

/* calculate and print the area */
void area(long long unsigned poligon) {

}

/* print the type of polygon and its vertices */
void print_polygon(long long unsigned poligon) {

}

/* do the operations on the current polygon */
void do_current(long long unsigned poligon) {

}

/* do the operations in the parameter on the list */
void do_all(long long unsigned poligon) {

}
