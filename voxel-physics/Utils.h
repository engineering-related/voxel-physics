#ifndef UTILS
#define UTILS

#include <string>
#include <regex>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include <sstream>
#include <stdexcept>
#include <fstream>

#define GET_VARIABLE_NAME(var) std::string((#var))
#define GET_TYPE_NAME(in) std::string(typeid(in).name())

namespace engine {

// Reads a text file and outputs a string with everything in the text file
inline std::string getFileContents(const std::string &fileName) {
  std::ifstream in(fileName, std::ios::binary);
  if (!in)
    throw std::runtime_error("Could not read file: " + fileName);

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();
  return contents;
}

inline std::vector<std::string> matchAll(const std::string& s, const std::regex& exp) {
  std::vector<std::string> matches;
  std::smatch res;

  std::string::const_iterator searchStart( s.cbegin() );
  while ( regex_search( searchStart, s.cend(), res, exp ) )
  {
    matches.push_back(res[0]);
    searchStart = res.suffix().first;
  }
  return matches;
}
}

#endif // !UTILS 