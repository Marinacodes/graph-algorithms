#include <stdio.h> 
#include <stdlib.h>

// maximum number of vertices
#define NMAX 2187
// maximum degree
#define DEG_MAX 16
#define DEBUG 0

// number of vertices
int n;
// degrees of vertices
int degree[NMAX];
// adjacency list
int G[NMAX][DEG_MAX];
int verbose;
int graph_num = 0;

int read_graph();
void print_graph();

int main(int argc, char *argv[])
{
	// Prints error message if incorrect usage
	if (argc != 2)
	{
		printf("Must specify verbosity level (0 for terse, 1 for verbose)\nExample: a.out 0 < in.txt > out.txt\n");
		exit(1);
	}

    // Reads in the user-specified verbosity level
	verbose = atoi(argv[1]);
    
    while (read_graph() == 0)
    {
        // If verbose output selected, prints graph
		if (verbose == 1)
		{
			print_graph();
            // print dominating set
		}
        else
        {
            // min is dominating set order
            // printf("%5d %3d %3d\n", graph_num, n, min);
        }
    }
	
	return 0;
}

/*
 * Reads graph. Returns 0 on success, 1 on failure. 
 */
int read_graph() 
{
    // Reads in input as long as end of file is not reached
	if (fscanf(stdin, "%d", &n) == 1)
	{
		int i, j;

		// Increments the number of graph
		graph_num++;

		// Prints error message if invalid number of vertices
		if (n < 1 || n > NMAX)
		{
			printf("Invalid input.\nThe number of vertices in a graph must be >= 1 and <= %d.\n", NMAX);
			printf("Graph %d: BAD GRAPH\n", graph_num);
			exit(1);
		}

		// For each vertex
		for (i = 0; i < n; i++)
		{
			// Reads in its degree
			if (scanf("%d", &degree[i]) != 1)
			{
				exit(1);
			}

			// For each degree
			for (j = 0; j < degree[i]; j++)
			{
				// Reads in the vertex's neighbours
				if (scanf("%d", &G[i][j]) != 1)
				{
					exit(1);
				}
			}
		}
        return 0;
#if DEBUG
		printf("This is graph # %d \n", graph_num);
		printf("Number of vertices is %d \n", n);
		printf("Printing degree[]\n");
		for (i = 0; i < n; i++)
		{
			printf("%d ", degree[i]);
		}

		printf("Printing adjacency list[][]\n");
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				printf("%d ", G[i][j]);
			}
			printf("\n");
		}
#endif
	}
    return 1;
}

/*
 * Prints graph
 */
void print_graph()
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