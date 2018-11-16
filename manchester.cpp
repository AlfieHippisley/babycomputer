// File Purpose - Run A Simulation Of Manchester Baby
// Written By - Alfie Hippisley, Archie Rutherford

// Date Created - 16/11/2018
// Date Last Modified - 16/11/2018

#include <iostream>
#include <fstream>


using namespace std;

// If this is true, main menu will keep displaying
bool runProgram = true;

int store[];

// MainMenu() - Purpose :
// This displays the menu to the user so they can choose an option

void initializeStore() {
	
	// We use a 32 bit wide & deep store
	int bitSize = 32;

	// A 2D array in c is an array of pointers to arrays
	int** store = new int*[bitSize];

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