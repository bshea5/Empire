/*
    Imperial Messengers

    Author: Brandon Shea
    Compiler: Clang Version 19.10.24728 for x86
    Summary: 
        Figure out how long it takes to send a message from the capitol throughout the Empire.

        Dijktra's Algorith is utilized to figure out the shortest routes from the capitol to each city.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAXSIZE 100

int minDistance(int dist[], int sptSet[], int nCities);
void dijkstra(int adjMatrix[MAXSIZE][MAXSIZE], int src, int nCities);
void printMatrix(int adjMatrix[MAXSIZE][MAXSIZE], int nCities);
int printResults(int dist[], int n);

// TODO: reformat for readabilities sake into Empire.h/.c files

int main(int argc, char* argv[]) {
    int nCities = 0;    // 
    int nRows = 1;      // # of rows given for distances

    // can't initialize array with a given variable,
    // so just used the expected max size in the specs
    int adjMatrix[MAXSIZE][MAXSIZE] = {0};
    
    char line[60];      // track a single row of input
    char* token;        // track a single element in a row

    // opening file for reading 
    FILE* fp = fopen("empire01.txt" , "r");

    if(fp == NULL) 
    {
        perror("Error opening file");
        return(-1);
    }

    // first row should have one value, number of cities
    if (fgets(line, 100, fp))
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

    // next n-1 lines should contain nRows values, either a number or a 'x'
    while(fgets(line, 100, fp)) 
    {
        // track # of tokens per row, to compare to nRows
        // if they don't match, than bad input
        // number of inputs for a row should be the same as the row count
        int nTokens = 0;     
        token = strtok(line, " ");
        
        // walk through other tokens for distance values
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
                perror("Error: Invalid input found");
                return(-1);
            }
    
            nTokens++;
            token = strtok(NULL, " ");
        }

        if (nTokens != nRows)
        {
            printf("Incorrect number of tokens for row\n");
            printf("nRows: %i | nTokens: %i \n", nRows, nTokens);
            perror("Error: Incorrect number of tokens");
            return(-1);
        }

        nRows++;
    }

    fclose(fp);
    printf("Number of cities: %i \n", nCities);
    printMatrix(adjMatrix, nCities);
    dijkstra(adjMatrix, 0, nCities);   
}

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[], int sptSet[], int nCities)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < MAXSIZE; v++)
    {
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
    }
    
    return min_index;
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int graph[MAXSIZE][MAXSIZE], int src, int nCities)
{
    int dist[MAXSIZE];      // The output array.  dist[i] will hold the shortest
                            // distance from src to i

    int sptSet[MAXSIZE];    // sptSet[i] will 1 if vertex i is included in shortest
                            // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as 0
    for (int i = 0; i < MAXSIZE; i++)
        dist[i] = INT_MAX, sptSet[i] = 0;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < nCities-1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in first iteration.
        int u = minDistance(dist, sptSet, nCities);

        // Mark the picked vertex as processed
        sptSet[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < nCities; v++)
        {
            // Update dist[v] only if is not in sptSet, there is an edge from 
            // u to v, and total weight of path from src to  v through u is 
            // smaller than current value of dist[v]
            if (!sptSet[v] && (graph[u][v] && graph[u][v] != -1) 
                && dist[u] != INT_MAX && dist[u]+graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // print the constructed distance array
    printResults(dist, nCities);
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

int printResults(int dist[], int n)
{
    int results = -1;
    printf("\nDistances from Source:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d \t\t %d\n", i, dist[i]);
        //results = dist[i] != -1 
    }

    return 1;
}
