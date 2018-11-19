#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <bitset>
#include <cstdint>
#include <vector>
#include <string>

using namespace std;

// Defines the exceptions that can be thrown during the assembly process.
enum AssemblerError {
  INSTRUCTION_NOT_FOUND,  // Invalid opcode
  UNEXPECTED_NUMERIC,     // Found numerical value where non-numerical value was expected
  UNEXPECTED_TOKEN,       // Found character where none was expected, or where a different type of character was expected.
  TOO_MANY_LABELS,        // Multiple labels defined for one location.
  UNDEFINED_LABEL,        // Label referenced but never defined
  MISSING_ARGUMENT,       // Missing argument for opcode that requires an argument
  DUPLICATE_LABEL         // One symbol defined multiple times
};


class BabyAssembler {

protected:
  string errVal; // the token at which the most recent AssemblerError occurred

  vector<string> readAssemblyFile(string fileName);
  uint8_t getInstruction(string instruction);

  // is part of the valid symbol character set
  bool isSymbolChar(char c) {
    return (c >= '0' && c <= '9')
        || (c >= 'A' && c <= 'Z')
        || (c == '_')
        || (c >= 'a' && c <= 'z');
  }
  // is whitespace, ':', or ';'
  bool isWordDelimiter(char c) { return (isspace(c) || c == ';' || c == ':'); }

public:
  string getErrVal() { return errVal; }

  bitset<32> babyBinary(uint32_t n); // converts n to a reversed bitset

  void assemble(string sourcefile, string destfile);

};

#endif
