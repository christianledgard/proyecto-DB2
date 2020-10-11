#include <string>
#include <fstream>
using namespace std;

bool exists(const string& file){
    ifstream ifile;
    ifile.open(file);
    if(ifile)
        return true;
    else
        return false;
}
