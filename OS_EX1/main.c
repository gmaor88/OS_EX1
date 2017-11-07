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
#define PRINT_ACTION_REQUESTED					8
#define PERIMITER_ACTION_REQUESTED				16
#define AREA_ACTION_REQUESTED					32
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
#define SHIFT_TO_GET_0_TO_4_VALUE				6

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

int get_on_who_to_preform(long long unsigned poligon);
int populate_actions_to_preform_array(int* actions_to_preform, long long unsigned poligon);
void print_all_vertices(long long unsigned poligon, int is_quad);
void print_triangle_area(long long unsigned poligon);
void print_triangle_perimeter(long long unsigned poligon);
double calc_distance(Vertex vertex1, Vertex vertex2);
void print_quad_area(long long unsigned poligon);
void print_quad_perimeter(long long unsigned poligon);

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
	int actions_to_preform[3], actions_to_preform_counter, preform_for = -1;
	
	if (poligon & NEW_POLIGON_MASK) {
		functions_array[ADD_POLIGON](poligon);
	}

	//testing funcs (print, prim, area)
	/*functions_array[PRINT_POLIGON](poligon);
	//functions_array[PERIMETER](poligon);
	//functions_array[AREA](poligon);
	*/

	actions_to_preform_counter= populate_actions_to_preform_array(actions_to_preform, poligon);
	preform_for = get_on_who_to_preform(poligon);
	//printf("%d, %d\r\n", preform_for, actions_to_preform_counter);
	
	/*switch (preform_for)
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



int get_on_who_to_preform(long long unsigned poligon) {
	int choice = POLYGONS_TO_DO_ACTIONS_ON_MASK;

	choice = choice & poligon;
	choice = choice >> SHIFT_TO_GET_0_TO_4_VALUE;

	return choice;
}

int populate_actions_to_preform_array(int* actions_to_preform, long long unsigned poligon) {
	int counter = 0, choice = ACTIONS_MASK;

	actions_to_preform[0] = FALSE;
	actions_to_preform[1] = FALSE;
	actions_to_preform[2] = FALSE;

	choice = choice & poligon;
	if (choice & PRINT_ACTION_REQUESTED) {
		actions_to_preform[0] = TRUE;
		counter++;
	}
	
	if (choice & PERIMITER_ACTION_REQUESTED) {
		actions_to_preform[1] = TRUE;
		counter++;
	}
	
	if (choice & AREA_ACTION_REQUESTED) {
		actions_to_preform[2] = TRUE;
		counter++;
	}

	return counter;
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
		long long unsigned vertex = COORDINATES_MASK;
		short x = 0, y = 0;

		vertex = poligon & vertex;
		x = vertex & COORDINATES_VALUE_MASK;
		vertex = vertex >> SHIFT_TO_Y_VALUE;
		y = vertex & COORDINATES_VALUE_MASK;
		printf(" {%d, %d}", (int)x, (int)y);
		poligon = poligon >> SHIFT_TO_NEXT_VERTEX;
	}
}

void print_triangle_area(long long unsigned poligon) {
	Triangle triangle;
	double culc_area;

	poligon = poligon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_TRIANGLE; i++)
	{
		long long unsigned  vertex = COORDINATES_MASK;
		short x = 0, y = 0;

		vertex = poligon & vertex;
		x = vertex & COORDINATES_VALUE_MASK;
		vertex = vertex >> SHIFT_TO_Y_VALUE;
		y = vertex & COORDINATES_VALUE_MASK;

		triangle.vertices[i].x = (int)x;
		triangle.vertices[i].y = (int)y;
		poligon = poligon >> SHIFT_TO_NEXT_VERTEX;
	}

	// area = 0.5 * |x_0y_1 + x_1y_2 + x_2y_0 - x_1y_0 - x_2y_1 - x_0y_2|
	culc_area = 0.5 * fabs(
		triangle.vertices[0].x * triangle.vertices[1].y +
		triangle.vertices[1].x * triangle.vertices[2].y +
		triangle.vertices[2].x * triangle.vertices[0].y -
		triangle.vertices[1].x * triangle.vertices[0].y -
		triangle.vertices[2].x * triangle.vertices[1].y -
		triangle.vertices[0].x * triangle.vertices[2].y);

	printf("%.1f", culc_area);
}

void print_triangle_perimeter(long long unsigned poligon) {
	Triangle triangle;
	double culc_perimeter, distance1, distance2, distance3;

	poligon = poligon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_TRIANGLE; i++)
	{
		long long unsigned  vertex = COORDINATES_MASK;
		short x = 0, y = 0;

		vertex = poligon & vertex;
		x = vertex & COORDINATES_VALUE_MASK;
		vertex = vertex >> SHIFT_TO_Y_VALUE;
		y = vertex & COORDINATES_VALUE_MASK;

		triangle.vertices[i].x = (int)x;
		triangle.vertices[i].y = (int)y;
		poligon = poligon >> SHIFT_TO_NEXT_VERTEX;
	}

	distance1 = calc_distance(triangle.vertices[0], triangle.vertices[1]);
	distance2 = calc_distance(triangle.vertices[1], triangle.vertices[2]);
	distance3 = calc_distance(triangle.vertices[2], triangle.vertices[0]);
	culc_perimeter = distance1 + distance2 + distance3;

	printf("%.1f", culc_perimeter);
}

double calc_distance(Vertex vertex1, Vertex vertex2)
{
	double distance;
	return distance = sqrt(pow(vertex1.x - vertex2.x, 2) + pow(vertex1.y - vertex2.y, 2));
}

void print_quad_area(long long unsigned poligon) {
	Quad quad;
	double culc_area, distance1, distance2;

	poligon = poligon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_QUAD; i++)
	{
		long long unsigned vertex = COORDINATES_MASK;
		short x = 0, y = 0;

		vertex = poligon & vertex;
		x = vertex & COORDINATES_VALUE_MASK;
		vertex = vertex >> SHIFT_TO_Y_VALUE;
		y = vertex & COORDINATES_VALUE_MASK;
		quad.vertices[i].x = (int)x;
		quad.vertices[i].y = (int)y;
		poligon = poligon >> SHIFT_TO_NEXT_VERTEX;
	}

	distance1 = calc_distance(quad.vertices[0], quad.vertices[1]);
	distance2 = calc_distance(quad.vertices[0], quad.vertices[3]);
	culc_area = distance1 * distance2;

	printf("%.1f", culc_area);
}

void print_quad_perimeter(long long unsigned poligon) {
	Quad quad;
	double culc_perimeter, distance1, distance2;

	poligon = poligon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_QUAD; i++)
	{
		long long unsigned  vertex = COORDINATES_MASK;
		short x = 0, y = 0;

		vertex = poligon & vertex;
		x = vertex & COORDINATES_VALUE_MASK;
		vertex = vertex >> SHIFT_TO_Y_VALUE;
		y = vertex & COORDINATES_VALUE_MASK;

		quad.vertices[i].x = (int)x;
		quad.vertices[i].y = (int)y;
		poligon = poligon >> SHIFT_TO_NEXT_VERTEX;
	}

	distance1 = calc_distance(quad.vertices[0], quad.vertices[1]);
	distance2 = calc_distance(quad.vertices[0], quad.vertices[3]);
	culc_perimeter = (distance1 * 2)+ (distance2 * 2);

	printf("%.1f", culc_perimeter);
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
	if (poligon & NEW_POLIGON_MASK == 0) {
		return;
	}

	printf(" perimeter = ");
	if (poligon & POLYGON_TYPE_MASK) {
		print_quad_perimeter(poligon);
	}
	else
	{
		print_triangle_perimeter(poligon);
	}

	printf("%s", "\r\n");
}

/* calculate and print the area */
void area(long long unsigned poligon) {
	if (poligon & NEW_POLIGON_MASK == 0) {
		return;
	}

	printf(" area = ");
	if (poligon & POLYGON_TYPE_MASK) {
		print_quad_area(poligon);
	}
	else
	{
		print_triangle_area(poligon);
	}

	printf("%s", "\r\n");
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