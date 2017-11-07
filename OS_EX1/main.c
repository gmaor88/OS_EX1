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
#define ADD_POLIGON								0
#define PERIMETER								1
#define AREA									2
#define PRINT_POLIGON							3
#define DO_CURRENT								4
#define	DO_ALL									5

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
#define SHIFT_TO_NEXT_VERTEX					12
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

void print_all_vertices(long long unsigned poligon, int is_quad);

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
	int actions_to_preform[3];
	int preform_for = -1;
	
	if (poligon & NEW_POLIGON_MASK) {
		functions_array[ADD_POLIGON](poligon);
	}

	//testing the print func
	functions_array[PRINT_POLIGON](poligon);

	/*populate_actions_to_preform_array(actions_to_preform, poligon);
	preform_for = get_on_who_to_preform(poligon);
	switch (preform_for)
	{
	case 0:
		functions_array[4](poligon);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		functions_array[6](poligon);
		break;
	default:
		break;
	}*/

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
		return TRUE;
	}

	return FALSE;
}

void free_poligon_list(ListNode* currentNode) {
	if (currentNode->next) {
		free_poligon_list(currentNode->next);
	}
	
	free(currentNode);
}


void print_all_vertices(long long unsigned poligon, int is_quad) {
	int num_of_vertices = NUM_OF_VERTICES_IN_TRIANGLE;
	
	if (poligon & NEW_POLIGON_MASK == 0) {
		return;
	}
	
	if (is_quad) {
		num_of_vertices = NUM_OF_VERTICES_IN_QUAD;
	}

	poligon = poligon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < num_of_vertices; i++)
	{
		long long unsigned  vertex = COORDINATES_MASK;
		short x = 0, y = 0;
		//vertex = poligon | vertex;
		//vertex = vertex & COORDINATES_MASK;
		vertex = poligon & vertex;
		x = vertex & COORDINATES_VALUE_MASK;
		vertex = vertex >> SHIFT_TO_Y_VALUE;
		y = vertex & COORDINATES_VALUE_MASK;

		printf(" {%d, %d}", (int)x, (int)y);
		poligon = poligon >> SHIFT_TO_NEXT_VERTEX;
	}
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
	int is_quad = FALSE;
	
	if (poligon & POLYGON_TYPE_MASK) {
		is_quad = TRUE;
		printf("%s", "square");
	}
	else
	{
		printf("%s", "triangle");
	}

	print_all_vertices(poligon, is_quad);
	printf("%s", "\r\n");
}

/* do the operations on the current polygon */
void do_current(long long unsigned poligon) {

}

/* do the operations in the parameter on the list */
void do_all(long long unsigned poligon) {

}

/***************************************************/