#include <string>
#include <map>
#include <fstream>
#include <utility>
#include <experimental/filesystem>
using namespace std;

template <typename RecordType>
struct Bucket{
    string label;
    int globalDepth;
    int blockFactor;
    int size=0;

    Bucket(string label, int globalDepth, int blockFactor):
    label(std::move(label)), globalDepth(globalDepth), blockFactor(blockFactor){
        fstream file;
        if(experimental::filesystem::exists(label+".dat")){
            file.open(label+".dat",ios::binary |ios::in);
            file.read((char*)&size,sizeof(int));
            file.close();
        }
        else{
            file.open(label+".dat",ios::binary | ios::out);
            file.write((char *)size, sizeof(int));
            file.close();
        }
    }
};