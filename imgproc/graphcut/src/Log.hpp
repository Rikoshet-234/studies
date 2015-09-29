#ifndef LOG_HPP
#define LOG_HPP


#include <iostream>
#include <fstream>


#define LOG 1


#if LOG
extern std::ostream &Log;
#else
extern std::ofstream Log;
#endif


#endif
