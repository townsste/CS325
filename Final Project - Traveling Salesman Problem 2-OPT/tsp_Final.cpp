/* Details
*-----------------------------------------------------------
* Project - TSP Group 13
* Stephen Townsend
* Edgar Marquez
* Nadia Viscovich Lopez
* CS325				   Fall 2017
* usage: ./tsp_Final <text file>
*/ 


/* Description
*-----------------------------------------------------------
* Program that given a set of n cities and for each pair of cities
* c1 and c2, the distances between them d(c1, c2) finds an ordering
* (called a tour) of the cities so that the distance traveled is 
* minimized.  The distance traveled is the sum of the distances from
* the first city in the ordering to the second city, plus the distance
* second city to the third city, and so on until the last city is 
* reached, and then adding the distance from the last city to the 
* first city. The program accepts problem instances on the command 
* line. The output file name is the input file’s name with * .tour 
* appended. It compiles/executes correctly and without debugging 
* on flip2.engr.oreognstate.edu.
*/

/* Input Specification
*-----------------------------------------------------------
* A problem instance should always be given as a text file. 
* Each line should define a city  by providing three numbers\
* separated by white space. 
*	The first number is the city identifier
*	The second number is the city’s x-coordinate 
*	The third number is the city’s y-coordinate.
*/

/* Output Specification
*-----------------------------------------------------------
* Output of solution to a file with name being the input file’s name
* with .tour appended. The output text file will contain n+1 lines, 
* where n is the number of cities.  
* The first line is the length of the tour program computes. 
* The next n lines contain the city identifiers in the order they are
* visited by the computed tour.
* Each city will be listed exactly once in this list.  
*/

//Visual Studios - to remove compatibility warnings
#define _CRT_SECURE_NO_WARNINGS

/* Library includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <ctime> 
#include <time.h> 
#include <iomanip>
#include <stack>

using namespace std;

void getFileLength(char*, int &);
bool readFile(struct cityType*, char*, int &);
int distanceFormula(int, int, int, int);
int nearestNeighbor(int**, int*, int);
int makeABetterTour(int*, int, int, int, int, int);
void twoOpt(int**, int *, int &, int);
void twoOptSwap(int**, int*, int &, int, int, int);
void outputFile(char**, int*, int, int);

#define SIZE 256

//Define struct to hold all information per activity
struct cityType
{
	int id;
	int xCoord;
	int yCoord;
};


/* TSP Main Function
*  by: Edgar Marquez
//Driver Function*/
int main(int argc, char* argv[])
{
	char *fileName;
	int n = -1, arrSize = 0;
	bool isValid;

	//Check to see that the correct number of arguments were passed from
	//command line
	if (argc != 2)
	{
		cout << "Invalid number of arguments.\n";
		cout << "usage: ./tsp_Edgar <text file>\n";
		exit(1);
	}
	
	fileName = argv[1];

	getFileLength(fileName, arrSize);

	cityType *Read;
	Read = new cityType[arrSize + 1];

	isValid = readFile(Read, fileName, n);

	//Run if file was able to be opened
	if (isValid == true)
	{
		// Create a graph of the cities
		int **G;

		//Create a dynamic 2D array
		G = new int*[n + 1];

		for (int i = 0; i < n + 1; i++)
			G[i] = new int[n + 1];

		//Create a tour
		int *tour;

		//Create a dynamic array
		tour = new int[n + 1];

		memset(tour, 0, (n+1)*sizeof(int));

		/* This was used for testing purposes only 
		cout << "\nCities:\n";
		//Print Selected Result Array to screen
		for (int j = 0; j < n; j++)
		{
			cout << Read[j].id << " " << Read[j].xCoord << " " << Read[j].yCoord << endl;
		} 
		*/

		// Set distances between cities
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				G[i][j] = distanceFormula(Read[i].xCoord, Read[j].xCoord, Read[i].yCoord, Read[j].yCoord);
			}
		}

		//START TIMER
		clock_t begin = clock();				//Start the cycle clock
		
		int tourLength = nearestNeighbor(G, tour, n);

		twoOpt(G, tour, tourLength, n);

		//END TIMER
		clock_t end = clock();					//End the cycle clock
		
		float ranFor = (float)(end - begin) / CLOCKS_PER_SEC;

		/* This was used for testing purposes only 
		cout << "\nRoute:" << endl;

		for (int i = 0; i < n; i++)
		{
			if (i != n-1)
				cout << tour[i] << " ";
			else
				cout << tour[i];
		}*/
		
		
		cout << "\nRunning time: " << fixed << setprecision(3)<< ranFor << " seconds." << endl;
		cout << "\n\nTour Length: " << tourLength << endl; 

		outputFile(argv, tour, tourLength, n);

		//Cleanup Dynamic Memory
		delete[] Read;
		
		//Free each sub-array
		for (int i = 0; i < n; i++)
			delete[] G[i];

		//Free the array of pointers
		delete[] G;

		delete[] tour;
		
		return 0;
	}
	else
	{
		delete[] Read;

		return -1;
	}
}

