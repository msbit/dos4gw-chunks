#ifndef __EXTRACT_H__
#define __EXTRACT_H__

#include <fstream>
#include <ostream>

bool extract_mz(std::ifstream &, std::ostream *); 
bool extract_bw(std::ifstream &, std::ostream *); 
bool extract_mz_le(std::ifstream &, std::ostream *); 

#endif
