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
void insertionSort(int *line, int);
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

		insertionSort(line, size);				//Sort the row

		clock_t end = clock();					//End the cycle clock

		double ranFor = (double)(end - begin) / CLOCKS_PER_SEC;

		printf("Running time: %f\n\n", ranFor);

		free(line);								//Free memory array
	}
	printf("Ending Program\n");
}

/******************************************
*				insertionSort
* This function uses the insertion sort.
* An element is compared to the previous
* and if it is less then that element 
* the positions swap.  This will go down
* the array untill all elements are sorted.
* The key is the element being compared.
* Anything greater moves ahead of the
* current possition and the key moves down
* one position.
******************************************/
void insertionSort(int *line, int size)
{
	int i;
	int prev;
	int key;									

	for (i = 1; i < size; i++)
	{
		key = line[i];							//Element to compare
		prev = i - 1;

		while (prev >= 0 && line[prev] > key)	//Check if at end location and location is greater then key
		{
			line[prev + 1] = line[prev];		//Move prev element forword
			prev = prev - 1;					//Go back one element
		}
		
		line[prev + 1] = key;					//Move key element back
	}
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