/* TSP Get File Length Function
*  by: Stephen Townsend
******************************************
*				getFileLength
* This function is used to jump to the end 
* of the file to get the legth of the file 
* so that the program can dynamically create 
* an array to the correct size.
******************************************/
void getFileLength(char* fileName, int &arrSize)
{
	FILE* fp;
	stack <char> file;

	fp = fopen(fileName, "r");
	
	int read;
	char line[SIZE], *val;

	while (fgets(line, SIZE, fp) != NULL)
	{
		file.push(line[SIZE]);		//Add to the stack
		arrSize++;					//Count stack as added
		file.pop();					//Clear off the stack
	}
	
	fclose(fp);							//Close file
}

/* TSP Read File Function
*  by: Edgar Marquez
******************************************
*				readFile
* This function is used to read the file
* that is specified in the command line.
* If there is an error in reading the program
* will exit with error 1.
******************************************/
bool readFile(cityType* Read, char * fileName, int &n)
{
	// Define Variables
	int valsRead = 0, num;
	char line[SIZE], *val;
	char delims[] = " \t\r\n";
	FILE* inFile;

	// Open and check tsp*.txt file exists
	if ((inFile = fopen(fileName, "r")) == NULL)
	{
		printf("Error opening file %s\n", fileName);
		return false;
	}

	// Read through file until reaching the end
	while (!feof(inFile))
	{
		int i = 0;
		fgets(line, SIZE, inFile);

		// Tokenize line read
		val = strtok(line, delims);
		valsRead = sscanf(val, "%d", &num);

		n++;

		// Pull out integers from line
		while (valsRead > 0)
		{
			val = strtok(NULL, delims);

			// Position of data read corresponds to specific
			// data in cityType struct
			if (i == 0)
			{
				Read[n].id = num;
			}
			if (i == 1)
			{
				Read[n].xCoord = num;
			}
			else
			{
				Read[n].yCoord = num;
			}
			i++;
			if (val == NULL)
			{
				valsRead = 0;
			}
			else
			{
				valsRead = sscanf(val, "%d", &num);
			}
		}
	}

	return true;
}


/* TSP Distance Function
*  by: Stephen Townsend
******************************************
*			distanceFormula
* This function uses the Euclidean distance
* formula to calculate the distance between
* two cities using the x and y cordinates.
* This function takes 2 x coords and 2 y
* coords.  Then will return a numerical
* distance
******************************************/
int distanceFormula(int x1, int x2, int y1, int y2)
{
	int xd = 0;
	int yd = 0;
	double nearest;
	int nearRound;

	//Calculate the difference
	xd = x1 - x2;
	yd = y1 - y2;

	//Square the difference result
	xd *= xd;
	yd *= yd;

	//Square root the squared result
	nearest = sqrt(xd + yd);

	//Round to nearest int
	nearRound = round(nearest);

	return nearRound;
}


