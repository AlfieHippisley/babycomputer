/*
  MANCHESTER BABY ASSEMBLER
  AC21009 ASSIGNMENT 3 - TEAM 3
   AUTHORS: Esther Bolik (170010779)
             Roman Brodskiy
  MODIFIED: 2018-11-18
*/

#include <cctype> // for isspace()
#include <cstdint> // for uint32_t
#include <fstream>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include <string>

#include "assembler.h"

using namespace std;

vector<string> BabyAssembler::readAssemblyFile(string filename) {
  vector<string> input;
  ifstream infile(filename);
  string line;
  while (getline(infile, line))
    input.push_back(line);
  return input;
}

// Given an opcode (eg. "STO"), this will return the corresponding three-digit
// binary value as a string (eg. "110").
uint8_t BabyAssembler::getInstruction(string instruction) {
  // These variables would be far better suited as class variables, but that was
  // causing problems, so they're here for now.
  //const size_t INSTRUCTION_AMOUNT = 7;
  const map<string, uint8_t> INSTRUCTION_SET = {
      {"JMP", 0}, // 000
      {"JRP", 1}, // 100
      {"LDN", 2}, // 010
      {"STO", 3}, // 110
      {"SUB", 4}, // 001
      {"CMP", 6}, // 011
      {"STP", 7}  // 111
    };

  if (!INSTRUCTION_SET.count(instruction)) {
    throw INSTRUCTION_NOT_FOUND;
  }
  return INSTRUCTION_SET.at(instruction);

  /*
  for (size_t i = 0; i < INSTRUCTION_AMOUNT; i++) {
    if (instruction == INSTRUCTION_SET[i][0]) {
      return (uint8_t)stoi(INSTRUCTION_SET[i][1]);
    }
  }
  throw INSTRUCTION_NOT_FOUND;*/
}

bitset<32> BabyAssembler::babyBinary(uint32_t n) {
  bitset<32> baby;
  for (uint8_t i = 0; i < 32; i++) {
    baby[31 - i] = n & (1 << i);
  }
  return baby;
}

