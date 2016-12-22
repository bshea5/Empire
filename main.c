#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Empire.h"

#define MAXSIZE 100

int main(int argc, char* argv[]) {
    int nCities = 0;    // 
    int nRows = 1;      // # of rows given for distances

    // can't initialize array with a given variable,
    // so just used the expected max size in the specs
    int adjMatrix[MAXSIZE][MAXSIZE] = {0};

    FILE *fp;
    char line[60];      // track a single row of input
    char* token;        // track a single element in a row

    // opening file for reading 
    fp = fopen("empire01.txt" , "r");

    if(fp == NULL) 
    {
        perror("Error opening file");
        return(-1);
    }

    // first row should have one value, number of cities
    if (fgets(line, 100, fp))
    {
        token = strtok(line, " ");
        printf( " %s\n", token );

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
        printf( " %s\n", line );

        // track # of tokens per row, to compare to nRows
        // if they don't match, than bad input'
        // number of inputs for a row should be the same as the row count
        int nTokens = 0;     
        token = strtok(line, " ");
        
        // walk through other tokens for distance values
        while( token != NULL ) 
        {         
            if ( isdigit(*token) )
            {
                adjMatrix[nRows][nTokens] = atoi(token);
            }
            else if ( tolower(*token) == 'x')
            {
                adjMatrix[nRows][nTokens] = -1;
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

    printf("Number of cities: %i \n", nCities);
    printf("nRows: %i \n", nRows);

    fclose(fp);

    // test call from Empire.c
    test();

    // print matrix
    for (int i = 0; i < nCities; i++ ) {

        for (int j = 0; j < nCities; j++ ) {
            printf("a[%d][%d] = %d\n", i, j, adjMatrix[i][j] );
        }
    }
}
