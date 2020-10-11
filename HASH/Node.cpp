#include <string>
#include <map>
#include <fstream>
#include <utility>
#include <experimental/filesystem>
using namespace std;

template <typename KeyType>
struct Node{
    map<KeyType,long> Bucket;
    string label;
    fstream file;

    int globalDepth;
    int size=0;
    int localDepth;

    Node(string label, int globalDepth, int localDepth):
    label(std::move(label)), globalDepth(globalDepth), localDepth(localDepth){
        if(experimental::filesystem::exists(label+".dat")){
            file.open(label+".dat",ios::binary |ios::in);
            KeyType tempKey;
            long tempPosition;
            while(file.read((char*)&tempKey,sizeof(KeyType))){
                file.read((char*)&tempPosition,sizeof(long));
                Bucket[tempKey]=tempPosition;
                ++size;
            }
            file.close();
        }
        else{
            file.open(label+".dat",ios::binary |ios::out);
            for(auto i:Bucket){
                file.write((char*)& i.first,sizeof(KeyType));
                file.write((char*)& i.second,sizeof(long));
                ++size;
                }
            }
            file.close();
        }
};
