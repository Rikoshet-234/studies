#include "Log.hpp"


#if LOG
std::ostream &Log = std::cout;
#else
std::ofstream Log("/dev/null");
#endif