void BabyAssembler::assemble(string sourcefile, string destfile) {
  // read source from text file
  vector<string> source = readAssemblyFile(sourcefile);

  /* FIRST PASS (whitespace/comment removal, token isolation, symbol table population) */

  // The output of the first pass; a vector of vector tuples (representing
  // lines) in the format of { OPCODE, ARGUMENT }, where ARGUMENT is optional
  // (represented by an empty string if absent).
  vector<vector<string>> tokens;

  map<string, uint8_t> symbols; // maps symbol names to store line numbers

  uint8_t lineaddr = 0; // current line number (within the store, not within the source file)
  for (string line : source) {
    vector<string> lineparts(3); // current line tuple, as described above
    size_t part = 0; // current index for line tuple
    bool labelled = false; // whether or not the line has already been labelled (used to ensure max of one per line)
    for (size_t i = 0; i < line.length();) { // for each character of the line (note that 'i' is advanced manually within the loop)
      if (line[i] == ';') { // rest of line is a comment
        break; // next line
      }
      if (isspace(line[i])) { // whitespace
        i++;
        continue; // next character
      }
      if (line[i] == ':') { // there should not be any standalone colons
        throw AssemblerError::UNEXPECTED_TOKEN;  // TODO - return in some way the line/col numbers in the source file at which the token was encountered
      }

      string word;
      if (line[i] >= '0' && line[i] <= '9') {
        if (part == 0) { // this is not the argument, so there should be no number here
          throw AssemblerError::UNEXPECTED_NUMERIC;
        }

        // read entire value
        for (; i < line.length() && !isWordDelimiter(line[i]); i++) {
          if (!(line[i] >= '0' && line[i] <= '9')) {
            throw AssemblerError::UNEXPECTED_TOKEN; // cannot start with numerals then change to letters (or other chars)
          }
          word += line[i];
        }

        lineparts.at(part) = word;

      } else if (isSymbolChar(line[i])) {
        for (; i < line.length() && !isWordDelimiter(line[i]); i++) {
          if (!isSymbolChar(line[i])) {
            throw AssemblerError::UNEXPECTED_TOKEN;
          }
          word += line[i];
        }

        if (i < line.length() && line[i] == ':') { // if the word is followed by a colon
          if (labelled) { // if a label has already been specified for this line
            throw AssemblerError::TOO_MANY_LABELS;
          }
          if (!lineparts[0].empty()) { // if this is not the first token in the line
            throw AssemblerError::UNEXPECTED_TOKEN;
          }
          if (symbols.count(word)) { // if a label with that name has already been defined elsewhere
            throw AssemblerError::DUPLICATE_LABEL;
          }
          i++; // skip colon
          symbols.emplace(word, lineaddr);
          labelled = true;
        } else {
          lineparts.at(part) = word;
          part++;
        }

      } else { // not a recognised character
        throw AssemblerError::UNEXPECTED_TOKEN;
      }

    }
    if (!lineparts[0].empty()) {
      tokens.push_back(lineparts);
      lineaddr++;
    }
  }
  source.clear();


  /* SECOND PASS (label resolution, opcode translation, numeric literal conversions) */

  // Output of the second pass; a vector of 32-bit bitsets, each of which
  // represents one line of the Baby's store. Note that not the entire store is
  // represented here; only the lines which were defined in the source.
  vector<bitset<32>> store(lineaddr);

  for (uint8_t i = 0; i < lineaddr; i++) { // for each line of the intermediate source
    uint32_t lineVal = 0;
    vector<string> line = tokens.at(i);

    if (line[0] == "VAR") {
      if (line[1].empty()) {
        throw AssemblerError::MISSING_ARGUMENT;
      }
      lineVal = stoi(line[1]);

    } else {
      uint8_t instruction = getInstruction(line[0]); // can throw AssemblerError::INSTRUCTION_NOT_FOUND
      lineVal += (instruction << 13);
      if (instruction >= 0 && instruction <= 5) { // the opcode requires an argument
        if (line[1].empty()) {
          throw AssemblerError::MISSING_ARGUMENT;
        }
        if (line[1][0] >= '0' && line[1][0] <= '9') {
          lineVal += stoi(line[1]);
        } else {
          if (!symbols.count(line[1])) {
            throw AssemblerError::UNDEFINED_LABEL;
          }
          lineVal += symbols.at(line[1]);
        }

      }

    }

    store.at(i) = babyBinary(lineVal); // reverse the order of the bits in lineVal and place it in store
  }


  /* SAVE TO FILE */

  ofstream out(destfile);
  for (uint8_t i = 0; i < store.size(); i++) {
    out << store.at(i).to_string() << endl;
  }
}

int main(int argc, char* argv[]) {
  BabyAssembler ass;
  try {
    ass.assemble("BabyTest1-Assembler.txt", "BabyTest1-MC.txt");
  } catch (AssemblerError e) {
    switch (e) {
      case AssemblerError::INSTRUCTION_NOT_FOUND:
        cout << "INSTRUCTION_NOT_FOUND" << endl;
        break;
      case AssemblerError::UNEXPECTED_NUMERIC:
        cout << "UNEXPECTED_NUMERIC" << endl;
        break;
      case AssemblerError::UNEXPECTED_TOKEN:
        cout << "UNEXPECTED_TOKEN" << endl;
        break;
      case AssemblerError::TOO_MANY_LABELS:
        cout << "TOO_MANY_LABELS" << endl;
        break;
      case AssemblerError::UNDEFINED_LABEL:
        cout << "UNDEFINED_LABEL" << endl;
        break;
      case AssemblerError::MISSING_ARGUMENT:
        cout << "MISSING_ARGUMENT" << endl;
        break;
      case AssemblerError::DUPLICATE_LABEL:
        cout << "DUPLICATE_LABEL" << endl;
        break;
    }
  }


  if (argc > 0) {
    ;
  }
  cout << "";
}
