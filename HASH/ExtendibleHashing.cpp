#include <bitset>
#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include "Bucket.cpp"
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

///////////////////////////////////////////////////

template <typename RecordType>
struct ExtendibleHash{
public:
    typedef typename RecordType::KeyType KeyType;
private:
    int globalDepth=0;
    int blockFactor=0;
    string hashFile;
    void writeIndixes(fstream &file){
        string index;
        for(int i=0;i<pow(2,globalDepth);++i){
            index=HashFunction(i,globalDepth);
            char array[globalDepth];
            strcpy(array, index.c_str());
            file.write((char*)&array,sizeof(char)*globalDepth);
        }
    }

public:
    explicit ExtendibleHash(string hashFile):hashFile(move(hashFile)){
        if(exists(hashFile)){
            ifstream file;
            file.open(hashFile,ios::binary |ios::in);
            file.read((char*)&globalDepth,sizeof(int));
            file.read((char*)&blockFactor,sizeof(int));
            file.close();
        }
        else{
            throw out_of_range("Este constuctor es para Hashes existentes, indique blockFactor y globalDepth inicial");
        }
    }


    ExtendibleHash(int globalDepth, int blockFactor,const string& hashFile) : globalDepth(globalDepth), blockFactor(blockFactor), hashFile(hashFile){
        if(!exists(hashFile)){
            fstream file;
            file.open(hashFile,ios::binary |ios::out);
            file.write((char*)&globalDepth,sizeof(int));
            file.write((char*)&blockFactor,sizeof(int));
            writeIndixes( file);
            file.close();
        }
        else{
            throw out_of_range("Este constuctor es para Hashes nuevos, solo cargue el archivo hash");
        }
    }


    RecordType* search(KeyType ID){
        string bucketLabel = HashFunction(ID,globalDepth);
        if(!exists(bucketLabel+".dat"))
            return nullptr;

        ifstream bucketFile;
        bucketFile.open(bucketLabel+".dat", ios::binary |ios::in);
        bucketFile.seekg(2 * sizeof(int), ios::beg);
        auto record = new RecordType();
        while(bucketFile.read((char*)&record,sizeof(RecordType)))
            if(record.ID==ID)
                return record;
        return nullptr;
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

    ~ExtendibleHash()=default;

};

int main(){
    ExtendibleHash<Record<int>> hash(3,3,"jungla.dat");
    return 0;
}