/* TSP nearestNeighbor
*  by: Stephen Townsend
******************************************
*				nearestNeighbor
* This function is used to make an intial
* tour using nearest neighbor algorithm.
* The program will start at 0 and go through
* the entire list of cities.  It will check
* for the best length found compareing
* against current best and an arbitrary high
* number that would not be a best solution.
******************************************/
//Step 1: Start with any random vertex, call it current vertex.
//Step 2 : Find an edge which gives minimum distance between the current vertex and an unvisited vertex, call it V.
//Step 3 : Now set that current vertex to unvisited vertex V and mark that vertex V as visited.
//Step 4 : Terminate the condition, if all the vertices are visited at least once.
//Step 5 : Go to step 2.
int nearestNeighbor(int **G, int *tour, int cities)
{
	int *testTour;
	testTour = new int[cities + 1];

	int testLength = 0;
	int finalLength = INT_MAX;		//Set to highest number possible
	int distance = INT_MAX;			//Set to highest number possible
	int nearbyCity;
	int run = 1;
	int cityCounter = 0;

	//Hold the cities in testing tour
	bool *testCity;
	testCity = new bool[cities + 1];

	memset(testCity, false, (cities + 1) * sizeof(bool));

	//First City in array
	testCity[cityCounter] = true;

	//Check each city
	while (cityCounter < cities)
	{
		testTour[0] = cityCounter;													//Place city at beginning of array to test potential length

		while (run < cities)														//While we are not at the end of cities
		{
			for (int i = 0; i < cities; i++)
			{	
				if (testCity[i] == false && G[testTour[run - 1]][i] < distance)		//If the current city has not been used and the previous citys distance is less then current
				{
					distance = G[testTour[run - 1]][i];								//Better distance then previous option	
					nearbyCity = i;													//Hold the city that has least distance
				}
			}

			//ADD CITY
			testTour[run] = nearbyCity;
			testLength += distance;
			testCity[nearbyCity] = true;
			run++;

			distance = INT_MAX;														//Chnage to highest number possible
		}

		//GET DISTANCE
		testLength += G[testTour[0]][testTour[cities - 1]];

		if (testLength < finalLength)												//is there a better solution?
		{
			finalLength = testLength;
			memcpy(tour, testTour, sizeof(int) * cities);
		}

		run = 1;																	//Reset runs
		testLength = 0;																//Test and get new length for new run
		memset(testCity, false, (cities + 1) * sizeof(bool));						//Reset the cities to test to false
		cityCounter++;																//Move to next City
		testCity[cityCounter] = true;												//Next City used in array
	}

	//Cleanup Dynamic Arrays
	delete[] testTour;
	delete[] testCity;

	return finalLength;
}


/* TSP Optimized Tour
*  by: Nadia Viscovich Lopez
******************************************
*				makeABetterTour
* This function is used to make an more 
* optimized tour form the initial tour.  
* This function is called from within 
* twoOptSwap when there is a better tour 
* solution found.
******************************************/
int makeABetterTour(int* tour, int oldLength, int length, int lower, int upper, int cities)
{
	//Create a tour
	int *newTour;
	int index = 0;

	//Create a dynamic array
	newTour = new int[cities];

	// route from 1 to lower - 1, in order
	for (int i = 0; i < lower; i++)
	{
		newTour[index] = tour[i];
		index++;
	}

	// route from lower to upper, in reverse order
	for (int i = upper; i >= lower; i--)
	{
		newTour[index] = tour[i];
		index++;
	}

	// route from upper + 1 to end, in order
	for (int i = upper + 1; i < cities; i++)
	{
		newTour[index] = tour[i];
		index++;
	}

	// move updated tour into tour array
	memcpy(tour, newTour, sizeof(int) * cities);

	return oldLength -= length;
}


