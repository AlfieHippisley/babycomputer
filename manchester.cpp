// File Purpose - Run A Simulation Of Manchester Baby
// Written By - Alfie Hippisley, Archie Rutherford

// Date Created - 16/11/2018
// Date Last Modified - 17/11/2018

#include <iostream>
#include <fstream>
#include <string>

// We need this unless you want to type std:: :)
using namespace std;

// If this is true, main menu will keep displaying
bool runProgram = true;

// We use a 32 bit wide & deep store
int bitSize = 32;

// A 2D array in C++ is an array of pointers to arrays
int** store = new int*[bitSize];

// The Control Instruction - stores the current line.
int CI = 0;

// Present instruction - The instruction currently being accessed.
int* PI = new int[bitSize];

// The Accumulator - holds number while they are being calculated.
int AC = 0;

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

	// This will store the name of the file to be read
	string fileName;

	// Ask the user for a filename
	cout << "\nEnter the filename : ";
	cin  >> fileName;
	cout << "\n";

	// This will open the selected file
	ifstream myfile(fileName);

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
}

// accumulator() - Purpose :
// The accumulator will process all of the opcode
// and operands taken from the store.
int accumulator() {

	// Fetch
	for (int i=0; i<bitSize; i++) {
		PI[i] = store[CI][i];
	}

	// Decode
	int opcode  = 0;
	int operand = 0;
	string operation;

	// Translate the opcode and operand from binary to integer
 	// #### #000 0000 0### 0000 0000 0000 0000
	if (store[CI][13] == 1) { opcode += 4; }
	if (store[CI][14] == 1) { opcode += 2; }
	if (store[CI][15] == 1) { opcode += 1; }

	if (store[CI][0] == 1) { operand += 16; }
	if (store[CI][1] == 1) { operand += 8;  }
	if (store[CI][2] == 1) { operand += 4;  }
	if (store[CI][3] == 1) { operand += 2;  }
	if (store[CI][4] == 1) { operand += 1;  }

	// Execute
	bool stop = false;

	switch (opcode)
	{
		case 0:
			// JMP
			CI = operand;
			operation = "JMP";
			break;

		case 1:
			// JRP
			CI = CI + operand;
			operation = "JRP";
			break;

		case 2:
			// LND
			AC = -operand;
			operation = "LND";
			break;

		case 3:
			// STO
			operand = AC;
			operation = "STO";
			break;

		case 4:
		case 5:
			// SUB
			AC = AC + operand;
			operation = "SUB";
			break;

		case 6:
			// CMP
			if (AC < 0) { CI++; }
			operation = "CMP";
			break;

		case 7:
			// STP
			stop = true;
			operation = "STP";
			break;
	}

	// Display
	cout << "\nPresent Instruction : ";
	for (int i=0; i<bitSize; i++) { cout << PI[i]; }
	cout << "\nControl Instruction : " << CI;
	cout << "\nOperation           : " << operation;
	if (operand != 0) { cout << "\nOperand             : " << operand; }
	cout << "\nAccumulator         : " << AC;
	cout << endl;

	// Increment Control Instruction
	CI++;

	// Check if Halted
	if (stop || CI == 31) { return 0; }

	return 1;
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
	cout << "0 - Exit Program\n";

	// Get User Input
	cin >> userEntry;

	// Process User Input
	switch (userEntry)
	{
		// Case 1 - Run Binary Code
		case 1:
			initializeStore();
			fillStore();
			while (accumulator() == 1) {}

		break;

		// Case 0 - Exit Program
		case 0:
			cout << "\nYou Are Now Exiting The Program - Good Bye!\n";
			runProgram = false;
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
