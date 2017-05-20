#include <stdio.h>
#include <stdlib.h>

// Maximum number of vertices
#define NMAX 100
#define DEBUG 0

int numVertices;
int degrees[NMAX];
int adjList[NMAX][NMAX];
int adjMatrix[NMAX][NMAX];
int sizeOfDomSet;
int domSet[NMAX];
int dominatedVertices[NMAX];
int graphNum = 0;
int verbosity;

void initialize_data_structures();

void print_graph();

int check_graph_validity();

void print_proposed_dominating_set();

int check_certificate_validity();

void check_dominating_set();

int main(int argc, char *argv[])
{
	// Prints error message if incorrect usage
	if (argc != 2)
	{
		printf("Must specify verbosity level (0 for terse, 1 for verbose)\nExample: a.out 0 < in.txt > out.txt\n");
		exit(1);
	}

	// Reads in the user-specified verbosity level
	verbosity = atoi(argv[1]);

	// Reads in input as long as end of file is not reached
	while (fscanf(stdin, "%d", &numVertices) == 1)
	{
		int i, j;

		// Increments the number of graph
		graphNum++;

		// Prints error message if invalid number of vertices
		if (numVertices < 1 || numVertices > NMAX)
		{
			printf("Invalid input.\nThe number of vertices in a graph must be >= 1 and <= %d.\n", NMAX);
			printf("Graph     %d: BAD GRAPH\n", graphNum);
			exit(1);
		}

		// Initializes array-based data structures to 0
		initialize_data_structures();

		// For each vertex
		for (i = 0; i < numVertices; i++)
		{
			// Reads in its degree
			if (scanf("%d", &degrees[i]) != 1 || degrees[i] < 0 || degrees[i] > numVertices - 1)
			{
				printf("Invalid input.\nThe value of a degree must be between 0 and %d.\n", numVertices - 1);
				printf("Graph     %d: BAD GRAPH \n", graphNum);
				exit(1);
			}

			// For each degree
			for (j = 0; j < degrees[i]; j++)
			{
				// Reads in the vertex's neighbours
				if (scanf("%d", &adjList[i][j]) != 1 || adjList[i][j] < 0 || adjList[i][j] > numVertices - 1)
				{
					printf("Invalid input.\nA vertex must be numbered between 0 and %d.\n", numVertices - 1);
					printf("Graph     %d: BAD GRAPH \n", graphNum);
					exit(1);
				}
			}
		}

		// Reads in the size of proposed dominating set
		if (scanf("%d", &sizeOfDomSet) != 1 || sizeOfDomSet < 1 || sizeOfDomSet > numVertices)
		{
			printf("Invalid input.\nThe size of the dominating set must be >= 1 and <= %d.\n", numVertices);
			printf("Graph     %d: BAD GRAPH \n", graphNum);
			exit(1);
		}

		// Reads in each vertex in proposed dominating set
		for (i = 0; i < sizeOfDomSet; i++)
		{
			if (scanf("%d", &domSet[i]) != 1)
			{
				printf("Invalid input.\nA vertex must be numbered between 0 and %d.\n", numVertices - 1);
				printf("Ensure the proposed dominating set matches its size.\n");
				printf("Graph     %d: BAD GRAPH \n", graphNum);
				exit(1);
			}
		}

		// If verbose output selected, prints graph
		if (verbosity == 1)
		{
			print_graph();
		}

		// First, checks graph validity
		// If a graph is invalid, the program aborts
		if (check_graph_validity() == 0)
		{
			// If a graph is valid, prints proposed dominating set
			if (verbosity == 1)
			{
				print_proposed_dominating_set();
			}
			// Then, checks certificate validity
			if (check_certificate_validity() == 0)
			{
				// If a certificate is valid, checks dominating set
				check_dominating_set();
			}
		}

#if DEBUG
		printf("This is graph # %d \n", graphNum);
		printf("numVertices is %d \n", numVertices);
		printf("Printing degrees[]\n");
		for (i = 0; i < numVertices; i++)
		{
			printf("%d ", degrees[i]);
		}
		printf("\nPrinting adjList[][]\n");
		for (i = 0; i < numVertices; i++)
		{
			for (j = 0; j < degrees[i]; j++)
			{
				printf("%d ", adjList[i][j]);
			}
			printf("\n");
		}
		printf("The size of proposed dominating set is %d\n", sizeOfDomSet);
		printf("Printing domSet[]\n");
		for (i = 0; i < sizeOfDomSet; i++)
		{
			printf("%d ", domSet[i]);
		}
		printf("\n");
		printf("Printing adjMatrix[][]\n");
		for (i = 0; i < numVertices; i++)
		{
			for (j = 0; j < numVertices; j++)
			{
				printf("%d ", adjMatrix[i][j]);
			}
			printf("\n");
		}
		printf("Printing dominatedVertices[]\n");
		for (i = 0; i < numVertices; i++)
		{
			printf("%d ", dominatedVertices[i]);
		}
		printf("\n");
#endif
	}
	return 0;
}

/*
 * Initializes parts of array-based data structures we will be using to 0
 */
void initialize_data_structures()
{
	int i, j;

	for (i = 0; i < numVertices; i++)
	{
		degrees[i] = 0;
		domSet[i] = 0;
		dominatedVertices[i] = 0;
		for (j = 0; j < numVertices; j++)
		{
			adjList[i][j] = 0;
			adjMatrix[i][j] = 0;
		}
	}
}

/*
 * Prints graph
 */
