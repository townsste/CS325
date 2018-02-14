//Visual Studios - to remove compatibility warnings
#define _CRT_SECURE_NO_WARNINGS

//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

//Universal File Pointer
FILE *fp;

//Function prototypes
void stoogeSort(int *arr, int, int);
void fillArrayGenerator(int *line, int);


/******************************************
*				main
* This will be used to direct the program.
******************************************/
void main()
{
	srand(time(NULL));							//Seed random number
	int runTime = true;
	char input[2] = { NULL };
	int size = 0;
	char str[2] = "n";
	double ranFor = 0;

	while (runTime == true)
	{
		printf("How large is array? ");
		scanf("%d", &size);

		printf("Random Number to Sort: %d\n", size);

		int *line = malloc(size * sizeof(int));

		memset(line, 0, sizeof line);			//Clear Buffer

		fillArrayGenerator(line, size);

		clock_t begin = clock();				//Start the cycle clock

		stoogeSort(line, 0, (size - 1));		//Sort the row

		clock_t end = clock();					//End the cycle clock

		double ranFor = (double)(end - begin) / CLOCKS_PER_SEC;

		printf("Running time: %f\n\n", ranFor);

		free(line);								//Free memory array
	}
	printf("Ending Program\n");
}

/******************************************
*				stoogeSort
* This function uses the stooge sort 
* algorithm.  The sort will split the array
* into 2/3 and sort the first two thirds.  
* It will then go to the next two thirds and
* sort.  THen it will go back to the first
* 2/3 array and sort again if needed.
******************************************/
void stoogeSort(int *line, int low, int high)
{
	int temp = 0;
	int m = 0;

	if (high == 2 || line[low] > line[high])
	{
		temp = line[low];
		line[low] = line[high];
		line[high] = temp;
	}
	if ((high - low + 1) > 2)
	{
		m = ((high - low + 1) / 3);
		stoogeSort(line, low, (high - m));
		stoogeSort(line, (low + m), high);
		stoogeSort(line, low, (high - m));
	}

	/*
	Source: 
	Implementation based on:
	Homework pseudocode and https://en.wikipedia.org/wiki/Stooge_sort
	*/
}


/*******************************************
*			randomGenerator
* This function is used to generate a random
* number so that the program can use a random
* room durring the connections process.
********************************************/
void fillArrayGenerator(int *line, int size)
{
	int i = 0;

	while (size != 0)
	{
		line[i] = rand() % 10001;					//Select a random room between 0-10,000
		i++;
		size--;
	}
}