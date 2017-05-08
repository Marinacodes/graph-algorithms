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

//int read_graph(int *numVertices, int degrees [NMAX], int adjList [NMAX][NMAX]);

void check_graph_validity();

int main (int argc, char *argv[])
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
			exit(EXIT_FAILURE);
		}

		// Increment the number of graph
		graphNum++;	
			
		// For each vertex
		for (i = 0; i < numVertices; i++)
		{
			// Read in its degree
			scanf("%d", &degrees[i]);

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

		#if DEBUG
			printf("This is graph # %d \n", graphNum);
			printf("numVertices is %d \n", numVertices);
			printf("Printing degrees[]\n");
			for (i = 0; i < numVertices; i++)
			{
				printf("%d ", degrees[i]);
			}
			printf("\nPrinting adjList[][]\n");
			for(i = 0; i < numVertices; i++) 
			{
	    		for(j = 0; j < degrees[i]; j++) 
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
			for(i = 0; i < numVertices; i++) 
			{
  				for(j = 0; j < numVertices; j++) 
				{
  					printf("%d ", adjMatrix[i][j]);
				}
   				printf("\n");
			}
		#endif	

		check_graph_validity();		
	}

	return 0;
}

void check_graph_validity() 
{
	// Check the validity of range in proposed dominating set
	int i, j;
	for (i = 0; i < sizeOfDomSet; i++)
	{
		if (domSet[i] < 0 || domSet[i] > numVertices - 1)
		{
			if (verbosity == 0)
			{
				printf("%d    %d \n", graphNum, 0);
			}
			if (verbosity == 1)
			{
				printf("Error- Value %d in the certificate is not in the range [%d, \t %d]\n", domSet[i], 0, numVertices - 1);
				printf("Graph \t %d : BAD CERTIFICATE \n", graphNum);
				printf("============================= \n");
			}
		}
	}
	
	// Initialize adjacency matrix
	for (i = 0; i < numVertices; i++)
	{
		for (j = 0; j < numVertices; j++)
		{
			adjMatrix[i][j] = 0;
		}
	}	
	
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
			if (adjList[i][j] != NMAX && j < degrees[i]) 
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
					printf("%d   %d \n", graphNum, -1);
					exit(EXIT_FAILURE);
				}
				if (verbosity == 1)
				{
					printf("*** Error- adjacency matrix is not symmetric: A[ %d][ %d] = %d, A[ %d][ %d] = %d\n", 
						i, j, adjMatrix[i][j], j, i, adjMatrix[j][i]);
					printf("Graph \t %d : BAD GRAPH \n", graphNum);
					exit(EXIT_FAILURE);
				}
			}				
		}
	} 
}
