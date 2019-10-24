#ifndef ALL_CHAR
#define ALL_CHAR

using std::string;

const string ALLSET("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#%'()*+,-./:;<=>\?[\\]^{|}_ \n\t\v\f~&");

#endif

#ifndef ESCAPED_CHAR
#define ESCAPED_CHAR
using std::set;

const set<char> ESCAPEDCHARS{ '.','|','*','(',')','+','?','{','}','[',']' };
#endif