// File Purpose - Run A Simulation Of Manchester Baby
// Written By - Alfie Hippisley, Archie Rutherford

// Date Created - 16/11/2018
// Date Last Modified - 16/11/2018

#include <iostream>

using namespace std;

// If this is true, main menu will keep displaying
bool runProgram = true;

// MainMenu() - Purpose :
// This displays the menu to the user so they can choose an option

void mainMenu() {
	
	// Create Int To Store User Input
	int userEntry = 0;

	// Display The Menu
	cout << "Main Menu - Please Choose An Option\n";
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
			cout << "\nYou have choosen option 1\n";
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
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
