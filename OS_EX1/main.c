#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#define NUM_OF_VERTICES_IN_TRIANGLE 3
#define NUM_OF_VERTICES_IN_QUAD 4
#define INPUT_STRING_LEN 16
#define True 1
#define False 0
#define NEW_POLIGON_MASK 0x2

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


void analyze_and_exec(long long unsigned poligon);
long long unsigned get_poligon_from_user();
void populate_function_array();
void init_poligon_list();
int is_end_of_input(long long unsigned poligon);
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
	long long unsigned poligon = 0;

	populate_function_array();
	init_poligon_list();

	while (True)
	{
		poligon = get_poligon_from_user();
		analyze_and_exec(poligon);
		if (is_end_of_input(poligon)) {
			break;
		}
	}

	free_poligon_list(poligon_list->head);
}


void analyze_and_exec(long long unsigned poligon) {
	if (poligon & NEW_POLIGON_MASK) {
		add_polygon(poligon);
	}


}

long long unsigned get_poligon_from_user() {
	long long unsigned poligon = 0;

	scanf("%llx", &poligon);

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
	dummy->polygon = 0;
	dummy->next = NULL;
	
	poligon_list->head = poligon_list->tail = dummy;
}

int is_end_of_input(long long unsigned poligon) {
	long long unsigned mask = 1;

	if (poligon & mask) {
		return False;
	}

	return True;
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
