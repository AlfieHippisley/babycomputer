/*
  MANCHESTER BABY ASSEMBLER - MENU
  AC21009 COURSEWORK 3 - TEAM 3
    AUTHOR: Esther Bolik (170010779)
  MODIFIED: 2018-11-19
*/

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "assembler.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

bool attemptAssembly(string sourcename, string destname) {
  BabyAssembler assembler;
  try {
    assembler.assemble(sourcename, destname);
    cout << "'" + sourcename + "' successfully assembled to '" << destname << "'!" << endl;
    return true;

  } catch (AssemblerError e) {
    switch (e) {
      case AssemblerError::INSTRUCTION_NOT_FOUND:
        cerr << "ERROR: '" << assembler.getErrVal() << "' was not recognised as an opcode." << endl;
        break;
      case AssemblerError::UNEXPECTED_NUMERIC:
        cerr << "ERROR: Unexpected numerical value encountered: '" << assembler.getErrVal() << "'." << endl;
        break;
      case AssemblerError::UNEXPECTED_TOKEN:
        cerr << "ERROR: Unexpected token encountered: '" << assembler.getErrVal() << "'." << endl;
        break;
      case AssemblerError::TOO_MANY_LABELS:
        cerr << "ERROR: Multiple labels defined for single line: '" << assembler.getErrVal() << "'." << endl;
        break;
      case AssemblerError::UNDEFINED_LABEL:
        cerr << "ERROR: The label '" << assembler.getErrVal() << "' is never defined." << endl;
        break;
      case AssemblerError::MISSING_ARGUMENT:
        cerr << "ERROR: The operation '" << assembler.getErrVal() << "' requires an argument." << endl;
        break;
      case AssemblerError::DUPLICATE_LABEL:
        cerr << "ERROR: The label '" << assembler.getErrVal() << "' is defined in more than one location." << endl;
        break;
    }

  } catch (ios_base::failure& e) {
    cerr << "ERROR: " << e.what() << endl;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

// Gets and returns a boolean value from cin, or throws a range_error if the
// input is invalid.
bool getBool() {
  string b;
  getline(cin, b);
  if (b == "y" || b == "Y" || b == "yes" || b == "Yes" || b == "YES"
      || b == "t" || b == "T" || b == "true" || b == "True" || b == "TRUE"
      || b == "1") {
    return true;
  } else if (b == "n" || b == "N" || b == "no" || b == "No" || b == "NO"
      || b == "f" || b == "F" || b == "false" || b == "False" || b =="FALSE"
      || b == "0") {
    return false;
  }
  throw range_error("value '" + b + "' not recognised as boolean"); // not the best exception class to use
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

  /* CLI */

  if (argc > 1) {
    vector<string> toAssemble;
    for (int i = 1; i < argc; i++) {
      // convert to C++ string
      string arg = argv[i];

      // check if the source file exists
      ifstream source(arg);
      if (!source.good()) { // ie. it does not exist (or some other problem was encountered)
        cerr << "The file '" << arg << "' cannot be opened. Skipping..." << endl;
        continue;
      }
      source.close();

      // check if the destination file exists
      ifstream dest(arg + ".mcb");
      if (dest.good()) { // ie. it already exists
        while (true) {
          cout << "The file '" << arg << ".mcb' already exists. Would you like to overwrite it? [y/n]: ";
          cout.flush();
          try {
            if (getBool()) {
              toAssemble.push_back(arg);
            } else {
              cout << "Skipping..." << endl;
            }
            break;
          } catch (range_error e) {
            continue;
          }
        }
      } else {
        toAssemble.push_back(arg);
      }
    }

    if (toAssemble.empty()) {
      cout << "No files remaining; exiting." << endl;
      return 1;
    }

    bool errorOccurred = false;
    for (vector<string>::iterator i = toAssemble.begin(); i != toAssemble.end(); i++) {
      errorOccurred |= !attemptAssembly(*i, *i + ".mcb");
    }
    return errorOccurred;
  }


  /* non-CLI */

  string sourcefile;
  while (true) {
    cout << "Please enter the name of a source code file to assemble (or leave blank to exit): ";
    cout.flush();
    getline(cin, sourcefile);

    if (sourcefile.empty()) {
      return 0;
    }

    ifstream sourcestream(sourcefile);
    if (!sourcestream.good()) { // ie. it does not exist (or some other problem was encountered)
      cerr << "The file '" << sourcefile << "' cannot be opened." << endl;
      continue;
    }
    sourcestream.close();
    break;
  }

  string destfile;
  while (true) {
    cout << "Please enter a target filename for the assembled program (or leave blank to exit): ";
    cout.flush();
    getline(cin, destfile);

    if (destfile.empty()) {
      return 0;
    }

    ifstream dest(destfile);
    if (dest.good()) { // ie. it already exists
      while (true) {
        cout << "The file '" << destfile << "' already exists. Would you like to overwrite it? [y/n]: ";
        cout.flush();
        try {
          if (!getBool()) {
            cout << "Exiting..." << endl;
            return 0;
          }
          break;
        } catch (range_error e) {
          continue;
        }
      }
    }
    break;
  }

  return !attemptAssembly(sourcefile, destfile);
}
