#include <string>
#include <map>
#include <fstream>
#include <utility>
#include "functions.cpp"
using namespace std;

template <typename RecordType>
struct Bucket{
    string label;
    int size=0;
    int localDepth;

    Bucket(string label, int globalDepth, int blockFactor):
    label(std::move(label)){
        fstream file;
        if(exists(label+".dat")){
            file.open(label+".dat",ios::binary |ios::in);
            file.read((char*)&size,sizeof(int));
            file.read((char*)&localDepth,sizeof(int));
            file.close();
        }
        else{
            file.open(label+".dat",ios::binary | ios::out);
            localDepth=1;
            file.write((char *)size, sizeof(int));
            file.write((char *)localDepth, sizeof(int));
            file.close();
        }
    }
};