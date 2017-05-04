#include <stdio.h>
#include <stdlib.h> 

// Maximum number of vertices
// If not big enough, print error that it should be increased and recompiled
#define NMAX 100

int main (int argc, char *argv[])
{
	// Task #1 - read input from in.txt
	
	// As long as end of file is not reached, do
		// Assign each graph a number, then for each graph
			// Read in the number of vertices in a graph
				// For each vertex
					// Read in its degree
					// For each degree
						// Read in the vertex's neighbours
			// Read in the proposed size of dominating set
				// Read in each vertex in proposed dominating set

	// Task #2 - process

	// Compose main algorithm here

	// Task #3 - output to out.txt
	
	// Check verbosity level
	
	// If verbosity level is 0
		// Print the graph number
		// If graph is illegal, print -1
		// Else if given set is not dominating set, print 0
		// Else (if given set is dominating set), print 1
	
	// If verbosity level is 1
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
