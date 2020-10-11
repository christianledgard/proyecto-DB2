#include <bitset>
#include <iostream>

#include "Node.cpp"
#include "Record.cpp"

template <typename KeyType>
string HashFunction(KeyType key, int length){
    string binaryRepresentation= bitset<64>(key).to_string();
    if(length>64){
        string zeroExtension;
        for(auto i=64;i<length;++i)
            zeroExtension+="0";
        return zeroExtension+binaryRepresentation;
    }
    else{
        return binaryRepresentation.substr(64-length,length);
    }
}



template <typename RecordType>
struct ExtendibleHash{

public:
    typedef typename RecordType::KeyType KeyType;
private:
    int globalDepth;
    int blockFactor;
    string hashFile;

public:
    ExtendibleHash(int globalDepth, int blockFactor,string hashFile) : globalDepth(globalDepth), blockFactor(blockFactor), hashFile(move(hashFile)){
    }

    RecordType* search(KeyType ID){
        string bucketLabel = HashFunction(ID,globalDepth);
        ifstream bucketFile;
        bucketFile.open(bucketLabel+".dat", ios::binary |ios::in);
        KeyType read; long logicalPosition; bool flag=false;
        while(bucketFile.read((char*)&read,sizeof(KeyType))){
            bucketFile.read((char*)&logicalPosition,sizeof(long));
            if(read==ID){
                flag=true;
                break;
            }
        }
        if(flag){
            auto record = new RecordType();
            ifstream datafile;
            datafile.open("datafile.dat", ios::binary |ios::in);
            datafile.seekg(logicalPosition*sizeof(RecordType),ios::beg);
            datafile.read((char*)&record,sizeof(RecordType));
            return record;
        }
        else{
            return nullptr;
        }
    }

    bool insert(RecordType record){
        if(search(RecordType::rec)==nullptr){
            ///TODO INSERT LOGIC
            return true;
        }
        else{
            return false;
        }
    }

    bool remove(KeyType key){
        if(search(RecordType::rec)== nullptr){
            ///TODO REMOVE LOGIC
            return true;
        }
        else{
            return false;
        }
    }

};



int main(){
    std::cout<<HashFunction(15,64);
    return 0;

}