/* TSP 2-Opt
*  by: Stephen Townsed
******************************************
*				twoOpt
* This function impliments the 2-opt 
* algorithm. It uses a nested For loop to 
* check send two cities from the G martix
* to the 2-Opt Swap function.  This will 
* keep track of the total tour length.  
* The old length will be compared to a new 
* Length that is being returned from the 
* swap function.  If there was a better 
* solution then the length will adjust and 
* the program will exit out of the loop 
* early.  Once the best solution is found 
* the program will exit out of the while 
* loop.
******************************************/
void twoOpt(int** G, int* tour, int &newLength, int cities)
{
	bool exitW = false;
	bool exitF = false;
	int oldLength = newLength;

	while (!exitW)
	{
		exitW = true;
		exitF = false;

		for (int i = 1; i < cities - 1; i++) 
		{
			for (int j = 2; j < cities; j++) 
			{
				oldLength = newLength;		//Hold the length while we get a new length from swapping
				twoOptSwap(G, tour, newLength, cities, i, j);	//Call the swap function
				if (newLength < oldLength)	//Check if there was a better path found
				{
					exitW = false;		//Continue the while loop
					exitF = true;		//Discontinue both for loops
					break;				//Exit
				}
			}
			if (exitF == true)			//Check if we exit outer for loop
				break;					//Exit
		}
	}
}


/* TSP 2-OPT Swap
*  by: Nadia Viscovich Lopez
******************************************
*				twoOptSwap
* This function is impliment the swap for 
* 2-opt algorithm.  THis functio will check 
* to see if there is a better solution then 
* what we have.  If there is a better 
* solution then it will keep track of the 
* current distance between cities and the 
* new distance bewtween cities.  If the 
* result of these distances is > 0 then 
* there is a better solution.  This will 
* trigger the function makeABetterTour.
******************************************/
void twoOptSwap(int** G, int* tour, int &length, int cities, int left, int right)
{
	//Note:
	//Left = Vertice 1 & Right = Vertice 2

	int lower, upper;	//Limits
	int newLength, currPath, newPath;
	bool betterOption = false;

	if (left > right)
	{
		lower = right;
		upper = left;
	}
	else
	{
		lower = left;
		upper = right;
	}

	//Determine if there is a posible swap 
	//Getting the distance of the current path distance and new path distance between nearest two cities.
if (upper + 1 < cities)
	{	//Get the current distance of the current path (edge, from city 1 and City 2)
		currPath = G[tour[lower - 1]][tour[lower]] + G[tour[upper]][tour[upper + 1]];
		
		//Get the current distance of the new path (edge, from city 1 and City 2)
		newPath = G[tour[lower - 1]][tour[upper]] + G[tour[lower]][tour[upper + 1]];
	}
	else
	{
		//Get the current distance of the current path (edge, from city 1 and City 2)
		currPath = G[tour[lower - 1]][tour[lower]] + G[tour[upper]][tour[0]];

		//Get the current distance of the new path (edge, from city 1 and City 2)
		newPath = G[tour[lower - 1]][tour[upper]] + G[tour[lower]][tour[0]];
	}

	//Better Tour Option
	newLength = currPath - newPath;

	//Check if there is infact a better path between the current and new paths
	if(newLength > 0)
		length = makeABetterTour(tour, length, newLength, lower, upper, cities);
}


//Write tour length and path to ouputfile 
// by: Nadia Viscovich Lopez
void outputFile(char** argv, int* tour, int tourLength, int n)
{
	FILE* outFile;

	char fname_out[100];
	strcpy(fname_out, argv[1]);
	strcat(fname_out, ".tour");

	//used to test outputfile. Will get this from 2OPT Function later.
	//int tourLength = 175;  

	//Copy Solution to tsp_Example_*.txt.tour file 
	outFile = fopen(fname_out, "w");
	if (!outFile) {
		fprintf(stderr, "\nERROR: Failed to open output file: '%s'\n.", fname_out);
		exit(-1);
	}

	fprintf(outFile, "%d\n", tourLength);
	for (int i = 0; i < n; i++) {
		if (i != n-1)
			fprintf(outFile, "%d\n", tour[i]);
		else
			fprintf(outFile, "%d", tour[i]);
	}

	//If there is an error closing the output file, exit with error
	if (fclose(outFile)) {
		fprintf(stderr, "\nERROR: Failed to close output file: '%s'\n", fname_out);
		exit(-1);
	}
}