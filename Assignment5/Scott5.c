/*
 * This program reads in a combination of permutations and dominating sets and maps the dominating sets to the permutations.
 */

#include <stdio.h> 
#include <stdlib.h>

#define MAXMDSSIZE 28  // add extra 1 for dom set size -> c_06 9, c_09 19, c_11 28
#define NUMDOMSETS 1404 // number of dominating sets -> c_06 18, c_09 36, c_11 1404
#define ROWS 968 // group order -> c_06 288, c_09 648, c_11 968
#define COLS 121 // n vertices in a k x k cycle graph -> c_06 36, c_09 81, c_11 121
#define DEBUG 0

int read_input(int* n, int* group_order, int G[ROWS][COLS], int min_dom_set[NUMDOMSETS][MAXMDSSIZE]);
void print_permutations(int G[ROWS][COLS]);
void print_min_dom_sets(int min_dom_set[NUMDOMSETS][MAXMDSSIZE]);
void map_automorphisms(int G[ROWS][COLS], int min_dom_set[NUMDOMSETS][MAXMDSSIZE]);
int cmpfunc (const void * a, const void * b);
void print_vector (int size, int arr[]);

int main()
{
	int n; // number of vertices
	int group_order;
	int G[ROWS][COLS]; // graph matrix
    int min_dom_set[NUMDOMSETS][MAXMDSSIZE]; // dom set matrix
	
    // Processes input
    while (read_input(&n, &group_order, G, min_dom_set) == 0)
    {
#if DEBUG
        print_permutations(G);
        print_min_dom_sets(min_dom_set);
#endif
        map_automorphisms(G, min_dom_set);
    }

	return EXIT_SUCCESS;
}

/*
 * Reads in a graph from standard input
 */
int read_input(int* n, int* group_order, int G[ROWS][COLS], int min_dom_set[NUMDOMSETS][MAXMDSSIZE]) 
{
    // Reads in input as long as end of file is not reached
	if (fscanf(stdin, "%d", n) == 1)
	{
        if (scanf("%d", group_order) != 1)
		{
            return EXIT_FAILURE;
		}
		int i, j;

		for (i = 0; i < ROWS; i++)
		{
			for (j = 0; j < COLS; j++)
			{
				if (scanf("%d", &G[i][j]) != 1)
				{
                    
				    return EXIT_FAILURE;
				}			    
			}            
		}
        for (i = 0; i < NUMDOMSETS; i++)
        {
            for (j = 0; j < MAXMDSSIZE; j++)
            {
                if (scanf("%d", &min_dom_set[i][j]) != 1)
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
 * Prints permutations in debug mode
 */
void print_permutations(int G[ROWS][COLS])
{
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }    
    printf("\n");
}

/*
 * Prints minimum dominating sets in debug mode
 */
void print_min_dom_sets(int min_dom_set[NUMDOMSETS][MAXMDSSIZE])
{
    int i, j;
    for (i = 0; i < NUMDOMSETS; i++)
    {
        for (j = 1; j < MAXMDSSIZE; j++)
        {
            printf("%d ", min_dom_set[i][j]);
        }
        printf("\n");
    }
}

/*
 * Maps automorphisms in various permutations to the identity matrix
 */
void map_automorphisms(int G[ROWS][COLS], int min_dom_set[NUMDOMSETS][MAXMDSSIZE])
{
    int i, j, k; 
    int arr[MAXMDSSIZE - 1]; 

    for (i = 0; i < NUMDOMSETS; i++) 
    {
        for (k = 0; k < ROWS; k++)
        {
            for (j = 1; j < MAXMDSSIZE; j++)
            {
                arr[j] = G[k][min_dom_set[i][j]];              
            }
            qsort(arr, MAXMDSSIZE, sizeof(int), cmpfunc);
#if DEBUG
            printf("%d  ", k);
#endif
            print_vector(MAXMDSSIZE, arr);
        }
        printf("\n");
    }
}

/*
 * Sorting function
 */
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

/*
 * Array printing utility
 */
void print_vector(int size, int arr[])
{
	int i;
	for (i = 1; i < size; i++)
	{
		printf("%3d ", arr[i]);
	}
	printf("\n");
}