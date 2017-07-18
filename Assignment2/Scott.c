#include <stdio.h> 
#include <stdlib.h>

// maximum number of vertices
#define NMAX 2187
// maximum degree
#define DEG_MAX 16
#define DEBUG 1

int read_graph(int *n, int degree[NMAX], int G[NMAX][DEG_MAX]); 
void print_graph(int n, int degree[NMAX], int G[NMAX][DEG_MAX], int graph_num);
void min_dom_set(int level, int n, int degree[NMAX], int G[NMAX][DEG_MAX], int n_dominated, int num_dominated[NMAX],
				int num_choice[NMAX], int size, int dom[NMAX], int min_size, int min_dom[NMAX]);
void print_vector(int size, int arr[]);
void print_min_dom_set(int size, int n, int dom[]);

int main(int argc, char *argv[])
{
	int n; // number of vertices
	int degree[NMAX]; // degrees of vertices
	int G[NMAX][DEG_MAX]; // adjacency list
	int n_dominated; // number of dominated vertices
	int num_dominated[NMAX]; // number of times a vertex is dominated
	int num_choice[NMAX]; // number of times a vertex could be dominated if all white vertices were red
	int size; // number of vertices added to dominating set
	int dom[NMAX]; // vertices added to dominating set
	int min_size; // minimum number of vertices in dominating set
	int min_dom[NMAX]; // vertices added to minimum dominating set
	int verbose;
	int graph_num = 0;
	
	// Prints error message if incorrect usage
	if (argc != 2)
	{
		printf("Must specify verbosity level (0 for terse, 1 for verbose)\nExample: a.out 0 < in.txt > out.txt\n");
		return EXIT_FAILURE;
	}

    // Reads in the user-specified verbosity level
	verbose = atoi(argv[1]);
    
    while (read_graph(&n, degree, G) == 0)
    {
		// Increments the number of graph
		graph_num++;
        // If verbose output selected, prints graph
		if (verbose == 1)
		{
			print_graph(n, degree, G, graph_num);
			// find minimum dominating set
			min_dom_set(0, n, degree, G, n_dominated, num_dominated, num_choice, size, dom, min_size, min_dom);
			// print dominating set, don't forget fflush(stdout);
			//print_min_dom_set(min_size, n, min_dom);
		}
        else
        {
            // printf("%5d %3d %3d\n", graph_num, n, min_size);
        }
    }
	
	return EXIT_SUCCESS;
}

/*
 * Reads graph. 
 */
int read_graph(int *n, int degree[NMAX], int G[NMAX][DEG_MAX]) 
{
    // Reads in input as long as end of file is not reached
	if (fscanf(stdin, "%d", n) == 1)
	{
		int i, j;

		// Prints error message if invalid number of vertices
		// if (n < 1 || n > NMAX)
		// {
		// 	printf("Invalid input.\nThe number of vertices in a graph must be >= 1 and <= %d.\n", NMAX);
		// 	printf("Graph %d: BAD GRAPH\n", graph_num);
		// 	return EXIT_FAILURE;
		// }

		// For each vertex
		for (i = 0; i < *n; i++)
		{
			// Reads in its degree
			if (scanf("%d", &degree[i]) != 1)
			{
				return EXIT_FAILURE;
			}

			// For each degree
			for (j = 0; j < degree[i]; j++)
			{
				// Reads in the vertex's neighbours
				if (scanf("%d", &G[i][j]) != 1)
				{
					return EXIT_FAILURE;
				}
			}
		}
        return EXIT_SUCCESS;
// #if DEBUG
// 		printf("This is graph # %d \n", graph_num);
// 		printf("Number of vertices is %d \n", n);
// 		printf("Printing degree[]\n");
// 		for (i = 0; i < n; i++)
// 		{
// 			printf("%d ", degree[i]);
// 		}

// 		printf("Printing adjacency list[][]\n");
// 		for (i = 0; i < n; i++)
// 		{
// 			for (j = 0; j < n; j++)
// 			{
// 				printf("%d ", G[i][j]);
// 			}
// 			printf("\n");
// 		}
// #endif
	}
    return EXIT_FAILURE;
}

/*
 * Prints graph
 */
