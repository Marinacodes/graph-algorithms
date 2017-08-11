/*
 * This program finds a minimum dominating set of a graph. It does not check the input graph for correctness. 
 * Input: provided through standard input 
 * 		  Format: number of vertices, degree of each vertex followed by a list of its neighbours
 * Output: goes out to standard output
 * 		   If verbosity is set to 1, graph followed by a dominating set
 *         If verbosity is set to 0, graph number, number of vertices, dominating set order
 */

#include <stdio.h> 
#include <stdlib.h>

// maximum number of vertices
#define NMAX 2187
// maximum degree
#define DEG_MAX 16
#define DEBUG 0

int read_graph(int* n, int degree[NMAX], int G[NMAX][DEG_MAX]); 
void print_graph(int n, int degree[NMAX], int G[NMAX][DEG_MAX]);
void min_dom_set(int level, int* n, int degree[NMAX], int G[NMAX][DEG_MAX], int* n_dominated, int num_dominated[NMAX],
				int num_choice[NMAX], int* size, int dom[NMAX], int* min_size, int min_dom[NMAX]);
void print_vector(int size, int arr[]);
void print_verbose_min_dom_set(int min_size, int n, int min_dom[]);
void print_nonverbose_min_dom_set(int graph_num, int n, int min_size);

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
	int verbose; // user-specified verbosity level, 0 for terse, 1 for verbose
	int graph_num = 0; // graph number
	
	// Prints error message if incorrect usage
	if (argc < 2)
	{
		printf("Must specify verbosity level (0 for terse, 1 for verbose)\nExample: a.out 0 < in.txt > out.txt\n");
		return EXIT_FAILURE;
	}

    // Reads in the user-specified verbosity level
	verbose = atoi(argv[1]);

	// Processes input    
    while (read_graph(&n, degree, G) == 0)
    {
		// Increments the number of graph
		graph_num++;
        // If verbose output selected, prints graph, finds and prints minimum dominating set
		if (verbose == 1)
		{
			print_graph(n, degree, G);
			min_dom_set(0, &n, degree, G, &n_dominated, num_dominated, num_choice, &size, dom, &min_size, min_dom);
			print_verbose_min_dom_set(min_size, n, min_dom);
		}
		// If terse output selected, prints graph number and number of vertices, finds and prints minimum dominating set order
        else
        {
			min_dom_set(0, &n, degree, G, &n_dominated, num_dominated, num_choice, &size, dom, &min_size, min_dom);
			print_nonverbose_min_dom_set(graph_num, n, min_size);
        }
    }
	
	return EXIT_SUCCESS;
}

/*
 * Reads in a graph from standard input
 */
int read_graph(int* n, int degree[NMAX], int G[NMAX][DEG_MAX]) 
{
    // Reads in input as long as end of file is not reached
	if (fscanf(stdin, "%d", n) == 1)
	{
		int i, j;

		// For each vertex
		for (i = 0; i < *n; i++)
		{
			// Reads in its degree
			if (scanf("%d", &degree[i]) != 1)
			{
				return EXIT_FAILURE;
			}

			if (degree[i] >= DEG_MAX)
			{
				printf("A vertex degree cannot be equal to or greater than %d", DEG_MAX);
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
	}
    return EXIT_FAILURE;
}

/*
 * Prints graph in verbose output mode
 */
void print_graph(int n, int degree[NMAX], int G[NMAX][DEG_MAX])
{
	int i, j;
	
	printf("%d\n", n);

	for (i = 0; i < n; i++)
	{
		printf("%d ", degree[i]);
		for (j = 0; j < degree[i]; j++)
		{			
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
}

/*
 * Finds a minimum dominating set of a graph
 * Coluring: red = in dominating set, blue = excluded from dominating set, white = undecided
 */
void min_dom_set(int level, int* n, int degree[NMAX], int G[NMAX][DEG_MAX], int* n_dominated, int num_dominated[NMAX],
				int num_choice[NMAX], int* size, int dom[NMAX], int* min_size, int min_dom[NMAX])
{

#if DEBUG
	printf("\nLevel %3d: \n", level);
	printf("Current dominating set:\n");
	print_vector(*size, dom);
	printf("Minimum dominating set:\n");
	print_vector(*min_size, min_dom);
	printf("Number of vertices dominated: %3d\n", *n_dominated);
	printf("Number of choices per vertex:\n");
	print_vector(*n, num_choice);
	printf("Number of times dominated:\n");
	print_vector(*n, num_dominated);
	printf("Size is: %d, min_size is: %d\n", *size, *min_size);
#endif

	int i;

	// initialize data structures
	// level is the vertex currently under consideration
	if (level == 0)
	{
		*n_dominated = 0;		
		*size = 0;
		// no values assigned to dom[]
		*min_size = *n;
		
		for (i = 0; i < *n; i++)
		{
			num_choice[i] = degree[i] + 1;
			num_dominated[i] = 0;
			min_dom[i] = i;
			// add vertex to list of own neighbours
			G[i][degree[i]] = i;
		}
	}
	
	// backtrack
	for (i = 0; i < *n; i++)
	{
		if (num_choice[i] == 0)
		{
			return;
		}
	}

	int u = *n - *n_dominated; // number of undominated vertices
	
	int delta = degree[0]; // maximum degree of a vertex
	for (i = 0; i < *n; i++)
	{
		if (degree[i] > delta)
		{
			delta = degree[i];
		}
	}

	int n_extra = (u + delta)/(delta + 1);
	
	if (*size + n_extra >= *min_size)
	{
		return;
	}

	if (level == *n || *n_dominated == *n)
	{
		if (*size < *min_size)
		{
			for (i = 0; i < *n; i++)
			{
				min_dom[i] = dom[i];
			}
			*min_size = *size;
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

	// undo colouring a vertex blue (colour it white)
	for (i = 0; i <= degree[u]; i++)
	{
		num_choice[G[u][i]]++;
	}
	
	// try colouring a vertex red
	dom[u] = 1;
	(*size)++;

	for (i = 0; i <= degree[u]; i++)
	{
		num_dominated[G[u][i]]++;
		// increase number of dominated vertices
		if (num_dominated[G[u][i]] == 1)
		{
			(*n_dominated)++;
		}
	}
	
	// recursive call
	min_dom_set(level + 1, n, degree, G, n_dominated, num_dominated, num_choice, size, dom, min_size, min_dom);

	// undo colouring a vertex red (colour it white)
	dom[u] = 0;
	(*size)--;	

	for (i = 0; i <= degree[u]; i++)
	{
		num_dominated[G[u][i]]--;
		// decrease number of dominated vertices
		if (num_dominated[G[u][i]] == 0)
		{
			(*n_dominated)--;
		}
	}
}

/*
 * Prints contents of an array in debug mode
 */
void print_vector(int size, int arr[])
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d: %d\n", i, arr[i]);
	}
	printf("\n");
}

/*
 * Prints a minimum dominating set order followed by a minimum dominating set in verbose output mode
 */
void print_verbose_min_dom_set(int min_size, int n, int min_dom[])
{
	printf("%d\n", min_size);

    int i;
    for (i = 0; i < n; i++) 
	{
        if (min_dom[i] == 1) 
		{
            printf("%d ", i);
        }
    }
    printf("\n\n");

	fflush(stdout);
}

/*
 * Prints graph number, number of vertices, and a minimum dominating set order in terse output mode
 */
void print_nonverbose_min_dom_set(int graph_num, int n, int min_size)
{
	printf("%5d %3d %3d\n", graph_num, n, min_size);
	
	fflush(stdout);
}