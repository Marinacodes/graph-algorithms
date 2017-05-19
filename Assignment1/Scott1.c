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
int graphNum = 0;
int verbosity;

void initialize_data_structures();

int check_graph_validity();

int check_certificate_validity();

void check_dominating_set();

void print_graph();

void print_proposed_dominating_set();

int main(int argc, char *argv[])
{
	// Print error message if incorrect usage
	if (argc != 2)
	{
		printf("Must specify verbosity level (0 for terse, 1 for verbose)\nExample: a.out 0 < in.txt > out.txt\n");
		exit(EXIT_FAILURE);
	}

	verbosity = atoi(argv[1]);

	// As long as end of file is not reached, do
	while (fscanf(stdin, "%d", &numVertices) == 1)
	{
		int i, j;

		// Print error message if incorrect number of vertices
		if (numVertices < 1 || numVertices > 100)
		{
			printf("The number of vertices in a graph must be greater than 0 and less than 100\nAdjust accordingly and recompile\n");
			exit(1);
		}

		initialize_data_structures();

		// Increment the number of graph
		graphNum++;

		// For each vertex
		for (i = 0; i < numVertices; i++)
		{
			// Read in its degree
			if (scanf("%d", &degrees[i]) != 1 || degrees[i] < 0)
			{
				printf("Invalid input. Integers from 0 to %d only are allowed.\n", NMAX);
				exit(1);				
			}

			// For each degree
			for (j = 0; j < degrees[i]; j++)
			{
				// Read in the vertex's neighbours
				scanf("%d", &adjList[i][j]);
			}
		}

		// Read in the size of proposed dominating set
		scanf("%d", &sizeOfDomSet);

		// Read in each vertex in proposed dominating set
		for (i = 0; i < sizeOfDomSet; i++)
		{
			scanf("%d", &domSet[i]);
		}


		// If verbose output, print graph
		if (verbosity == 1)
		{
			print_graph();
		}

		// First, check graph validity
		// If a graph is invalid, the program aborts
		if (check_graph_validity() == 0)
		{
			// If a graph is valid, print proposed dominating set
			if (verbosity == 1)
			{
				print_proposed_dominating_set();
			}
			// Then, check certificate validity
			if (check_certificate_validity() == 0)
			{
				// If a certificate is valid, check dominating set
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
		printf("Printing adjacency matrix\n");
		for (i = 0; i < numVertices; i++)
		{
			for (j = 0; j < numVertices; j++)
			{
				printf("%d ", adjMatrix[i][j]);
			}
			printf("\n");
		}
#endif
	}
	return 0;
}

void print_graph()
{
	int i, j;
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

void check_dominating_set()
{
	int i, j, err;
	int dominatedVertices[numVertices];

	// Initialize dominatedVertices[]
	for (i = 0; i < numVertices; i++)
	{
		dominatedVertices[i] = 0;
	}

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

int check_graph_validity()
{
	int i, j, k, l;

	// Create adjacency matrix from adjacency list
	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < numVertices; j++)
		{
			if (i == j)
			{
				adjMatrix[i][j] = 1;
			}
			// This is hacky and needs to be fixed
			if (j < degrees[i])
			{
				adjMatrix[i][adjList[i][j]] = 1;
			}
		}
	}

	// Check matrix for symmetry
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

	int adjListRow[NMAX];
	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < degrees[i]; j++)
		{
			adjListRow[j] = adjList[i][j];
		}

		// Detect self-loops
		for (k = 0; k < degrees[i]; k++)
		{
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

			// Detect multiple edges
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
						printf("*** Error- Multiple edges found incident to vertex %d\n", adjListRow[k]);
						printf("Graph     %d: BAD GRAPH \n", graphNum);
						exit(1);
					}
				}
			}
		}
	}
	return 0;
}

int check_certificate_validity()
{
	// Check the validity of range in proposed dominating set
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

void initialize_data_structures()
{
	int i, j;
	// Initialize adjacency list and adjacency matrix
	for (i = 0; i < numVertices; i++)
	{
		degrees[i] = 0;
		domSet[i] = 0;
		for (j = 0; j < numVertices; j++)
		{
			adjList[i][j] = 0;
			adjMatrix[i][j] = 0;
		}
	}
}