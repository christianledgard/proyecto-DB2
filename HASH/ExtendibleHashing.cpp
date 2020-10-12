#include <bitset>
#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include <cstdio>
#include "Bucket.cpp"
#include "Record.cpp"
template <typename KeyType>
string HashFunction(KeyType key, long length){
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
    long globalDepth=0;
    long blockFactor=0;
    string hashFile;

    void writeIndixes(fstream &file, long depth){
        string index;
        for(int i=0;i<pow(2,globalDepth);++i){
            index=HashFunction(i,globalDepth);
            char array[globalDepth];
            strcpy(array, index.c_str());
            file.write((char*)&array,sizeof(char)*globalDepth);
            file.write((char*)&depth,sizeof(long));
        }
    }
    void createInitialBuckets() const {
        fstream zero,one;
        long zeroNumber=0;
        zero.open("0.dat",ios::binary |ios::out);
        one.open("1.dat",ios::binary |ios::out);
        zero.write((char*)&zeroNumber,sizeof(long));
        one.write((char*)&zeroNumber,sizeof(long));
        zero.close();
        one.close();
    }

    void createNewFiles(string const& bucketName){
        fstream zero,one;
        long zeroNumber=0;
        zero.open("0"+bucketName+".dat",ios::binary |ios::out);
        one.open("1"+bucketName+".dat",ios::binary |ios::out);
        zero.write((char*)&zeroNumber,sizeof(long));
        one.write((char*)&zeroNumber,sizeof(long));
        zero.close();
        one.close();
    }
    void updatePointersofIndixes(string const& bucketName, long localDepth){
        long prevDepth=globalDepth-(localDepth+1);
        fstream hash;
        hash.open(hashFile,ios::binary |ios::out);
        for(int i=0;i<pow(2,prevDepth);++i){
            string binaryPrev=HashFunction(i,prevDepth);
            string binaryNumber=binaryPrev+bucketName;
            unsigned long decimal = bitset<64>(binaryNumber).to_ulong();
            hash.seekg(sizeof(long)*2+(sizeof(globalDepth)+sizeof(long))*decimal+ sizeof(globalDepth),ios::beg);
            long newPointer=localDepth+1;
            hash.write((char*)& newPointer,sizeof(long));
        }
        hash.close();
    }
    void insertRecordsFromOldBucket(const string& bucketName, long extendedDepth){
        fstream bucket, bucketZero,bucketOne;
        bucket.open(bucketName+".dat", ios::binary | ios::in | ios::out);
        bucketZero.open("0"+bucketName+".dat", ios::binary | ios::in | ios::out);
        bucketOne.open("1"+bucketName+".dat", ios::binary | ios::in | ios::out);
        bucket.seekg(sizeof(long),ios::beg);
        bucketOne.seekg(sizeof(long),ios::beg);
        bucketZero.seekg(sizeof(long),ios::beg);
        RecordType record;
        int counterOfZeros=0, counterOfOnes=0;
        while(bucket.read((char*)&record,sizeof(RecordType))){
            string hashValue= HashFunction(record.ID,globalDepth);
            char labelOfRecord = hashValue[globalDepth-extendedDepth];
            if(labelOfRecord=='0'){
                bucketZero.write((char*)&record,sizeof(RecordType));
                ++counterOfZeros;
            }
            else{
                bucketOne.write((char*)&record,sizeof(RecordType));
                ++counterOfOnes;
            }
        }
        bucketOne.seekg(0,ios::beg);
        bucketZero.seekg(0,ios::beg);
        bucketZero.write((char*)&counterOfZeros,sizeof(long));
        bucketOne.write((char*)&counterOfOnes,sizeof(long));
        bucket.close(); bucketOne.close(); bucketZero.close();
    }

    void removeOldBucket(const string& bucketName){
        remove((bucketName + ".dat").c_str());
    }

    void splitBucket(long localDepth, const string &bucketName) {
        createNewFiles(bucketName);
        updatePointersofIndixes(bucketName, localDepth);
        insertRecordsFromOldBucket(bucketName, localDepth + 1);
        removeOldBucket(bucketName);
    }
    void rebuildIndexFile(){
        fstream file,temp;

        file.open(hashFile,ios::binary |ios::out |ios::in);
        temp.open("temp.dat",ios::binary |ios::out);
        ++globalDepth;

        temp.write((char*)&globalDepth,sizeof(long));
        temp.write((char*)&blockFactor,sizeof(long));

        char index[globalDepth-1];
        long localDepth=0;

        file.seekg(2*sizeof(long),ios::beg);

        for(int i=0;i<pow(2,globalDepth-1);++i){
            file.read((char*)&index,sizeof(char)*(globalDepth-1));
            file.read((char*)&localDepth,sizeof(long));
            char extended[globalDepth];
            extended[0]='0';
            for(int item=1;item<globalDepth;++item)
                extended[item]=index[item-1];
            temp.write((char*)&extended,sizeof(char)*globalDepth);
            temp.write((char*)&localDepth,sizeof(long));
        }

        file.seekg(2*sizeof(long),ios::beg);

        for(int i=0;i<pow(2,globalDepth-1);++i){
            file.read((char*)&index,sizeof(char)*(globalDepth-1));
            file.read((char*)&localDepth,sizeof(long));
            char extended[globalDepth];
            extended[0]='1';
            for(int item=1;item<globalDepth;++item)
                extended[item]=index[item-1];
            temp.write((char*)&extended,sizeof(char)*globalDepth);
            temp.write((char*)&localDepth,sizeof(long));
        }

        temp.close(); file.close();
        string oldname= "temp.dat";
        //remove(hashFile.c_str());
        rename(oldname.c_str(),hashFile.c_str());
    }


public:
    explicit ExtendibleHash(string hashFile):hashFile(move(hashFile)){
        if(exists(hashFile)){
            ifstream file;
            file.open(hashFile,ios::binary |ios::in);
            file.read((char*)&globalDepth,sizeof(long));
            file.read((char*)&blockFactor,sizeof(long));
            file.close();
        }
        else{
            throw out_of_range("Este constuctor es para Hashes existentes, indique blockFactor y globalDepth inicial");
        }
    }


    ExtendibleHash(long globalDepth, long blockFactor,const string& hashFile):
    globalDepth(globalDepth), blockFactor(blockFactor), hashFile(hashFile){
        if(!exists(hashFile)){
            fstream file;
            file.open(hashFile,ios::binary |ios::out);
            file.write((char*)&globalDepth,sizeof(long));
            file.write((char*)&blockFactor,sizeof(long));
            writeIndixes( file,1);
            file.close();
            createInitialBuckets();
        }
        else{
            throw out_of_range("Este constuctor es para Hashes nuevos, solo cargue el archivo hash");
        }
    }

    RecordType* searchRecord(KeyType ID){
        string bucketLabel = HashFunction(ID,globalDepth);
        unsigned long decimal = bitset<64>(bucketLabel).to_ulong();
        fstream file;
        file.open(hashFile,ios::binary |ios::in);
        file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
        long localDepth;
        file.read((char*)&localDepth,sizeof(long));
        file.close();

        string bucketName=  bucketLabel.substr(globalDepth-localDepth,globalDepth);
        fstream bucketFile;
        bucketFile.open(bucketName+".dat",ios::binary |ios::in| ios::out);
        bucketFile.seekg(sizeof(long), ios::beg);
        auto record = new RecordType();
        while(bucketFile.read((char*)&(*record),sizeof(RecordType)))
            if(record->ID==ID)
                return record;
        return nullptr;
    }

    bool insertRecord(RecordType record){
        if(searchRecord(record.ID) == nullptr){
            string hashValue=HashFunction(record.ID,globalDepth);
            unsigned long decimal = bitset<64>(hashValue).to_ulong();
            fstream file;
            file.open(hashFile,ios::binary |ios::in);
            file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
            long localDepth;
            file.read((char*)&localDepth,sizeof(long));
            file.close();

            string bucketName= hashValue.substr(globalDepth-localDepth,globalDepth);
            fstream bucketFile;
            bucketFile.open(bucketName+".dat",ios::binary |ios::in| ios::out);

            long size;
            bucketFile.read((char*)&size,sizeof(long));

            if(size<blockFactor){
                ++size;
                bucketFile.seekg(0,ios::beg);
                bucketFile.write((char*)&size,sizeof(long));
                bucketFile.seekg(size*sizeof(RecordType),ios::cur);
                bucketFile.write((char*)&record,sizeof(RecordType));
            }
            else{
                if(localDepth==globalDepth){
                    rebuildIndexFile();
                //splitBucket(localDepth, bucketName);
                }
            }
            return true;
        }
        else{
            return false;
        }
    }

    bool removeRecord(KeyType key){
//        if(searchRecord(record.ID)==nullptr){
//            ///TODO REMOVE LOGIC
//            return true;
//        }
//        else{
//            return false;
//        }
    }
    ~ExtendibleHash()=default;
};

int main(){
    remove("00.dat");
    remove("0.dat");
    remove("10.dat");
    remove("1.dat");
    remove("temp.dat");
    remove("test.dat");
    ExtendibleHash<Record<long>> hash(1,3,"test.dat");
    hash.insertRecord(Record<long>(16));
    hash.insertRecord(Record<long>(4));
    hash.insertRecord(Record<long>(6));
    hash.insertRecord(Record<long>(22));
    string temp="temp.dat";
//    hash.insertRecord(Record<long>(24));
//    hash.insertRecord(Record<long>(10));
//    hash.insertRecord(Record<long>(31));
//    hash.insertRecord(Record<long>(7));
//    hash.insertRecord(Record<long>(9));
//    hash.insertRecord(Record<long>(20));
//    hash.insertRecord(Record<long>(26));
    return 0;
}