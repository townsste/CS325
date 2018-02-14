/*********************************************
* CS325 - Assignment 5 - Stooge SortBabyface vs Heels
* Author: Stephen Townsend
* File: babyfaceHeels.cpp
* This c++ file is used to categorize if a group
* of wrestlers can be divided up into Babyfaces
* and Heels.  THis will read from a user defined
* file.  THe file must be in the format of:
* Number of Verticies
* Name of wrestler1
* Name of wrestler2
* Number of Edges
* Name of wrestler1 name of wrestler2
*********************************************/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//Function prototypes
void getArrSize(ifstream &, int &);
void addVertData(ifstream &, string **, int);
void addEdgeData(ifstream &, string **, int, int);
void bfs(string **, int, int);
bool checkViable(string *, string *, int);
void print(string *, string *, int, bool);

int main()
{
	int sizeV = 0;
	int sizeE = 0;
	bool open = false;

	string ** verticies;
	string user;
	ifstream fp;							//Open file

	while (!open)								//Loop till file can be opened
	{
		cout << "Enter a file name followed by .txt: ";

		getline(cin, user);

		fp.open(user.c_str());
		if (fp.is_open())								//Check if open
		{
			open = true;
			cout << endl;
		}
		else
			cout << endl << user << " could not be accessed!\n" << endl;
	}

	//Verticies
	getArrSize(fp, sizeV);						//Get number of verticies

	//Create a dynamic 2D array
	verticies = new string*[sizeV];

	for (int i = 0; i < sizeV+1; i++)
		verticies[i] = new string[sizeV+1];

	addVertData(fp, verticies, sizeV);			//Add Verticies to array

	//Edges
	getArrSize(fp, sizeE);						//Get number of edges

	addEdgeData(fp, verticies, sizeV, sizeE);	//Add edge to array

	bfs(verticies, sizeV, sizeE); //Run BFS

	//Clean up Array
	//for (int i = 0; i < sizeV+1 ; i++)
	//	delete[] verticies[i];

	//verticies = 0;
}


/******************************************
*				getArrSize
* This function is used to determine how 
* many verticies and edges there will be.
******************************************/
void getArrSize(ifstream &file, int &size)
{
	file >> size;							//Read file to size
}


/******************************************
*				addVertData
* This function is used to add the verticies
* to the first row in the matrix based on the
* file that is being read in.
******************************************/
void addVertData(ifstream &file, string **arr, int sizeV)
{
	int i;

	for (i = 0; i < sizeV; i++)
		file >> arr[0][i];					//Read data into array
}


/******************************************
*				addEdgeData
* This function is used to fill in the 2D
* array based on the rival inputs.  This 
* funtions fills out a matrix.  If there is
* a rivalry then this will be updated with 
* both of the members.
******************************************/
void addEdgeData(ifstream &file, string **arr, int sizeV, int sizeE)
{
	string vert;
	string edge;
	int i;
	int j;
	int breakout = 0;
	int addedEdge = 0;

	while (breakout != sizeE)						//If we are not finished continue
	{
		file >> vert;								//Get the verticie
		file >> edge;								//Get the connecting edge

		for (i = 0; i < sizeV; i++)
		{
			if (vert == arr[0][i])					//Find column
				for (j = 0; j < sizeV; j++)
				{
					if (edge == arr[0][j])			//Find edge column
					{
						arr[j+1][i] = edge;			//Place edge in row under the verticy
						arr[i+1][j] = vert;

						addedEdge = 1;				//We added an edge
						breakout++;					//One step closer
						break;						//Leave inner loop early
					}
				}
			if (addedEdge == 1)						//Did we add something?
			{
				addedEdge--;						//Reset counter
				break;								//Leave outer loop early
			}
		}
	}
}


/******************************************
*				bfs
* This function is used fill in an array for
* babyfaces or heels depending on the rivals
* from addEdgeData function.
******************************************/
void bfs(string **arr, int sizeV, int sizeE)
{
	bool viableSplit = true;						//Bool to report if we can make the groups

	//Babyface array to hold babyfaces
	string * babyface;
	babyface = new string[sizeV];

	//Heels array to hold Heels
	string * heels;
	heels = new string[sizeV];

	int babyFCounter = 0;
	int heelsCounter = 0;

	if (sizeE != (sizeV * 2))						//Check if everyone is a rival or not
	{
		int i = 0;									//Use first posiiton in array to check
		int j;
		while (i != 2)								//ONly check the first two people in the array
		{
			for (j = 0; j < sizeV; j++)				//Loop to array
			{
				if (arr[j + 1][i] != "")			//he position is not empty.  Do something
				{
					if (i == 0)						//If we are at first location set rivals to heels
					{
						heels[j] = arr[j + 1][i];
						heelsCounter++;
					}
					else							//Fill in babyfaces
					{
						babyface[j] = arr[j + 1][i];
						babyFCounter++;
					}
				}

			}
			i++;
		}

		viableSplit = checkViable(babyface, heels, sizeV);	//Check our solution

		print(babyface, heels, sizeV, viableSplit);			//Print results
	}
	else
	{
		print(babyface, heels, sizeV, false);			//Print false results
	}

	//Array Cleanup
	delete[] babyface;
	babyface = 0;

	delete[] heels;
	heels = 0;
}


/******************************************
*				checkViable
* This function is used to check the babyfaces
* and heels arrays against each other.  If 
* There is a member in both groups then the
* split cannot be made.
******************************************/
bool checkViable(string *baby, string *heel, int sizeV)
{
	int i = 0;
	int valid = -1;
	while (i != sizeV)
	{
		if (baby[i] == heel[i])
			return false;
		i++;
	}
}


/******************************************
*				print
* This function is used to print the results
******************************************/
void print(string *arrBf, string *arrHl, int size, bool truth)
{
	int i;

	if (truth)
	{
		cout << "Yes" << endl;

		cout << "Babyface: ";

		for (i = 0; i < size; i++)
			cout << arrBf[i] << " ";
		
		cout << "\nHeels: ";

		for (i = 0; i < size; i++)
			cout << arrHl[i] << " ";

		cout << endl;
	}
	else
		cout << "No, if impossible" << endl;
}