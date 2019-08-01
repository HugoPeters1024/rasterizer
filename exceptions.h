#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <sstream>

class ShaderMissingException : public std::exception 
{
private:
  std::string msg;

public:
   ShaderMissingException(const char* filename) {
     std::stringstream ss;
     ss << "Could not open file for shader compilation: " << filename;
     msg = ss.str(); 
   }

   virtual const char* what() const throw()
   {
     return msg.c_str();
   }
};

#endif