void print_graph(int n, int degree[NMAX], int G[NMAX][DEG_MAX], int graph_num)
{
	int i, j;

	printf("Graph \t%d:\n", graph_num);
	
	for (i = 0; i < n; i++)
	{
		printf("  %d( %d):", i, (degree[i]));
		for (j = 0; j < degree[i]; j++)
		{
			printf("   %d", G[i][j]);
		}
		printf("\n");
	}
}

void min_dom_set(int level, int n, int degree[NMAX], int G[NMAX][DEG_MAX], int n_dominated, int num_dominated[NMAX],
				int num_choice[NMAX], int size, int dom[NMAX], int min_size, int min_dom[NMAX])
{
#if DEBUG
	printf("\nLevel %3d: \n", level);
	printf("Current dominating set:\n");
	print_vector(size, dom);
	printf("Minimum dominating set:\n");
	print_vector(min_size, min_dom);
	printf("Number of vertices dominated: %3d\n", n_dominated);
	printf("Number of choices per vertex:\n");
	print_vector(n, num_choice);
	printf("Number of times dominated:\n");
	print_vector(n, num_dominated);
	printf("Size is: %d, min_size is: %d\n", size, min_size);
#endif

	int i, j;
	// initialize data structures
	if (level == 0)
	{
		n_dominated = 0;		
		size = 0;
		// no values assigned to dom[]
		min_size = n;
		for (i = 0; i < n; i++)
		{
			num_choice[i] = degree[i] + 1;
			num_dominated[i] = 0;
			min_dom[i] = i;
			G[i][degree[i]] = i;
		}
	}

	// backtrack
	for (i = 0; i < n; i++)
	{
		if (num_choice[i] == 0)
		{
			return;
		}
	}

	int u = n - n_dominated; // number of undominated vertices
	int delta = degree[0]; // maximum degree of a vertex

	for (i = 0; i < n; i++)
	{
		if (degree[i] > delta)
		{
			delta = degree[i];
		}
	}

	int n_extra = (u + delta - 1)/delta;
	
	if (size + n_extra >= min_size)
	{
		return;
	}

	if (level == n || n_dominated == n)
	{
		if (size < min_size)
		{
			for (i = 0; i < min_size; i++)
			{
				min_dom[i] = dom[i];
			}
			min_size = size;
			printf("size is %d\n", size);
			printf("min_size is %d\n", min_size);
		}
		return;
	}

	// exhaustive backtrack
	u = level;
	// try colouring a vertex blue
	for (i = 0; i <= degree[u]; i++)
	{
		num_choice[G[u][i]]--;
	}

	// recursive call
	min_dom_set(level + 1, n, degree, G, n_dominated, num_dominated, num_choice, size, dom, min_size, min_dom);

	// undo colouring a vertex blue
	for (i = 0; i <= degree[u]; i++)
	{
		num_choice[G[u][i]]++;
	}

	// try colouring a vertex red
	dom[u] = 1;
	size++;
	for (i = 0; i <= degree[u]; i++)
	{
		num_dominated[G[u][i]]++;
		n_dominated++;
	}
	printf("n_dominated after colouring a vertex red %d\n", n_dominated);
	
	// recursive call
	min_dom_set(level + 1, n, degree, G, n_dominated, num_dominated, num_choice, size, dom, min_size, min_dom);

	// undo colouring a vertex red
	dom[u] = 0;
	size--;
	
	for (i = 0; i <= degree[u]; i++)
	{
		num_dominated[G[u][i]]--;
		n_dominated--;	
	}

	printf("n_dominated after undoing colouring a vertex red %d\n", n_dominated);
	printf("function min_dom_set is done\n");
	printf("Minimum dominating set:\n");
	print_vector(min_size, min_dom);
	printf("min_size is %d\n", min_size);
	print_min_dom_set(min_size, n, min_dom);
	exit(EXIT_SUCCESS);
}

void print_vector(int size, int arr[])
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("value %d at index %d\n", arr[i], i);
	}
	printf("\n");
}

void print_min_dom_set(int size, int n, int dom[])
{
	printf("Dominating set size is %d\n", size);
	printf("Dominatng set is:\n");
    int i;
    for(i = 0; i < n; i++) {
        if(dom[i] == 1) {
            printf("%d ", i);
        }
    }

    printf("\n");
}