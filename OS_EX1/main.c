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
#define ADD_POLYGON								0
#define PERIMETER								1
#define AREA									2
#define PRINT_POLYGON							3
#define DO_CURRENT								4
#define	DO_ALL									5
#define TRIANGLE								0
#define QUAD									4

/***************************************************/

/**************************************************/
///				MASKS And Shift					///
/**************************************************/
#define NEW_POLYGON_MASK						0x2
#define POLYGON_TYPE_MASK						0x4
#define ACTIONS_MASK							0x38
#define PRINT_ACTION_REQUESTED					8
#define PERIMITER_ACTION_REQUESTED				16
#define AREA_ACTION_REQUESTED					32
#define POLYGONS_TO_DO_ACTIONS_ON_MASK			0xc0   
#define COORDINATES_MASK						0xFFF
#define COORDINATES_VALUE_MASK					0x3f
#define SIGN_BIT_MASK							0x20
#define NEGATIVE_BIT_MASK						0xFFC0
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
void analyze_and_exec(long long unsigned polygon);
long long unsigned get_polygon_from_user();
void populate_functions_array();
void init_polygon_list();
int is_end_of_input(long long unsigned polygon);
void free_polygon_list(ListNode* currentNode);

int get_on_who_to_preform(long long unsigned polygon);
void populate_actions_to_preform_array(int* actions_to_preform, long long unsigned polygon);
void do_for_all_from_shape(long long unsigned polygon, int is_quad);
void print_all_vertices(long long unsigned polygon, int is_quad);
void print_triangle_area(long long unsigned polygon);
void print_triangle_perimeter(long long unsigned polygon);
double calc_distance(Vertex vertex1, Vertex vertex2);
void print_quad_area(long long unsigned polygon);
void print_quad_perimeter(long long unsigned polygon);
void handle_negatinve(int* n);
void do_actions_on_polygon(int* actions_to_preform, long long unsigned polygon);
void get_vertex(long long unsigned polygon, short* x, short*y);

void add_polygon(long long unsigned polygon);
void perimeter(long long unsigned polygon);
void area(long long unsigned polygon);
void print_polygon(long long unsigned polygon);
void do_current(long long unsigned polygon);
void do_all(long long unsigned polygon);

/***************************************************/

/**************************************************/
///						Globals					///
/**************************************************/
func_ptr functions_array[6];
List* polygon_list;

/***************************************************/

/**************************************************/
///						Main					///
/**************************************************/
void main() {
	long long unsigned polygon = 0;

	populate_functions_array();
	init_polygon_list();

	while (TRUE)
	{
		polygon = get_polygon_from_user();
		analyze_and_exec(polygon);
		if (is_end_of_input(polygon)) {
			break;
		}
	}

	free_polygon_list(polygon_list->head);
	free(polygon_list);
	printf("done");
}

/***************************************************/

/**************************************************/
///				Implementations					///
/**************************************************/
void analyze_and_exec(long long unsigned polygon) {
	int preform_for = -1;

	if (polygon & NEW_POLYGON_MASK) {
		functions_array[ADD_POLYGON](polygon);
	}

	preform_for = get_on_who_to_preform(polygon);
	switch (preform_for)
	{
	case 0:
		functions_array[DO_CURRENT](polygon);
		break;
	case 1:
		do_for_all_from_shape(polygon, TRIANGLE);
		break;
	case 2:
		do_for_all_from_shape(polygon, QUAD);
		break;
	case 3:
		functions_array[DO_ALL](polygon);
		break;
	default:
		break;
	}

}

long long unsigned get_polygon_from_user() {
	long long unsigned polygon = 0;

	scanf_s("%llx", &polygon);

	return polygon;
}

void populate_functions_array() {
	functions_array[0] = add_polygon;
	functions_array[1] = perimeter;
	functions_array[2] = area;
	functions_array[3] = print_polygon;
	functions_array[4] = do_current;
	functions_array[5] = do_all;
}

void init_polygon_list() {
	polygon_list = (List*)malloc(sizeof(List));
	ListNode* dummy = (ListNode*)malloc(sizeof(ListNode));
	dummy->polygon = 0;
	dummy->next = NULL;

	polygon_list->head = polygon_list->tail = dummy;
}

