/*********************************************
* CS325 - Assignment 4 - Last to Start
* Author: Stephen Townsend
* File: lasttostart.c
* This c file is used to implement a greedy
* algorithm that uses the last activity to
* schedule and find an optimal soultion working
* backwards.
* It reads from a text file called act and The 
* txt file will need to be in format of,
* number of activities in list and then the list of
* integers seperated by spaces 
* (Activity# Start End).
* Ex: 3
*     3 1 8
*     2 1 2
*     1 3 9
*********************************************/

//Visual Studios - to remove compatibility warnings
#define _CRT_SECURE_NO_WARNINGS

//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Universal File Pointer
FILE *fp;

#define MAX 2147483647

//Function prototypes
int numberOfRuns();
void getArrSize(int *size);
void readFile(int *line, int);
int pathToTake(int *line, int *path, int);
void mergeSort(int *arr, int, int, int);
void merge(int *arr, int, int, int, int);
void outputResults(int *path, int);


/******************************************
*				main
* This will be used to direct the program.
******************************************/
void main()
{
	int size = 0;
	int sorts = 0;
	int counter = 0;
	
	sorts = numberOfRuns();						//Check how much we need to sort

	fp = fopen("act.txt", "r");					//Open the current directory file in read mode 

	while (sorts != 0)
	{
		getArrSize(&size);						//Count the rows in the file
		size *= 3;
		int *line = malloc(size * sizeof(int));
		memset(line, 0, sizeof line);			//Clear Buffer

		int *path = malloc((size/3) * sizeof(int));
		memset(path, 0, sizeof line);			//Clear Buffer
								
		readFile(line, size);					//Get a row from the file

		counter = pathToTake(line, path, size);

		//mergeSort(line, 0, (size - 1), size);	//Sort the row

		/* Print array elements for testing */
			//int i;
			//for (i = 0; i < size; i++)
			//	printf("%d ", line[i]);
			//printf("\n");

		outputResults(path, counter);

		sorts--;								//Decrease remaining rows
		free(line);								//Free memory array
		free(path);								//Free memory array
	}

	fclose(fp);									//Close the file
}


/******************************************
*			numberOfRuns
* This function is used to check how many 
* runs will be needed.  It will count
* the lines and check the first element.
* If the First element is null or a letter
* then the count will not increment.
******************************************/
int numberOfRuns()
{
	fp = fopen("act.txt", "r");					//Open the current directory file in read mode 

	char test[50];								//Holder to store the first element
	int get = 0;

	while (fgets(test, MAX, fp) != NULL)
	{
		int val = atoi(test);
		if (val > 0);							//Do not count invalid or empty rows
		while (val != 0)						//Move the fp forward x times based on val
		{
			fgets(test, MAX, fp);
			val--;
		}
		get++;
	}

	fclose(fp);

	return get;
}


/******************************************
*				getArrSize
* This function is used to determine how
* large each sort array will need to be.
* This is done by using the first element.
* That element determines how many elements 
* there are in each list.
******************************************/
void getArrSize(int *size)
{
	fscanf(fp, "%d", size);						//Capture the first element in the list
}


/******************************************
*				readFile
* This function is used to read the data
* file and place each element from a given
* list into the array that is passed into
* the function.
******************************************/
void readFile(int *line, int size)
{
	int i = 0;
	int j = 0;
	for (i; i < size; i++)
		fscanf(fp, "%d ", &line[i]);
}


/******************************************
*				pathToTake
* This function is to keep track of the path
* to an omptimal schedule we are taking. 
* This uses a for loop to reverse through
* the line array.  It will check the end time
* against the start time of the previous line
* or line up from it.  If the curr time is
* Greater or equal to the next, we can take the
* Lesser time and schedule that activity.
******************************************/
pathToTake(int *line, int *path, int size)
{
	int curr = size-1;								//Keep track of the current location
	int i = size - 4;								//Move down the list to compare
	int j = 0;										//Counter

	path[j] = line[curr - 2];						//Path Starts at the last activity
	j++;

	for (i; i > 0; i--)								//Reverse through the array
	{
		if (line[curr-1] >= line[i])				//If end is greater or equal to start
		{
			path[j] = line[i - 2];					//Schedule the activity
			curr = i;								//Our location is current activity
			j++;
		}
		i -= 2;										//Move to next activity in list
	}
	return j;										//Retun # of activities scheduled
}


/******************************************
*				mergeSort
* This function uses the merge sort.  It
* will recursively spit the array in half
* until it gets to the lowest point.
* From there the function will call merge
* to combine the split arrays in sorted
* order.
******************************************/
void mergeSort(int *line, int low, int high, int size)
{
	/*Priority:  low (Lower limit) < split (middle limit) < high (higher limit)*/

	if (low < high)									//Check if Left less than right.  More then two elements available. Can be split
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
* will be placed in a temp array.  The temp
* array will then put the sorted elements
* back into the line array.
******************************************/
void merge(int *line, int low, int split, int high, int size)
{
	int *temp = malloc(size * sizeof(int));			//array used for merging   

	int left = low;				//Left Side
	int right = split+1;		//Right Side
	int i = 0;

	while (left <= split && right <= high)			//while elements in both lists
	{
		if (line[left] < line[right])				//If left is less then right
		{
			temp[i] = line[left];					//Place left into temp
			//Shift to next location
			i++;
			left++;
		}
		else
		{
			temp[i] = line[right];					//Place right into temp
			//Shift to next location
			i++;
			right++;
		}
	}

	while (left <= split)							//Check if there are more elements on the left
	{
		temp[i] = line[left];						//Place into temp
		//Shift to next location
		i++;
		left++;
	}

	while (right <= high)							//Check if there are more elements on the right
	{
		temp[i] = line[right];						//Place into temp
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


/******************************************
*				outputFile
* This function is used to output the
* Array to the screen in reverse order.
******************************************/
void outputResults(int *path, int size)
{
	int i = size-1;

	printf("Number of activities selected = %d\n", size);

	printf("Activities: ");
	for (i; i >= 0; i--)
		printf("%d ", path[i]);
	printf("\n");
}