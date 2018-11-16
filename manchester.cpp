// File Purpose - Run A Simulation Of Manchester Baby
// Written By - Alfie Hippisley, Archie Rutherford

// Date Created - 16/11/2018
// Date Last Modified - 16/11/2018

#include <iostream>
#include <fstream>
#include <string>

// We need this unless you want to type std:: :)
using namespace std;

// If this is true, main menu will keep displaying
bool runProgram = true;

// We use a 32 bit wide & deep store
int bitSize = 32;

// A 2D array in c is an array of pointers to arrays
int** store = new int*[bitSize];

// initializeStore() - Purpose :
// This will create a 2d array that will act as the store
// Note For Me Later (Alfie) - Get the StackOverflow used for reference

void initializeStore() {

	// We go down the array of pointers creating new arrays of 32 bits
	for (int i = 0; i < bitSize; ++i) 
	{
		store[i] = new int[bitSize];
	}

	// Fill The Array With 0
	for (int i = 0; i < bitSize; i++)
	{
		for (int i2 = 0; i2 < bitSize; i2++)
		{
			store[i][i2] = 0;
		}
	}
}

// fillStore() - Purpose :
// Fill the store with predefined binary code from a file
// Note For Me - University Of Wollongong used for reference

void fillStore() {

	// This will store the lines as they come in
	string line;

	// This is the file link
	ifstream myfile("BabyTest1-MC.txt");

	// This keeps count of what line we are on	 
	int counter = 0;

	// If the file has been found and is open
	if (myfile.is_open())
	{
		while (getline(myfile, line)) // While there are still lines in the file
		{
			for (int i = 0; i < bitSize; i++) // For each index in the string up to defined bitsize eg 32 (String is array of char)
			{	
				char lineChar = line[i]; // The file reads in as Strings, if we use them we would end up storing ascii values
				store[counter][i] = lineChar -'0'; // So we convert the ascii to ints for storage and processing
			}

			// Thats a line complete, so increase counter by one manually due to while loop
			counter++;
		}

		// Close file as we no longer need it
		myfile.close();
	}

	// If we cant find the file display this error message
	else cout << "Unable to open file";

	// Print the values in the array (Debugging only, remove before submission)
	for (int i = 0; i < bitSize; i++)
	{
		cout << endl << i << " Line Is - " << endl;

		for (int i2 = 0; i2 < bitSize; i2++)
		{
			cout << store[i][i2];
		}
	}
}

// mainMenu() - Purpose :
// This displays the user menu and takes their inputs
void mainMenu() {
	
	// Create Int To Store User Input
	int userEntry = 0;

	// Display The Menu
	cout << "\nMain Menu - Please Choose An Option\n";
	cout << "--------------------------------------------\n";
	cout << "1 - Run The Default Binary Code\n";
	cout << "2 - Import Binary Code\n";
	cout << "3 - Exit Program\n";

	// Get User Input
	cin >> userEntry;

	// Process User Input
	switch (userEntry)
	{
		// Case 1 - Run Default Binary Code
		case 1:
			initializeStore();
			fillStore();
		break;

		// Case 2 - Import Binary Code
		case 2:
			cout << "\nYou have choosen option 2\n";
		break;

		// Case 0 - Exit Program
		case 3:
			cout << "\nYou Are Now Exiting The Program - Good Bye!\n";
			exit (0);
		break;

		// Default - For Invalid Input
		default:
			cout << "\nPlease Enter A Valid Single Number As Input\n";
		break;
	}
}

int main()
{
	// Run & Display The Main Menu
	while (runProgram) {
		mainMenu();
	}

	return 0;
}