/*********************************************
* CS325 - Assignment 1 - Insertion Sort
* Author: Stephen Townsend
* File: insertsort.c
* This c file is used to implement the insertion
* sort algorithm.  This program will read a
* text file called data.txt.
* The txt file will need to be in format of,
* number of items in list and then the list of
* integers seperated by spaces.
* Ex: 5 6 3 8 1 2
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
int numberOfSorts();
void getArrSize(int *size);
void readFile(int *line, int);
void insertionSort(int *line, int);
void outputFile(int *line, int);


/******************************************
*				main
* This will be used to direct the program.
******************************************/
void main()
{
	int size = 0;
	int sorts = 0;
	
	sorts = numberOfSorts();					//Check how much we need to sort

	fopen("insert.out", "w");					//Create new merge.out file

	fp = fopen("data.txt", "r");				//Open the current directory file in read mode 

	while (sorts != 0)
	{
		getArrSize(&size);						//Count the rows in the file

		int *line = malloc(size * sizeof(int));

		memset(line, 0, sizeof line);			//Clear Buffer

		readFile(line, size);					//Get a row from the file

		insertionSort(line, size);				//Sort the row

		/* Print array elements for testing */
			//int i;
			//for (i = 0; i < size; i++)
			//	printf("%d ", line[i]);
			//printf("\n");

		outputFile(line, size);

		sorts--;								//Decrease remaining rows
		free(line);								//Free memory array
	}

	fclose(fp);									//Close the file
}


/******************************************
*			numberOfSorts
* This function is used to check how many 
* sorts will need to be run.  It will count
* the lines and check the first element.
* If the First element is null or a letter
* then the count will not increment.
******************************************/
int numberOfSorts()
{
	fp = fopen("data.txt", "r");				//Open the current directory file in read mode 

	char test[50];					//Holder to store the first element
	int get = 0;

	while (fgets(test, MAX, fp) != NULL)
	{
		int val = atoi(test);
		if (val > 0)							//Do not count invalid or empty rows
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
	for (i; i < size; i++)
	{
		fscanf(fp, "%d ", &line[i]);
	}
}


/******************************************
*			insertionSort
* This function uses the insertion sort.
* An element is compared to the previous
* and if it is less than that element 
* the positions swap.  This will go down
* the array until all elements are sorted.
* The key is the element being compared.
* Anything greater moves ahead of the
* current position and the key moves down
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

		while (prev >= 0 && line[prev] > key)	//Check if at end location and location is greater than key
		{
			line[prev + 1] = line[prev];		//Move prev element forward
			prev = prev - 1;					//Go back one element
		}
		
		line[prev + 1] = key;					//Move key element back
	}
}


/******************************************
*				outputFile
* This function is used to output the sorted
* Array to a file called insert.out.
******************************************/
void outputFile(int *line, int size)
{
	FILE *fpOut;
	int i = 0;

	fpOut = fopen("insert.out", "a");			//Open insert.out file

	while (i < size)
	{
		fprintf(fpOut, "%d ", line[i]);			//Add sorted array to file
		i++;
	}

	fprintf(fpOut, "\n");						//Print new line to file

	fclose(fpOut);								//Close file
}