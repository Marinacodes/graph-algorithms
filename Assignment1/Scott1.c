#include <stdio.h>
#include <stdlib.h> 

// Maximum number of vertices
// If not big enough, print error that it should be increased and recompiled
#define NMAX 100

int numVertices;
int degrees[NMAX];
int adjList[NMAX][NMAX];
int sizeOfDomSet;
int domSet[NMAX];

//int read_graph(int *numVertices, int degrees [NMAX], int adjList [NMAX][NMAX]);

int main (int argc, char *argv[])
{
	// Task #1 - read input from in.txt
	
	// As long as end of file is not reached, do
	while (!feof(stdin))
	{
	int i, j;
		// Assign each graph a number, then for each graph
			// Read in the number of vertices in a graph
			scanf("%d", &numVertices);
			printf("numVertices is %d \n", numVertices);
				
				// For each vertex
				printf("Printing degrees[]\n");
				for (i = 0; i < numVertices; i++)
				{
					// Read in its degree
					scanf("%d", &degrees[i]);
					printf("%d ", degrees[i]);

					// For each degree
					for (j = 0; j < degrees[i]; j++)
					{
						// Read in the vertex's neighbours
						scanf("%d", &adjList[i][j]);
					}					
				}
				printf("\n");

				printf("Printing adjList[][]\n");
				for(i = 0; i < numVertices; i++) {
    				for(j = 0; j < degrees[i]; j++) {
        				printf("%d ", adjList[i][j]);
    				}
    				printf("\n");
				} 
			
			// Read in the size of proposed dominating set
			scanf("%d", &sizeOfDomSet);
			printf("The size of proposed dominating set is %d\n", sizeOfDomSet);
			
			// Read in each vertex in proposed dominating set
			printf("Printing domSet[]\n");
			for (i = 0; i < sizeOfDomSet; i++)
			{
				scanf("%d", &domSet[i]);	
				printf("%d ", domSet[i]);
			}
			printf("\n");
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
}
