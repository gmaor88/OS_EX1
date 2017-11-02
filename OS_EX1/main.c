#define _CRT_SECURE_NO_WARNINGS

/**************************************************/
///					Include						///
/**************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/***************************************************/

/**************************************************/
///					Define						///
/**************************************************/
#define TRUE									1
#define FALSE									0
#define NUM_OF_VERTICES_IN_TRIANGLE				3
#define NUM_OF_VERTICES_IN_QUAD					4
#define INPUT_STRING_LEN						16

/***************************************************/

/**************************************************/
///				MASKS And Shift					///
/**************************************************/
#define NEW_POLIGON_MASK						0x2
#define POLYGON_TYPE_MASK						0x4
#define ACTIONS_MASK							0x38
#define POLYGONS_TO_DO_ACTIONS_ON_MASK			0xc0   
#define COORDINATES_MASK						0xFFF
#define COORDINATES_VALUE_MASK					0x3f
#define SIGN_BIT_MASK							0x20
#define	SHIFT_TO_FIRST_VERTEX					8
#define SHIFT_TO_SECOND_VERTEX					20
#define SHIFT_TO_THIRD_VERTEX					32
#define SHIFT_TO_FOURTH_VERTEX					44
#define SHIFT_TO_Y_VALUE						6

/***************************************************/

/**************************************************/
///				Typedef And Structs				///
/**************************************************/
typedef struct _vertex {
	int x, y;
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

typedef void(*func_ptr)(long long unsigned);

/***************************************************/

/**************************************************/
///					Prototyps					///
/**************************************************/
void analyze_and_exec(long long unsigned poligon);
long long unsigned get_poligon_from_user();
void populate_functions_array();
void init_poligon_list();
int is_end_of_input(long long unsigned poligon);
void free_poligon_list(ListNode* currentNode);


void add_polygon(long long unsigned poligon);
void perimeter(long long unsigned poligon);
void area(long long unsigned poligon);
void print_polygon(long long unsigned poligon);
void do_current(long long unsigned poligon);
void do_all(long long unsigned poligon);

/***************************************************/

/**************************************************/
///						Globals					///
/**************************************************/
func_ptr functions_array[6];
List* poligon_list;

/***************************************************/

/**************************************************/
///						Main					///
/**************************************************/
void main() {
	long long unsigned poligon = 0;

	populate_functions_array();
	init_poligon_list();

	while (TRUE)
	{
		poligon = get_poligon_from_user();
		analyze_and_exec(poligon);
		if (is_end_of_input(poligon)) {
			break;
		}
	}

	free_poligon_list(poligon_list->head);
}
/***************************************************/


/**************************************************/
///				Implementations					///
/**************************************************/
void analyze_and_exec(long long unsigned poligon) {
	if (poligon & NEW_POLIGON_MASK) {
		functions_array[0](poligon);
	}



}

long long unsigned get_poligon_from_user() {
	long long unsigned poligon = 0;

	scanf("%llx", &poligon);

	return poligon;
}

void populate_functions_array() {
	functions_array[0] = add_polygon;
	functions_array[1] = perimeter;
	functions_array[2] = area;
	functions_array[3] = print_polygon;
	functions_array[4] = do_current;
	functions_array[5] = do_all;
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
	ListNode *node = (ListNode*)malloc(sizeof(ListNode));
	//init mode
	node->polygon = poligon;
	node->next = NULL;
	//add to list and set the list new tail
	poligon_list->tail->next = node;
	poligon_list->tail = node;
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

/***************************************************/