/*********************************************
* CS325 - Assignment 3 - Minimum Coin Chnage
* Author: Stephen Townsend
* File: minCoinChange.c
* This c file is used to implement the minimum
* Coin Change algorithm.  This program will 
* read a text file called amount.txt.
* The txt file will need to be in format of,
* coin denominations as integers separated by 
* spaces.  Then a new line with the change 
* requested.
* Ex: 1 3 7 12
*     29
*********************************************/

//Visual Studios - to remove compatibility warnings
#define _CRT_SECURE_NO_WARNINGS

//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Universal File Pointer
FILE *fp;
FILE *fp2;

#define MAX 2147483647

//Function prototypes
int numberOfRuns();
int getArrSize();
void readFile(int *line, int);
int minCoinChange(int *arr, int *count, int, int);
void outputFile(int *line, int *count, int, int);


/******************************************
*				main
* This will be used to direct the program.
******************************************/
void main()
{
	int size = 0;
	int runs = 0;
	int coinsUsed = 0;
	
	runs = numberOfRuns();							//Check how many times to calculate change

	fopen("change.txt", "w");						//Create new change.txt file

	fp = fopen("amount.txt", "r");					//Open file in read mode to use file data
	fp2 = fopen("amount.txt", "r");					//Open file in read mode to calculate each lines size

	while (runs != 0)
	{
		size = getArrSize();						//Count the rows in the file
		
		int *line = malloc((size+1) * sizeof(int)); //Array to hold coin denominations and change amount
		int *count = malloc(size * sizeof(int));	//Array to keep count of coins needed
		
		int i;
		for (i = 0; i < size; i++)				//Set each element of arrays to 0
		{
			line[i] = 0;
			count[i] = 0;
		}

		line[size + 1] = 0;							//Extra element in line to set to zero.  Pos holds change amount

		readFile(line, size);						//Get a row from the file

		coinsUsed = minCoinChange(line, count, line[size], size);	//Line[size] == total change
		
		/* Print array elements for testing */
			//int i;
			//for (i = 0; i < size; i++)
			//	printf("%d ", line[i]);
			//printf("\n");

		outputFile(line, count, coinsUsed, size);

		runs--;										//Decrease remaining runs
		free(count);								//Free memory array
	}

	fclose(fp);										//Close fp file
	fclose(fp2);									//Close fp2 file
}


/******************************************
*			numberOfRuns
* This function is used to check how many
* changes will need to be run.  It will 
* count the lines and check the first 
* element. If the First element is null 
* or a letter then the count will not 
* increment.
******************************************/
int numberOfRuns()
{
	fp = fopen("amount.txt", "r");					//Open the current directory file in read mode 

	char test[50];									//Holder to store the first element
	int get = 0;

	while (fgets(test, MAX, fp) != NULL)
	{
		int val = atoi(test);
		if (val > 0)								//Do not count invalid or empty rows
			get++;
	}

	fclose(fp);

	return (get/2);
}


/******************************************
*				getArrSize
* This function is used to determine how
* large each array will need to be.
* This is done by counting the spaces and 
* newline character.  This will allow for 
* an accurate count.  Trying to count each 
* element seemed to be a problem with more 
* then one-digit integers.
******************************************/
int getArrSize()
{
	int size = 0;
	int c = 0;
	int next = 0;
	
	do
	{
		c = fgetc(fp2);

		if (c == 32 || c == 10 && next != 1)		//Check space and newline and at coin values line
			size++;

		if (c == 10)								
			next++;									//A newline character was found

		if (c == -1)								//At end of file with no newline character
			break;

	} while (next != 2);							//Keep looping until fp is at next set of denominations

	return size;
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
	int i;
	for (i = 0; i <= size; i++)
	{
		fscanf(fp, "%d ", &line[i]);
	}
}


/******************************************
*				minCoinChange
* This function uses the minimum coin change
* algorithm.  This is using dynamic 
* programing using the bottom-up method with 
* iteration.  It is designed to use two arrays.  
* One array is used for total coin and the 
* other is a tracker coin array.  The 
* function will go through each coin and 
* iterate through the total array to 
* determine if a coin can make the total.  
* If the total can be made with the current 
* coin then the second array will hold that 
* coins position.  This is a way to track 
* and remember what coins can make what totals.  
* The end result is taking the last element 
* in the array and subtracting that with 
* the coin location from the second array.  
* By subtracting the totals of the coins, we 
* are able to gather what the coin combination 
* is.  This will be the minimum amount of 
* coins needed to make the designated total.
******************************************/
int minCoinChange(int *coins, int *coinCount, int total, int size)
{
	int coinsUsed = 0;
	int *totalCArr = malloc((total + 1) * sizeof(int));
	int *trackCArr = malloc((total + 1) * sizeof(int));

	int k;
	for (k = 0; k < total + 1; k++)
	{
		if (k == 0)
			totalCArr[k] = 0;						//Set the first element in the total array to 0
		else
			totalCArr[k] = MAX;						//Set every other element to infinity
		
		trackCArr[k] = -1;							//Set each element in the track array to -1
	}

	int i;
	int j;
	for (i = 0; i < size; i++)						//Loop to check coin denominations
	{
		for (j = 1; j <= total; j++)				//Loop through each coin option from 0 to total
		{
			if (j >= coins[i])						//If current location is greater or equal to the current coin 
				if (totalCArr[j - coins[i]] + 1 < totalCArr[j])
				{
					totalCArr[j] = totalCArr[j - coins[i]] + 1;
					trackCArr[j] = i;
				}
		}
	}
	
	//Get the locations of the coins that make the total and how many coins were used
	int loc = total;
	while (loc != 0)
	{
		i = trackCArr[loc];						//Starting at the end of the array == total change needed
		loc -= coins[i];						//Subtract the current location with the coin at the given position
		coinCount[i] += 1;						//Add one to the coinCount array to keep track of qty of each coin
		coinsUsed++;							//Add one to the coinUsed total
	}

		free(totalCArr);						//Free the synamic array
		free(trackCArr);						//Free the synamic array

		return coinsUsed;						//Return the total number of coins that were used
}

/******************************************
*				outputFile
* This function is used to output the coin
* change information to a file called 
* change.txt.
******************************************/
void outputFile(int *line, int *count, int coinsUsed, int size)
{
	FILE *fpOut;
	int i = 0;

	fpOut = fopen("change.txt", "a");			//Open change.txt file

	while (i < size)
	{
		fprintf(fpOut, "%d ", line[i]);			//Add coin denominations to file
		i++;
	}

	fprintf(fpOut, "\n%d\n", line[i]);			//Add change integer to file

	i = 0;
	while (i < size)
	{
		fprintf(fpOut, "%d ", count[i]);		//Add coin combination to file
		i++;
	}

	fprintf(fpOut, "\n%d\n", coinsUsed);		//Add total coins used to file

	fclose(fpOut);								//Close file
}