#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

enum AssemblerTableError{INTSRUCTION_NOT_FOUND};

string getInstruction(string instruction){
	int instructionAmount = 7;
	string instructionSet[instructionAmount][2] =  {{"JMP", "000"},
													{"JRP", "100"},
													{"LDN", "010"},
													{"STO", "110"},
													{"SUB", "001"},
													{"CMP", "011"},
													{"STP", "111"}};
	for(int i = 0; i < instructionAmount; i++){
		if(instruction == instructionSet[i][0])
			return instructionSet[i][1];
	}
	throw(INTSRUCTION_NOT_FOUND);
}

vector<string> readAssemblyFile(string fileName){
	vector<string> input;
	ifstream infile(fileName + ".txt");
	string str;
	while (getline(infile, str))
		input.push_back(str);
	return input;
}

int main(){
	vector<string> input = readAssemblyFile("BabyTest1-Assembler");
	for(string line : input){
		if (line.at(0) != *";"){
			string ss1(1, line.at(0));
			string ss2(1, line.at(1));
			string ss3(1, line.at(2));
			string ss4(1, line.at(3));
			string ss5(1, line.at(4));
			int i = 0;
			string variableName;
			if(line.at(0) == *" "){
				i = 0;
			}
			else if(ss1 + ss2 + ss3 + ss4 + ss5 == "START"){
				i = 6;
				cout << "START ";
			}
			else if(ss1 + ss2 + ss3 == "END"){
				i = 4;
				cout << "END ";
			}
			else{
				i = 6;
				variableName = ss1 + ss2 + ss3 + ss4 + ss5;
			}

			while(line.at(i) == *" "){
				i++;
			}
			string s1(1, line.at(i));
			string s2(1, line.at(i + 1));
			string s3(1, line.at(i + 2));
			string instruction = s1 + s2 + s3;
			try{
				cout << getInstruction(instruction) << "0000000000000000" << endl;
			}
			catch(AssemblerTableError INTSRUCTION_NOT_FOUND){
				if(instruction == "VAR"){
					i += 4;
					string s0;
					while(line.at(i) != *" "){
						string s(1, line.at(i));
						s0 = s0 + s;
						i++;
					}
					int n = stoi(s0);
					cout << bitset<32>(n).to_string() << endl;
				}
			}
		}
	}
	return 0;
}