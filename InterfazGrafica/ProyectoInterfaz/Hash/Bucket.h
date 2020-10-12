#ifndef BUCKET_H
#define BUCKET_H

#include <string>
#include <map>
#include <fstream>
#include <utility>
using namespace std;

template <typename RecordType>
struct Bucket{
    string label;
    long size=0;

    Bucket(const string& label, long globalDepth, long blockFactor):
    label(label){
        fstream file;
        if(existsFile(label+".dat")){
            file.open(label+".dat",ios::binary |ios::in);
            file.read((char*)&size,sizeof(long));
            file.close();
        }
        else{
            file.open(label+".dat",ios::binary | ios::out);
            file.write((char *)size, sizeof(long));
            file.close();
        }
    }
    bool existsFile(const string& file){
    ifstream ifile;
    ifile.open(file);
    if(ifile)
        return true;
    else
        return false;
    }
};

#endif