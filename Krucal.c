#define  _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include<limits.h>


#define MAX_VERTICES_IN_GRAPH 5000
#define MAX_EDGE_IN_GRAPH(n) (n*(n+1)/2)

 
struct Edge {
	short first_v, second_v;
	unsigned weigth;
};

short* nodes, * ranks;
void swap(struct Edge* a1, struct Edge* a2);
void  sift_down(struct Edge* enges, int n, int i);
void  heap_sort(struct Edge* arr, int n);
void make_set(int x);
int find_set(int z);
int  union_sets(int x, int y);
int is_correct_value(unsigned value, unsigned min, unsigned max);
int is_correct_input_edge(short vertices, short first, short second, unsigned weigth);

void swap(struct Edge* a1, struct Edge* a2) {
	struct Edge temp = *a1;
	*a1 = *a2;
	*a2 = temp;
}

void  sift_down(struct Edge* enges, int n, int i) {
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < n && enges[l].weigth < enges[largest].weigth)
		largest = l;
	if (r < n && enges[r].weigth < enges[largest].weigth)
		largest = r;

	if (largest != i) {
		swap(&enges[i], &enges[largest]);
		sift_down(enges, n, largest);
	}
}

void  heap_sort(struct Edge* arr, int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		sift_down(arr, n, i);

	while (n > 0) {
		swap(&arr[0], &arr[n - 1]);
		n--;
		sift_down(arr, n, 0);
	}
}

void make_set(int x){
	nodes[x] = x;
	ranks[x] = 0;
}

int find_set(int z){
	if (z == nodes[z])
		return z;
	else
		return nodes[z] = find_set(nodes[z]);
}

int  union_sets(int x, int y){
	x = find_set(x);
	y = find_set(y);
	
	if (x == y)
		return 0;

	if (ranks[x] < ranks[y])
		nodes[x] = y;
	else {
		nodes[y] = x;
		if (ranks[x] == ranks[y])
			++ranks[x];
	}
	return 1;
}

int is_correct_value(unsigned value, unsigned min, unsigned max) {
	if (min <= value && value <= max)
		return 0;
	else
		return -1;
}

int is_correct_input_edge(short vertices, short first, short second, unsigned weigth) {
	if (is_correct_value(first, 1, vertices) == -1 || is_correct_value(second, 1, vertices) == -1) {
		printf("bad vertex");
		return -1;
	}
	else if (is_correct_value(weigth, 0, INT_MAX) == -1) {
		printf("bad length");
		return -1;
	}
	return 0;
}


int main() {
	int vertices, enges = 0;
	struct Edge* arr_edges;


	if (!scanf("%d", &vertices)
		|| is_correct_value(vertices, 0, MAX_VERTICES_IN_GRAPH)) {
		printf("bad number of vertices");
		return 0;
	}

	if (!scanf("%d", &enges)
		|| is_correct_value(enges, 0, MAX_EDGE_IN_GRAPH(vertices))) {
		printf("bad number of edges");
		return 0;
	}

	if (vertices == 0) {
		printf("no spanning tree");
		return 0;
	}
	if (enges == 0 && vertices == 1) {
		return 0;
	}

	arr_edges = calloc((size_t)enges, sizeof(struct Edge));
	if (arr_edges == NULL)
		return 0;

	for (int i = 0; i < enges; i++) {
		int  second_vertice, first_vertice, weigth = 0;
		if (fscanf(stdin, "%d %d %d", &first_vertice, &second_vertice, &weigth) == EOF) {
			printf("bad number of lines");
			free(arr_edges);
			return 0;
		}
		if (is_correct_input_edge(vertices, first_vertice, second_vertice, weigth) == -1) {
			free(arr_edges);
			return 0;
		}

		struct Edge new_edge = { .first_v = first_vertice - 1 ,.second_v = second_vertice - 1,.weigth = weigth };
		arr_edges[i] = new_edge;
	}

	nodes = calloc((size_t)vertices, sizeof(short));
	ranks = calloc((size_t)vertices, sizeof(short));
	for (int i = 0; i < vertices; i++) make_set(i);

	heap_sort(arr_edges, enges);

	int* result = calloc(vertices, sizeof(int));
	char* visited = calloc(vertices, sizeof(char));

	short temp = 0;
	short count = 0;

	for (long  i = enges - 1; i >= 0; i--) {
		if (union_sets(arr_edges[i].first_v, arr_edges[i].second_v) != 0) {

			if (visited[arr_edges[i].first_v] == 0)
				temp++;
			visited[arr_edges[i].first_v] = 1;
			if (visited[arr_edges[i].second_v] == 0)
				temp++;
			visited[arr_edges[i].second_v] = 1;
		 

			result[count++] = i;
		}
	}
	if (temp != vertices){
		free(visited);
		free(arr_edges);
		free(nodes);
		free(ranks);
		free(result);
		printf("no spanning tree");
		return 0;
	}
	for (int j = 0; j < count; j++)
		printf("%d %d\n", arr_edges[result[j]].first_v + 1, arr_edges[result[j]].second_v + 1);

	free(arr_edges);
	free(nodes);
	free(ranks);
	free(visited);
	free(result);

	return 0;
}