int is_end_of_input(long long unsigned polygon) {
	long long unsigned mask = 1;

	if (polygon & mask) {
		return TRUE;
	}

	return FALSE;
}

void free_polygon_list(ListNode* currentNode) {
	if (currentNode->next) {
		free_polygon_list(currentNode->next);
	}

	free(currentNode);
}


int get_on_who_to_preform(long long unsigned polygon) {
	int choice = POLYGONS_TO_DO_ACTIONS_ON_MASK;

	choice = choice & polygon;
	choice = choice >> SHIFT_TO_GET_0_TO_4_VALUE;

	return choice;
}

void populate_actions_to_preform_array(int* actions_to_preform, long long unsigned polygon) {
	int choice = ACTIONS_MASK;

	actions_to_preform[0] = FALSE;
	actions_to_preform[1] = FALSE;
	actions_to_preform[2] = FALSE;

	choice = choice & polygon;
	if (choice & PRINT_ACTION_REQUESTED) {
		actions_to_preform[0] = TRUE;
	}

	if (choice & PERIMITER_ACTION_REQUESTED) {
		actions_to_preform[1] = TRUE;
	}

	if (choice & AREA_ACTION_REQUESTED) {
		actions_to_preform[2] = TRUE;
	}
}

void do_for_all_from_shape(long long unsigned polygon, int is_quad) {
	int actions_to_preform[3];
	ListNode* curr = polygon_list->head->next;

	if (!curr) {
		return; //empty list
	}

	populate_actions_to_preform_array(actions_to_preform, polygon);
	while (curr)
	{
		int type = curr->polygon & POLYGON_TYPE_MASK;
		if (type != is_quad) {
			curr = curr->next;
			continue;
		}

		do_actions_on_polygon(actions_to_preform, curr->polygon);
		curr = curr->next;
	}
}

void print_all_vertices(long long unsigned polygon, int is_quad) {
	int num_of_vertices = NUM_OF_VERTICES_IN_TRIANGLE;

	if (polygon & NEW_POLYGON_MASK == 0) {
		return;
	}

	if (is_quad) {
		num_of_vertices = NUM_OF_VERTICES_IN_QUAD;
	}

	polygon = polygon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < num_of_vertices; i++)
	{
		short x = 0, y = 0;

		get_vertex(polygon, &x, &y);
		printf(" {%d, %d}", (int)x, (int)y);
		polygon = polygon >> SHIFT_TO_NEXT_VERTEX;
	}
}

