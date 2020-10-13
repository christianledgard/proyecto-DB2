#include <string>
#include <fstream>
using namespace std;

bool existsFile(const string& file){
    ifstream ifile;
    ifile.open(file);
    if(ifile)
        return true;
    else
        return false;
}
