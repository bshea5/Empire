/* =====================================================================
    Imperial Messengers

    Author: Brandon Shea
    Compiler: Clang Version 19.10.24728 for x86
    Summary: 
        Figure out the minimun time required to send a message from the 
        capitol to throughout the Empire.

        Dijktra's Algorithm is utilized to figure out the shortest routes 
        from the capitol to each city.

    Instructions:
        ./main <filename> 

        Supply a file that fits the given input specs. The program will 
        return the expected time to deliver the message throughout the 
        Imperial cities.

    Notes:
        - The capitol is assumed to be city index 0.
        - INT_MAX is used to fill the initial distance results array. 
          Any path with that value, is considered invalid since it is
          used for initial values in the distance array.
        - 0 is an acceptable distance input for a path(teleports I suppose).

    TimeTrack:
        Enviroment and version control set up       - 2 hrs
        File I/O and verifying valid inputs         - 4 hrs
        Dijktra's implementation                    - 4 hrs
        Testing, reformatting, and documentation    - 8 hrs
===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAXSIZE 100
#define LINESIZE 1000

// =====================================================================

int minDistance(int dist[], int sPath[], int nCities);
int dijkstra(int adjMatrix[MAXSIZE][MAXSIZE], int src, int nCities);
void printMatrix(int adjMatrix[MAXSIZE][MAXSIZE], int nCities);
void printResults(int dist[], int nCities, int src);

// =====================================================================

int main(int argc, char* argv[]) {
    int nCities = 0;    // 
    int nRows = 1;      // # of rows given for distances

    // can't initialize array with a given variable,
    // so just used the expected max size in the specs
    int adjMatrix[MAXSIZE][MAXSIZE] = { 0 };
    
    char line[LINESIZE];    // track a single row of input
    char* token;            // track a single element in a row

    if ( argc != 2 ) 
    {
        perror("Please supply a file name");
        return -1;
    }
    printf("arg 0: %s", argv[1]);

    // opening file for reading 
    // FILE* fp = fopen("empire02.txt" , "r");
    FILE* fp = fopen(argv[1] , "r");

    if(fp == NULL) 
    {
        perror("Error opening file");
        return(-1);
    }

    // First row should have one value, number of cities.
    if (fgets(line, LINESIZE, fp))
    {
        token = strtok(line, " ");

        if ( isdigit(*token) )
            nCities = atoi(token); 
        else 
        {
            perror("Error: First value must be a valid number");
            return(-1);
        }
    }
    else 
    {
        perror("Error: Invalid input");
        return(-1);
    }

    // The next n-1 lines should contain nRows values, either a number or a 'x' character.
    // Use these to construct the adjacency matrix.
    while(fgets(line, LINESIZE, fp)) 
    {
        int nTokens = 0;    // Track # of tokens per row, to compare to nRows.
                            // If they don't match, than the input invalid.

        token = strtok(line, " ");
        
        // Walk through other tokens for distance values.
        while( token != NULL ) 
        {         
            if ( isdigit(*token) )
            {
                int value = atoi(token);
                adjMatrix[nRows][nTokens] = value;
                adjMatrix[nTokens][nRows] = value;
            }
            else if ( tolower(*token) == 'x' )
            {
                adjMatrix[nRows][nTokens] = -1;
                adjMatrix[nTokens][nRows] = -1;
            }
            else
            {
                printf("Invalid input: %s", token);
                printf("Error: Invalid input found\n");
                return(-1);
            }
    
            nTokens++;
            token = strtok(NULL, " ");
        }

        if (nTokens != nRows)
        {
            printf("nRows: %i | nTokens: %i \n", nRows, nTokens);
            printf("Incorrect number of tokens for row\n");
            return(-1);
        }

        nRows++;
    }

    if (nRows != nCities)
    {
        printf("nRows: %i | nCities: %i \n", nRows, nCities);
        printf("Error: Number of rows for matrix values does not mactch the number of cities given.\n");
        return(-1);
    }

    printf("\nNumber of cities: %i \n", nCities);
    fclose(fp);
    printMatrix(adjMatrix, nCities);
    dijkstra(adjMatrix, 0, nCities);  // assuming 0 index is the capitol 
}

// Find the vertex with minimum distance value, from the set of 
// vertices not yet included in shortest path tree
int minDistance(int dist[], int sPath[], int nCities)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < MAXSIZE; v++)
    {
        if (sPath[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
    }
    
    return min_index;
}

// Funtion that implements Dijkstra's from the src to each vertex
// for a graph represented using adjacency matrix representation
// In this case, src represents the index for our capital city
int dijkstra(int graph[MAXSIZE][MAXSIZE], int src, int nCities)
{
    int dist[MAXSIZE];      // The output array.  dist[i] will hold the shortest
                            // distance from src to i

    int sPath[MAXSIZE];     // sPath[i] will be 1 if vertex i is included in shortest
                            // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INT_MAX and stpSet[] as 0
    for (int i = 0; i < MAXSIZE; i++)
        dist[i] = INT_MAX, sPath[i] = 0;

    // Distance of source vertex(the Capitol in this case) from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < nCities-1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in first iteration.
        int u = minDistance(dist, sPath, nCities);

        // Mark the picked vertex as processed
        sPath[u] = 1;

        // Update distance value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < nCities; v++)
        {
            int newDistance = dist[u] + graph[u][v];

            if (!sPath[v] 
                && (graph[u][v] || graph[u][v] == 0)    // 0 is acceptable, think teleports 
                && graph[u][v] > -1                     // negative represents blocked path
                && dist[u] != INT_MAX
                && newDistance < dist[v])
            {
                dist[v] = newDistance;

                // handle distances that exceed the maximun integer and turn negative
                if (dist[v] < 0)
                {
                    printf("\nThe empire is too vast. A path exceeds INT_MAX: %d\n", dist[v]);
                    return -1;
                }
            }
        }
    }

    printResults(dist, nCities, src);
    return 1;
}

void printMatrix(int adjMatrix[MAXSIZE][MAXSIZE], int nCities)
{
    for (int i = 0; i < nCities; i++ ) {
        for (int j = 0; j < nCities; j++ ) {
            printf("a[%d][%d] = %d\t", i, j, adjMatrix[i][j]);
        }
        printf("\n");
    }
}

// print the constructed distance array and the minimum time required
void printResults(int dist[], int nCities, int src)
{
    int result = -1;
    printf("\nDistances from Source:\n");
    for (int i = 0; i < nCities; i++)
    {
        printf("%d%s \t %d\n", 
            i, 
            i == src ? " (Capitol)" : "\t", 
            dist[i]
        );
        result = result < dist[i] ? dist[i]: result;
    }

    if (result == -1 || result == INT_MAX)
        printf("\n\nMessage cannot be delivered.\n\n");
    else
        printf("\n\nMininum time required to deliver message throughout the Imperial Cities:\033[32m %d \033[0m\n\n"
            , result);
}
