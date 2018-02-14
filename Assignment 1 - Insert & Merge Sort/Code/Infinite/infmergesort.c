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
void mergeSort(int *arr, int, int, int);
void merge(int *arr, int, int, int, int);
void fillArrayGenerator(int *line, int);


/******************************************
*				main
* This will be used to direct the program.
******************************************/
void main()
{
	srand(time(NULL));			//Seed random number
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

		memset(line, 0, sizeof line);		//Clear Buffer

		fillArrayGenerator(line, size);

		clock_t begin = clock();		//Start the cycle clock

		mergeSort(line, 0, (size - 1), size);	//Sort the row

		clock_t end = clock();			//End the cycle clock
		
		double ranFor = (double)(end - begin) / CLOCKS_PER_SEC;
		
		printf("Running time: %f\n\n", ranFor);

		free(line);						//Free memory array
	}
	printf("Ending Program\n");
}


/******************************************
*				mergeSort
* This function uses the merge sort.  It
* will recursively spit the array in half
* untill it gets to the lowest point.
* From there the function will call merge
* to combine the split arrays in sorted
* order.
******************************************/
void mergeSort(int *line, int low, int high, int size)
{
	/*Priority:  low (Lower limit) < split (middle limit) < high (higher limit)*/

	if (low < high)									//Check if Left less then right.  More then two elements available. Can be split
	{
		int split = (low + high) / 2;				//Find the middle location of the array

		mergeSort(line, low, split, size);			//Recursive left side of array
		mergeSort(line, (split + 1), high, size);	//Recursive right side of array 
		merge(line, low, split, high, size);
	}
}


/******************************************
*				merge
* This function uses merges the split array
* from the mergesort function.  This will 
* look at the left or right side and 
* choose the smallest integer.  The integer
* will be placed in a temp arrat.  The temp
* array will then put the sorted elements
* back into the line array.
******************************************/
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}

	while (left <= split)		//Check if there are more elements on the left
	{
		temp[i] = line[left];	//Place into temp
		//Shift to next location
		i++;
		left++;
	}

	while (right <= high)		//Check if there are more elements on the right
	{
		temp[i] = line[right];	//Place into temp
		//Shift to next location
		i++;
		right++;
	}

	i--;

	while (i >= 0)
	{
		line[low + i] = temp[i];
		i--;
	}

	free(temp);
}


/*******************************************
*			randomGenerator
* This function is used to generate random
* numbers to fill the dynamic array.  From
* 0 to 10000.
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