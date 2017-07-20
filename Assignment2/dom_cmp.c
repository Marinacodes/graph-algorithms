#include <stdio.h>
#include <stdlib.h>
#define STUDENT_MAX 128
#define DEBUG 0
int read_params(FILE *fp, int *n, int *graph_num, int *k);
int main(int argc, char *argv[])
{
    int n_file;
    FILE *fp[STUDENT_MAX];
    int i;


    int n[STUDENT_MAX], graph_num[STUDENT_MAX], k[STUDENT_MAX];

    if (argc  < 3)
    {
        printf("Usage: %s <output files to compare (at least 2)>\n", argv[0]);
        exit(0);
    }

    if (argc > STUDENT_MAX)
    {
         printf("Constant STUDENT_MAX= %3d should be at least the number of files %3d\n", STUDENT_MAX, argc);
         exit(0);
    }

    for (i=1; i < argc; i++)
    {
        fp[i-1]= fopen(argv[i], "r");
        if (fp[i-1] == NULL)
        {
            printf("Failed to open file %2d: %s\n", i, argv[i]);
            exit(0);
        }
    }
   n_file= argc-1;

    printf("  #     n :");
    for (i=1; i < argc; i++)
        printf("   k%0d  ", i);
    printf("\n");
    while (read_params(fp[0], &n[0], &graph_num[0], &k[0]))
    {
         for (i=1 ; i < n_file; i++)
         {

             if (read_params(fp[i], &n[i], &graph_num[i], &k[i]))
             {
                  if (n[0] != n[i])
                  {
                      printf("Error: n values %3d %3d do not match files %s %s \n", 
                             n[0], n[i], argv[1], argv[i+1]);
                      exit(0);
                  }
                  if (graph_num[0] != graph_num[i])
                  {
                      printf("Error: graph number values %3d %3d do not match files %s %s \n", 
                           graph_num[0], graph_num[i], argv[1], argv[i+1]);
                      exit(0);
                  }
#if DEBUG
                  printf("read in from file %1d %s: %4d %4d %4d\n", i, argv[i-1], n[i], graph_num[i], k[i]);
                  fflush(stdout);
#endif
              }
              else
              {
                 k[i]= -1;
              }
         }
         printf("%3d  %4d :",  graph_num[0], n[0]);

         int min_pos, max_pos;

         min_pos=1;
         max_pos=1;

         for (i=2; i < n_file; i++)
         {
            if (k[i] < k[min_pos]) min_pos=i;
            if (k[i] > k[max_pos]) max_pos=i;
         }

         for (i=0; i < n_file; i++)
         {
             
             if (k[i] == -1) printf("  ---  ");
             else 
             {
                printf(" %4d", k[i]);
                if (i > 0 && k[i] == k[min_pos]) printf("- ");
                else if (i > 0 && k[i] == k[max_pos]) printf("+ ");
                else printf("  ");
             }
         }
         printf("\n");
         fflush(stdout);
    }
}
int read_params(FILE *fp, int *n, int *graph_num, int *k)
{
    if (fscanf(fp, "%d", graph_num)!= 1) return(0);

    if (fscanf(fp, "%d", n)!= 1) 
    {
         printf("Error- Failed to read in graph number.\n");
         return(0);
    }
    if (fscanf(fp, "%d", k)!= 1) 
    {
         printf("Error- Failed to read in dominating set size.\n");
         return(0);
    }
    return(1);
}
