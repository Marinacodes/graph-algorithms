#include <stdio.h>
#include <stdlib.h> 

// Maximum number of vertices
// If not big enough, print error that it should be increased and recompiled
#define NMAX 100
#define DEBUG 0

int numVertices;
int degrees[NMAX];
int adjList[NMAX][NMAX];
int sizeOfDomSet;
int domSet[NMAX];
int graphNum = 1;
int verbosity;

//int read_graph(int *numVertices, int degrees [NMAX], int adjList [NMAX][NMAX]);

void check_graph_validity();

int main (int argc, char *argv[])
{
	// Task #1 - read input from in.txt
    
	verbosity = atoi(argv[1]);

	// As long as end of file is not reached, do
	while (fscanf(stdin, "%d", &numVertices) == 1) 
	{
		int i, j;
				
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
		
		check_graph_validity();
		
		#if DEBUG
			printf("\nThis is graph # %d \n", graphNum);
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
		#endif

		// Increment the number of graph
		graphNum++;
		
	}

	// Task #2 - process

	// Compose main algorithm here
	

	// Task #3 - output to out.txt
	
	// Check verbosity level
	// printf("verbosity is %s \n", argv[1]);
	// int verbosity = argv[1];
	
	// If verbosity level is 0
	// if (verbosity == 0)
		// Print the graph number
		// If graph is illegal, print -1
		// Else if given set is not dominating set, print 0
		// Else (if given set is dominating set), print 1
	
	// If verbosity level is 1
	// if (verbosity == 1)
		// Print the graph number
		// Print graph
		// If the proposed dominating set is valid
			// Print the proposed dominating set
			// Print graph number followed by OK
		// If the graph is invalid
			// Print an error message explaining why the graph is invalid
			// Print graph number followed by BAD GRAPH
			// Abort program
		// If the certificate is invalid
			// Print the proposed dominating set
			// Print error message, explaining why certificate is invalid
			// Print graph number followed by BAD CERTIFICATE
	return 0;
}

void check_graph_validity() 
{
	int i;
	for (i = 0; i < sizeOfDomSet; i++)
	{
		if (domSet[i] < 0 || domSet[i] > numVertices - 1)
		{
			if (verbosity == 0)
			{
				printf("%d \t %d \n", graphNum, 0);
			}
			if (verbosity == 1)
			{
				printf("Error- Value %d in the certificate is not in the range [%d, \t %d] \n", domSet[i], 0, numVertices - 1);
				printf("Graph \t %d : BAD CERTIFICATE \n", graphNum);
				printf("============================= \n");
			}
		}
	}
}