void print_triangle_area(long long unsigned polygon) {
	Triangle triangle;
	double culc_area;

	polygon = polygon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_TRIANGLE; i++)
	{
		short x = 0, y = 0;

		get_vertex(polygon, &x, &y);
		triangle.vertices[i].x = (int)x;
		triangle.vertices[i].y = (int)y;
		polygon = polygon >> SHIFT_TO_NEXT_VERTEX;
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

void print_triangle_perimeter(long long unsigned polygon) {
	Triangle triangle;
	double culc_perimeter, distance1, distance2, distance3;

	polygon = polygon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_TRIANGLE; i++)
	{
		short x = 0, y = 0;

		get_vertex(polygon, &x, &y);
		triangle.vertices[i].x = (int)x;
		triangle.vertices[i].y = (int)y;
		polygon = polygon >> SHIFT_TO_NEXT_VERTEX;
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

void print_quad_area(long long unsigned polygon) {
	Quad quad;
	double culc_area, distance1, distance2;

	polygon = polygon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_QUAD; i++)
	{
		short x = 0, y = 0;

		get_vertex(polygon, &x, &y);
		quad.vertices[i].x = (int)x;
		quad.vertices[i].y = (int)y;
		polygon = polygon >> SHIFT_TO_NEXT_VERTEX;
	}

	distance1 = calc_distance(quad.vertices[0], quad.vertices[1]);
	distance2 = calc_distance(quad.vertices[0], quad.vertices[3]);
	culc_area = distance1 * distance2;

	printf("%.1f", culc_area);
}

void print_quad_perimeter(long long unsigned polygon) {
	Quad quad;
	double culc_perimeter, distance1, distance2;

	polygon = polygon >> SHIFT_TO_FIRST_VERTEX;
	for (size_t i = 0; i < NUM_OF_VERTICES_IN_QUAD; i++)
	{
		short x = 0, y = 0;

		get_vertex(polygon, &x, &y);
		quad.vertices[i].x = (int)x;
		quad.vertices[i].y = (int)y;
		polygon = polygon >> SHIFT_TO_NEXT_VERTEX;
	}

	distance1 = calc_distance(quad.vertices[0], quad.vertices[1]);
	distance2 = calc_distance(quad.vertices[0], quad.vertices[3]);
	culc_perimeter = (distance1 * 2) + (distance2 * 2);

	printf("%.1f", culc_perimeter);
}

void handle_negatinve(int* n) {
	*n |= NEGATIVE_BIT_MASK;
	*n = ~(*n);
	*n += 1;
	*n *= -1;
}

void do_actions_on_polygon(int* actions_to_preform, long long unsigned polygon) {
	if (actions_to_preform[0]) {
		functions_array[PRINT_POLYGON](polygon);
	}

	if (actions_to_preform[1]) {
		functions_array[PERIMETER](polygon);
	}

	if (actions_to_preform[2]) {
		functions_array[AREA](polygon);
	}
}

void get_vertex(long long unsigned polygon, short* x, short*y) {
	long long unsigned vertex = COORDINATES_MASK;

	vertex = polygon & vertex;
	*x = vertex & COORDINATES_VALUE_MASK;
	if (*x & SIGN_BIT_MASK) {
		handle_negatinve(x);
	}

	vertex = vertex >> SHIFT_TO_Y_VALUE;
	*y = vertex & COORDINATES_VALUE_MASK;
	if (*y & SIGN_BIT_MASK) {
		handle_negatinve(y);
	}
}


/* add new polygon to the list*/
void add_polygon(long long unsigned polygon) {
	ListNode *node = (ListNode*)malloc(sizeof(ListNode));
	//init mode
	node->polygon = polygon;
	node->next = NULL;
	//add to list and set the list new tail
	polygon_list->tail->next = node;
	polygon_list->tail = node;
}

/* calculate and print the perimeter */
void perimeter(long long unsigned polygon) {
	if (polygon & NEW_POLYGON_MASK == 0) {
		return;
	}

	printf(" perimeter = ");
	if (polygon & POLYGON_TYPE_MASK) {
		print_quad_perimeter(polygon);
	}
	else
	{
		print_triangle_perimeter(polygon);
	}

	printf("%s", "\r\n");
}

/* calculate and print the area */
void area(long long unsigned polygon) {
	if (polygon & NEW_POLYGON_MASK == 0) {
		return;
	}

	printf(" area = ");
	if (polygon & POLYGON_TYPE_MASK) {
		print_quad_area(polygon);
	}
	else
	{
		print_triangle_area(polygon);
	}

	printf("%s", "\r\n");
}

/* print the type of polygon and its vertices */
void print_polygon(long long unsigned polygon) {
	int is_quad = FALSE;

	if (polygon & POLYGON_TYPE_MASK) {
		is_quad = TRUE;
		printf("%s", "square");
	}
	else
	{
		printf("%s", "triangle");
	}

	print_all_vertices(polygon, is_quad);
	printf("%s", "\r\n");
}

/* do the operations on the current polygon */
void do_current(long long unsigned polygon) {
	int actions_to_preform[3];

	if (polygon & NEW_POLYGON_MASK == 0) {
		return;
	}

	populate_actions_to_preform_array(actions_to_preform, polygon);
	do_actions_on_polygon(actions_to_preform, polygon);
}

/* do the operations in the parameter on the list */
void do_all(long long unsigned polygon) {
	int actions_to_preform[3];
	ListNode* curr = polygon_list->head->next;

	if (!curr) {
		return;
	}

	populate_actions_to_preform_array(actions_to_preform, polygon);
	while (curr)
	{
		do_actions_on_polygon(actions_to_preform, curr->polygon);
		curr = curr->next;
	}
}

/***************************************************/