void print_graph()
{
	int i, j;

	// Reorders adjacency list rows in ascending order
	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < degrees[i] - 1; j++)
		{
			if (adjList[i][j] > adjList[i][j + 1])
			{
				int temp = adjList[i][j];
				adjList[i][j] = adjList[i][j + 1];
				adjList[i][j + 1] = temp;
			}
		}
	}

	printf("Graph \t%d:\n", graphNum);
	
	for (i = 0; i < numVertices; i++)
	{
		printf("  %d( %d):", i, (degrees[i]));
		for (j = 0; j < degrees[i]; j++)
		{
			printf("   %d", adjList[i][j]);
		}
		printf("\n");
	}
}

/*
 * Checks the validity of a graph and aborts program if illegal.
 * Examples include graphs with asymmetric adjacency matrices
 * (e.g., digraphs) and multigraphs
 */
int check_graph_validity()
{
	int i, j, k, l;

	// Creates adjacency matrix from adjacency list
	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < numVertices; j++)
		{
			// Sets 1s on the diagonal as a vertex dominates itself
			if (i == j)
			{
				adjMatrix[i][j] = 1;
			}
			// Sets 1s to denote edges between vertices
			if (j < degrees[i])
			{
				adjMatrix[i][adjList[i][j]] = 1;
			}
		}
	}

	// Checks matrix for symmetry
	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < numVertices; j++)
		{
			if (adjMatrix[i][j] != adjMatrix[j][i])
			{
				if (verbosity == 0)
				{
					printf("    %d %d\n", graphNum, -1);
					exit(1);
				}
				if (verbosity == 1)
				{
					printf("*** Error- adjacency matrix is not symmetric: A[ %d][ %d] = %d, A[ %d][ %d] = %d\n",
						   i, j, adjMatrix[i][j], j, i, adjMatrix[j][i]);
					printf("Graph     %d: BAD GRAPH \n", graphNum);
					exit(1);
				}
			}
		}
	}

	// Inspects adjacency list row by row
	int adjListRow[NMAX];

	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < degrees[i]; j++)
		{
			adjListRow[j] = adjList[i][j];
		}

		for (k = 0; k < degrees[i]; k++)
		{
			// Detects self-loops
			if (adjListRow[k] == i)
			{
				if (verbosity == 0)
				{
					printf("    %d %d\n", graphNum, -1);
					exit(1);
				}
				if (verbosity == 1)
				{
					printf("*** Error- Self-loop found at vertex %d\n", i);
					printf("Graph     %d: BAD GRAPH \n", graphNum);
					exit(1);
				}
			}

			// Detects multiple edges
			for (l = k + 1; l < degrees[i]; l++)
			{
				if (adjListRow[k] == adjListRow[l])
				{
					if (verbosity == 0)
					{
						printf("    %d %d\n", graphNum, -1);
						exit(1);
					}
					if (verbosity == 1)
					{
						printf("*** Error- Multiple edges found connecting vertex %d to vertex %d\n", i, adjListRow[k]);
						printf("Graph     %d: BAD GRAPH \n", graphNum);
						exit(1);
					}
				}
			}
		}
	}
	return 0;
}

/*
 * Prints proposed dominating set
 */
void print_proposed_dominating_set()
{
	int i;
	
	printf("Proposed dominating set:\n");
	for (i = 0; i < sizeOfDomSet; i++)
	{
		printf("  %d ", domSet[i]);
	}
	printf("\n");
}

/*
 * Checks the validity of range in proposed dominating set
 */
int check_certificate_validity()
{
	int i, j;

	for (i = 0; i < sizeOfDomSet; i++)
	{
		if (domSet[i] < 0 || domSet[i] > numVertices - 1)
		{
			if (verbosity == 0)
			{
				printf("    %d  %d\n", graphNum, 0);
				return 1;
			}
			if (verbosity == 1)
			{
				printf("Error- Value  %d in the certificate is not in the range [%d,   %d]\n", domSet[i], 0, numVertices - 1);
				printf("Graph     %d: BAD CERTIFICATE \n", graphNum);
				printf("============================= \n");
				return 1;
			}
		}
	}
	return 0;
}

/*
 * Checks that every vertex is dominated
 */
void check_dominating_set()
{
	int i, j, err;

	// Populates dominatedVertices[] with data
	for (i = 0; i < sizeOfDomSet; i++)
	{
		dominatedVertices[domSet[i]] = 1;
	}

	for (i = 0; i < sizeOfDomSet; i++)
	{
		for (j = 0; j < numVertices; j++)
		{
			if (adjMatrix[domSet[i]][j] == 1)
			{
				dominatedVertices[j] = 1;
			}
		}
	}

	for (i = 0; i < numVertices; i++)
	{
		if (dominatedVertices[i] != 1)
		{
			err = 1;
			break;
		}
		else
		{
			err = 0;
		}
	}
	if (err == 1)
	{
		if (verbosity == 0)
		{
			printf("    %d  %d\n", graphNum, 0);
		}
		else if (verbosity == 1)
		{
			printf("Error- Vertex   %d is not dominated\n", i);
			printf("Graph     %d: BAD CERTIFICATE\n", graphNum);
			printf("=============================\n");
		}
	}
	else if (err == 0)
	{
		if (verbosity == 0)
		{
			printf("    %d  %d\n", graphNum, 1);
		}
		else if (verbosity == 1)
		{
			printf("Graph     %d: OK\n", graphNum);
			printf("=============================\n");
		}
	}
}