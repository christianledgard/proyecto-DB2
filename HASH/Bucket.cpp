#include <string>
#include <map>
#include <fstream>
#include <utility>
#include "functions.cpp"
using namespace std;

template <typename RecordType>
struct Bucket{
    string label;
    long size=0;
    long header=-1;

    Bucket(const string& label, long globalDepth, long blockFactor):
    label(label){
        fstream file;
        if(existsFile(label + ".dat")){
            file.open(label+".dat",ios::binary |ios::in);
            file.read((char*)&size,sizeof(long));
            file.read((char*)&header,sizeof(long));
            file.close();
        }
        else{
            file.open(label+".dat",ios::binary | ios::out);
            file.write((char *)size, sizeof(long));
            file.write((char *)header, sizeof(long));
            file.close();
        }
    }
};