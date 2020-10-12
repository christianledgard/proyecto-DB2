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
        long prevDepth=globalDepth-localDepth;
        fstream hash;
        hash.open(hashFile,ios::binary |ios::out|ios::in);
        for(int i=0;i<pow(2,prevDepth);++i){
            string binaryPrev=HashFunction(i,prevDepth);
            string binaryNumber=binaryPrev+bucketName;
            unsigned long decimal = bitset<64>(binaryNumber).to_ulong();
            hash.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
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
        long counterOfZeros=0, counterOfOnes=0;
        while(bucket.read((char*)&record,sizeof(RecordType))){
            string hashValue= HashFunction(record.ID,extendedDepth);
            char labelOfRecord = hashValue[0];
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
        rename(oldname.c_str(),hashFile.c_str());
    }


    void rebuildColapse(){
        fstream file,temp;

        file.open(hashFile,ios::binary |ios::out |ios::in);
        temp.open("temp.dat",ios::binary |ios::out);
        --globalDepth;

        file.seekg(sizeof(long)*2,ios::beg);

        temp.write((char*)&globalDepth,sizeof(long));
        temp.write((char*)&blockFactor,sizeof(long));

        string index;
        long localDepth;

        for(int i=0;i<pow(2,globalDepth);++i){
            file.seekg(sizeof(char)*(globalDepth+1),ios::cur);
            file.read((char*)&localDepth,sizeof(long));

            index=HashFunction(i,globalDepth);
            char array[globalDepth];
            strcpy(array, index.c_str());

            temp.write((char*)&array,sizeof(char)*globalDepth);
            temp.write((char*)&localDepth,sizeof(long));
        }

        temp.close(); file.close();
        string oldname= "temp.dat";
        rename(oldname.c_str(),hashFile.c_str());
    }

    void safeAddAfterSplit(RecordType &record, string &hashValue, unsigned long decimal, fstream &file, long &localDepth,
                      string &bucketName) const {
        hashValue=HashFunction(record.ID, globalDepth);
        decimal = bitset<64>(hashValue).to_ulong();
        file.open(hashFile, ios::binary | ios::in);
        file.seekg(sizeof(long)*2 + (sizeof(char) * globalDepth + sizeof(long)) * decimal + sizeof(char) * globalDepth, ios::beg);
        file.read((char*)&localDepth,sizeof(long));
        file.close();

        fstream bucketFile2;
        bucketName= hashValue.substr(globalDepth - localDepth, globalDepth);
        bucketFile2.open(bucketName+".dat",ios::binary |ios::in| ios::out);
        long size2;
        bucketFile2.read((char*)&size2,sizeof(long));
        ++size2;
        bucketFile2.seekg(0,ios::beg);
        bucketFile2.write((char*)&size2,sizeof(long));
        bucketFile2.seekg((size2-1)*sizeof(RecordType),ios::cur);
        bucketFile2.write((char*)&record,sizeof(RecordType));
        bucketFile2.close();
    }

    pair<RecordType*,string>getRecordAndBuffer(KeyType ID){
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
                return make_pair(record,bucketName);
        return make_pair(nullptr,bucketName);
    }

    pair<bool,pair<string,string>> getcontextBuddy(const string&key){
        string hashValue=HashFunction(key,globalDepth);
        string buddyHashValue=getBuddy(hashValue);
        unsigned long decimal = bitset<64>(hashValue).to_ulong();
        unsigned long decimalBuddy = bitset<64>(buddyHashValue).to_ulong();
        fstream file;
        file.open(hashFile,ios::binary |ios::in);
        file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
        long localDepth,localDepthBuddy;
        file.read((char*)&localDepth,sizeof(long));
        file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
        file.read((char*)&localDepthBuddy,sizeof(long));
        file.close();

        string bucketName= hashValue.substr(globalDepth-localDepth,globalDepth);
        string bucketBuddyName= buddyHashValue.substr(globalDepth-localDepthBuddy,globalDepth);

        return make_pair(bucketName!=bucketBuddyName,make_pair(bucketName,bucketBuddyName));
    }

public:
    explicit ExtendibleHash(string hashFile):hashFile(move(hashFile)){
        if(existsFile(hashFile)){
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
        if(!existsFile(hashFile)){
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
        return getRecordAndBuffer(ID).first;
    }

    bool insertRecord(RecordType record){
        pair<RecordType*,string> recordAndBuffer=getRecordAndBuffer(record.ID);
        if(recordAndBuffer.first == nullptr){
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
                bucketFile.seekg((size-1)*sizeof(RecordType),ios::cur);
                bucketFile.write((char*)&record,sizeof(RecordType));
                bucketFile.close();
            }
            else{
                if(localDepth==globalDepth)
                    rebuildIndexFile();
                splitBucket(localDepth, bucketName);
                safeAddAfterSplit(record, hashValue, decimal, file, localDepth, bucketName);
            }
            return true;
        }
        else{
            return false;
        }
    }


    string getBuddy(const string& hashValue){
        string buddy=hashValue;
        if(buddy[0]=='0')
            buddy[0]='1';
        else
            buddy[0]='0';
        return buddy;
    }


    bool hasBuddy(const string& key){
        return getcontextBuddy(key).first;
    }

    bool collapse(){
        ///TODO COLLAPSE
        bool canColapse=false;
        if(globalDepth>1){
            canColapse=true;
            long halfSize= pow(2,globalDepth)/2;
            fstream file;
            file.open(hashFile,ios::binary |ios::in);

            char hashValue[globalDepth], buddyHashValue[globalDepth];
            for(long i=0;i<halfSize;++i){
                file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*i,ios::beg);
                long localDepth,localDepthBuddy;
                file.read((char*)&hashValue,+ sizeof(char)*globalDepth);
                file.read((char*)&localDepth,sizeof(long));
                file.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*i+pow(2,globalDepth-1),ios::beg);
                file.read((char*)&buddyHashValue,+ sizeof(char)*globalDepth);
                file.read((char*)&localDepthBuddy,sizeof(long));

                string bucketName= hashValue.substr(globalDepth-localDepth,globalDepth);
                string bucketBuddyName= buddyHashValue.substr(globalDepth-localDepthBuddy,globalDepth);

                if(bucketBuddyName!=bucketName){
                    canColapse= false;
                    return canColapse;
                }
            }
            rebuildColapse();
        }
        return canColapse;
    }


    void tryCombine(const string& key){
        auto contextBuddy=getcontextBuddy(key);
        if(contextBuddy.first){

            string bucketName,buddyBuckeyName;
            fstream bucketFile,buddyBucketFile;

            bucketFile.open(bucketName+".dat",ios::binary |ios::in| ios::out);
            buddyBucketFile.open(buddyBuckeyName+".dat",ios::binary |ios::in| ios::out);

            long size,sizeBuddy;
            bucketFile.read((char*)&size,sizeof(long));
            buddyBucketFile.read((char*)&sizeBuddy,sizeof(long));

            if(size+sizeBuddy<=blockFactor){
                ///TODO MERGE BUCKET
                if(collapse()){
                    tryCombine(key);
                }
            }
        }
    }

    void deleteRecordFromFile(const string& key){

    }

    bool removeRecord(KeyType key){
        pair<RecordType*,string> recordAndBuffer=getRecordAndBuffer(key);
        if(recordAndBuffer.first!=nullptr){
            ///TODO deleteRecordFromFile(key);
            tryCombine(key);
            return true;
        }
        else{
            return false;
        }
    }
    ~ExtendibleHash()=default;
};

int main(){
    remove("000.dat");
    remove("1.dat");
    remove("010.dat");
    remove("100.dat");
    remove("110.dat");
    remove("test.dat");
    ExtendibleHash<Record<long>> hash(1,3,"test.dat");
   // ExtendibleHash<Player<long>> hashPlayer(1,1,"player.dat");

    hash.insertRecord(Record<long>(16));
    hash.insertRecord(Record<long>(4));
    hash.insertRecord(Record<long>(6));
    hash.insertRecord(Record<long>(22));
    hash.insertRecord(Record<long>(24));
    hash.insertRecord(Record<long>(10));
    hash.insertRecord(Record<long>(31));
    hash.insertRecord(Record<long>(7));
    hash.insertRecord(Record<long>(9));
    hash.insertRecord(Record<long>(20));
    hash.insertRecord(Record<long>(26));
    return 0;
}

