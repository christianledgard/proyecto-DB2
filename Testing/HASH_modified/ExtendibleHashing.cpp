#include <bitset>
#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include <chrono>
#include <cstdio>
#include <vector>
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
        long zeroNumber=0,minus=-1;
        zero.open("0.dat",ios::binary |ios::out);
        one.open("1.dat",ios::binary |ios::out);
        zero.write((char*)&zeroNumber,sizeof(long));
        one.write((char*)&zeroNumber,sizeof(long));
        zero.write((char*)&minus,sizeof(long));
        one.write((char*)&minus,sizeof(long));
        zero.close();
        one.close();
    }
    void createNewFiles(string const& bucketName,long & accesosADisco){
        fstream zero,one;
        long zeroNumber=0,minus=-1;
        zero.open("0"+bucketName+".dat",ios::binary |ios::out);
        one.open("1"+bucketName+".dat",ios::binary |ios::out);
        zero.write((char*)&zeroNumber,sizeof(long));
        ++accesosADisco;
        one.write((char*)&zeroNumber,sizeof(long));
        ++accesosADisco;
        zero.write((char*)&minus,sizeof(long));
        ++accesosADisco;
        one.write((char*)&minus,sizeof(long));
        ++accesosADisco;
        zero.close();
        one.close();
    }
    void updatePointersOfIndixes(string const& bucketName, long localDepth,long & accesosADisco){
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
            ++accesosADisco;
        }
        hash.close();
    }

    void insertRecordsFromOldBucket(const string& bucketName, long extendedDepth,long & accesosADisco){
        fstream bucket, bucketZero,bucketOne;
        bucket.open(bucketName+".dat", ios::binary | ios::in | ios::out);
        bucketZero.open("0"+bucketName+".dat", ios::binary | ios::in | ios::out);
        bucketOne.open("1"+bucketName+".dat", ios::binary | ios::in | ios::out);
        bucket.seekg(sizeof(long)*2,ios::beg);
        bucketOne.seekg(sizeof(long)*2,ios::beg);
        bucketZero.seekg(sizeof(long)*2,ios::beg);
        RecordType record;
        long counterOfZeros=0, counterOfOnes=0;
        while(bucket.read((char*)&record,sizeof(RecordType))){
            ++accesosADisco;
            if(record.prevDelete==-2){
                string hashValue= HashFunction(record.ID,extendedDepth);
                char labelOfRecord = hashValue[0];
                if(labelOfRecord=='0'){
                    bucketZero.write((char*)&record,sizeof(RecordType));
                    ++accesosADisco;
                    ++counterOfZeros;
                }
                else{
                    bucketOne.write((char*)&record,sizeof(RecordType));
                    ++accesosADisco;
                    ++counterOfOnes;
                }
            }
        }
        bucketOne.seekg(0,ios::beg);
        bucketZero.seekg(0,ios::beg);
        long minus=-1;
        accesosADisco+=4;
        bucketZero.write((char*)&counterOfZeros,sizeof(long));
        bucketOne.write((char*)&counterOfOnes,sizeof(long));
        bucketZero.write((char*)&minus,sizeof(long));
        bucketOne.write((char*)&minus,sizeof(long));
        bucket.close(); bucketOne.close(); bucketZero.close();
    }

    void removeOldBucket(const string& bucketName){
        remove((bucketName + ".dat").c_str());
    }

    void splitBucket(long localDepth, const string &bucketName,long & accesosADisco) {
        createNewFiles(bucketName,accesosADisco);
        updatePointersOfIndixes(bucketName, localDepth,accesosADisco);
        insertRecordsFromOldBucket(bucketName, localDepth + 1,accesosADisco);
        removeOldBucket(bucketName);
    }

    void rebuildIndexFile(long & accesosADisco){
        fstream file,temp;

        file.open(hashFile,ios::binary |ios::out |ios::in);
        temp.open("temp.dat",ios::binary |ios::out);
        ++globalDepth;

        temp.write((char*)&globalDepth,sizeof(long));
        temp.write((char*)&blockFactor,sizeof(long));
        ++accesosADisco;
        ++accesosADisco;

        char index[globalDepth-1];
        long localDepth=0;

        file.seekg(2*sizeof(long),ios::beg);

        for(int i=0;i<pow(2,globalDepth-1);++i){
            file.read((char*)&index,sizeof(char)*(globalDepth-1));
            ++accesosADisco;
            file.read((char*)&localDepth,sizeof(long));
            ++accesosADisco;
            char extended[globalDepth];
            extended[0]='0';
            for(int item=1;item<globalDepth;++item)
                extended[item]=index[item-1];
            temp.write((char*)&extended,sizeof(char)*globalDepth);
            ++accesosADisco;
            temp.write((char*)&localDepth,sizeof(long));
            ++accesosADisco;
        }

        file.seekg(2*sizeof(long),ios::beg);

        for(int i=0;i<pow(2,globalDepth-1);++i){
            file.read((char*)&index,sizeof(char)*(globalDepth-1));
            ++accesosADisco;
            file.read((char*)&localDepth,sizeof(long));
            ++accesosADisco;
            char extended[globalDepth];
            extended[0]='1';
            for(int item=1;item<globalDepth;++item)
                extended[item]=index[item-1];
            temp.write((char*)&extended,sizeof(char)*globalDepth);
            ++accesosADisco;
            temp.write((char*)&localDepth,sizeof(long));
            ++accesosADisco;
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
                      string &bucketName, long & accesosADisco){
        hashValue=HashFunction(record.ID, globalDepth);
        decimal = bitset<64>(hashValue).to_ulong();
        file.open(hashFile, ios::binary | ios::in);
        file.seekg(sizeof(long)*2 + (sizeof(char) * globalDepth + sizeof(long)) * decimal + sizeof(char) * globalDepth, ios::beg);
        ++accesosADisco;
        file.read((char*)&localDepth,sizeof(long));
        file.close();

        fstream bucketFile2;
        bucketName= hashValue.substr(globalDepth - localDepth, globalDepth);
        bucketFile2.open(bucketName+".dat",ios::binary |ios::in| ios::out);
        long size2;
        bucketFile2.read((char*)&size2,sizeof(long));
        ++accesosADisco;
        ++size2;
        bucketFile2.seekg(0,ios::beg);
        bucketFile2.write((char*)&size2,sizeof(long));
        ++accesosADisco;
        long header;
        bucketFile2.read((char*)&header,sizeof(long));
        ++accesosADisco;
        if(header!=-1)
            size2=header;
        else
            --size2;
        bucketFile2.seekg((size2)*sizeof(RecordType),ios::cur);
        bucketFile2.write((char*)&record,sizeof(RecordType));
        ++accesosADisco;
        bucketFile2.close();
    }

    pair<RecordType*,string>getRecordAndBuffer(KeyType ID, long & accesosADisco){
        string bucketLabel = HashFunction(ID,globalDepth);
        unsigned long decimal = bitset<64>(bucketLabel).to_ulong();
        fstream file;
        file.open(hashFile,ios::binary |ios::in);
        file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
        long localDepth;
        file.read((char*)&localDepth,sizeof(long));
        ++accesosADisco;
        file.close();

        string bucketName=  bucketLabel.substr(globalDepth-localDepth,globalDepth);
        fstream bucketFile;
        bucketFile.open(bucketName+".dat",ios::binary |ios::in| ios::out);
        bucketFile.seekg(sizeof(long)*2, ios::beg);
        auto record = new RecordType();
        while(bucketFile.read((char*)&(*record),sizeof(RecordType))){
            ++accesosADisco;
            if(record->ID==ID and record->prevDelete==-2)
                return make_pair(record,bucketName);
        }
        return make_pair(nullptr,bucketName);
    }

    pair<bool,pair<string,string>> getcontextBuddy(KeyType key){
        string hashValue=HashFunction(key,globalDepth);
        string buddyHashValue=getBuddy(hashValue);
        unsigned long decimal = bitset<64>(hashValue).to_ulong();
        unsigned long decimalBuddy = bitset<64>(buddyHashValue).to_ulong();
        fstream file;
        file.open(hashFile,ios::binary |ios::in);
        file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
        long localDepth,localDepthBuddy;
        file.read((char*)&localDepth,sizeof(long));
        file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimalBuddy+ sizeof(char)*globalDepth,ios::beg);
        file.read((char*)&localDepthBuddy,sizeof(long));
        file.close();

        string bucketName= hashValue.substr(globalDepth-localDepth,globalDepth);
        string bucketBuddyName= buddyHashValue.substr(globalDepth-localDepthBuddy,globalDepth);

        return make_pair(bucketName!=bucketBuddyName,make_pair(bucketName,bucketBuddyName));
    }

    void mergeBuckets(const string& bucket,const string& buddy){
        string sufix= bucket.substr(1,buddy.length());
        fstream newBucket, childOne, childTwo;
        long minus=-1;
        newBucket.open(sufix+".dat", ios::out);
        childOne.open(bucket+".dat", ios::binary | ios::in | ios::out);
        childTwo.open(buddy+".dat", ios::binary | ios::in | ios::out);

        long size1,size2,totalSize;
        childOne.read((char*)&size1,sizeof(long));
        childTwo.read((char*)&size2,sizeof(long));
        childOne.seekg(sizeof(long),ios::cur);
        childTwo.seekg(sizeof(long),ios::cur);
        totalSize=size1+size2;
        newBucket.write((char*)&totalSize,sizeof(long));
        newBucket.write((char*)&minus,sizeof(long));
        RecordType record;
        while(childOne.read((char*)& record,sizeof(RecordType))){
            if(record.prevDelete==-2){
                newBucket.write((char*)&record,sizeof(RecordType));
            }
        }

        while(childTwo.read((char*)& record,sizeof(RecordType))){
            if(record.prevDelete==-2){
                newBucket.write((char*)&record,sizeof(RecordType));
            }
        }


        fstream hash;
        hash.open(hashFile,ios::binary|ios::in|ios::out);

        long newPointer=(long)bucket.length()-1;

        long newDepth=globalDepth-sufix.length();
        for(long i=0;i<=pow(2,globalDepth-bucket.length());++i){
            string binaryPrev=HashFunction(i,newDepth);
            string binaryNumber=binaryPrev+sufix;
            unsigned long decimal = bitset<64>(binaryNumber).to_ulong();
            hash.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*decimal+sizeof(char)*globalDepth,ios::beg);
            hash.write((char*)& newPointer,sizeof(long));
        }
        newBucket.close();
        removeOldBucket(bucket);
        removeOldBucket(buddy);
        hash.close();
    }

    string getBuddy(const string& hashValue){
        string buddy=hashValue;
        if(buddy[0]=='0')
            buddy[0]='1';
        else
            buddy[0]='0';
        return buddy;
    }

    bool collapse(){
        bool canColapse=false;
        if(globalDepth>1){
            canColapse=true;
            long halfSize= pow(2,globalDepth)/2;
            fstream file;
            file.open(hashFile,ios::binary |ios::in);

            char hashValue[globalDepth], buddyHashValue[globalDepth];
            for(long i=0;i<halfSize;++i){
                file.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*i,ios::beg);
                long localDepth,localDepthBuddy;
                file.read((char*)&hashValue,+ sizeof(char)*globalDepth);
                file.read((char*)&localDepth,sizeof(long));
                file.seekg(sizeof(long)*2+(sizeof(char)*globalDepth+sizeof(long))*(i+pow(2,globalDepth-1)),ios::beg);
                file.read((char*)&buddyHashValue,+ sizeof(char)*globalDepth);
                file.read((char*)&localDepthBuddy,sizeof(long));

                string hashString,buddyHashString;
                for(auto it:hashValue)
                    hashString+=it;

                for(auto it:buddyHashValue)
                    buddyHashString+=it;

                string bucketName= hashString.substr(globalDepth-localDepth,globalDepth);
                string bucketBuddyName= buddyHashString.substr(globalDepth-localDepthBuddy,globalDepth);

                if(bucketBuddyName!=bucketName){
                    canColapse= false;
                    return canColapse;
                }
            }
            rebuildColapse();
        }
        return canColapse;
    }

    void tryCombine(KeyType key,string& buffer){
        auto contextBuddy=getcontextBuddy(key);
        if(contextBuddy.first){

            string buddyBucketName=getBuddy(buffer);
            fstream bucketFile,buddyBucketFile;

            bucketFile.open(buffer+".dat",ios::binary |ios::in| ios::out);
            buddyBucketFile.open(buddyBucketName + ".dat", ios::binary | ios::in | ios::out);

            long size,sizeBuddy;
            bucketFile.read((char*)&size,sizeof(long));
            buddyBucketFile.read((char*)&sizeBuddy,sizeof(long));
            bucketFile.close();
            buddyBucketFile.close();
            if(size+sizeBuddy<=blockFactor){
                mergeBuckets(buffer,buddyBucketName);
                if(collapse()){
                    buffer = buffer.substr(1,buffer.length());
                    tryCombine(key,buffer);
                }
            }
        }
    }

    void deleteRecordFromFile(KeyType key,const string &buffer){
        fstream bucketFile;
        bucketFile.open(buffer+".dat",ios::binary |ios::in| ios::out);
        long size,header;
        bucketFile.read((char*)&size,sizeof(long));
        bucketFile.read((char*)&header,sizeof(long));
        auto record = new RecordType();
        long i;
        for(i=0;i<size;++i){
            bucketFile.read((char*)&(*record),sizeof(RecordType));
            if(record->ID==key)
                break;
        }
        bucketFile.seekg(sizeof(long)*2+sizeof(RecordType)*i+sizeof(KeyType),ios::beg);
        bucketFile.write((char*)&header,sizeof(long));
        bucketFile.seekg(sizeof(long),ios::beg);
        bucketFile.write((char*)&i,sizeof(long));
        --size;
        bucketFile.seekg(0,ios::beg);
        bucketFile.write((char*)&size,sizeof(long));
        bucketFile.close();
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

    long searchRecord(KeyType ID){
        long accesosADisco=0;
        getRecordAndBuffer(ID,accesosADisco);
        return accesosADisco;
    }

    long insertRecord(RecordType record, bool print){
        long accesosADisco=0;
        pair<RecordType*,string> recordAndBuffer=getRecordAndBuffer(record.ID, accesosADisco);
        if(recordAndBuffer.first == nullptr){
            string hashValue=HashFunction(record.ID,globalDepth);
            unsigned long decimal = bitset<64>(hashValue).to_ulong();
            fstream file;
            file.open(hashFile,ios::binary |ios::in);
            file.seekg(sizeof(long)*2+( sizeof(char)*globalDepth+sizeof(long))*decimal+ sizeof(char)*globalDepth,ios::beg);
            long localDepth;
            ++accesosADisco;
            file.read((char*)&localDepth,sizeof(long));
            file.close();

            string bucketName= hashValue.substr(globalDepth-localDepth,globalDepth);
            fstream bucketFile;
            bucketFile.open(bucketName+".dat",ios::binary |ios::in| ios::out);

            long size,header;
            ++accesosADisco;
            ++accesosADisco;
            bucketFile.read((char*)&size,sizeof(long));
            bucketFile.read((char*)&header,sizeof(long));

            if(size<blockFactor){
                ++size;
                bucketFile.seekg(0,ios::beg);
                ++accesosADisco;
                bucketFile.write((char*)&size,sizeof(long));
                bucketFile.seekg(sizeof(long),ios::cur);
                --size;
                if(header!=-1){
                    RecordType lastRemoved;
                    bucketFile.seekg((header)*sizeof(RecordType),ios::cur);
                    bucketFile.read((char*)&lastRemoved,sizeof(RecordType));
                    ++accesosADisco;
                    bucketFile.seekg(sizeof(long),ios::beg);
                    bucketFile.write((char*)&lastRemoved.prevDelete,sizeof(long));
                    ++accesosADisco;
                    bucketFile.seekg(header*sizeof(RecordType)+sizeof(long)*2,ios::beg);
                    bucketFile.write((char*)&record,sizeof(RecordType));
                    ++accesosADisco;
                }
                else{
                    bucketFile.seekg((size)*sizeof(RecordType),ios::cur);
                    bucketFile.write((char*)&record,sizeof(RecordType));
                    ++accesosADisco;
                    bucketFile.close();
                }
            }
            else{
                if(localDepth==globalDepth)
                    rebuildIndexFile(accesosADisco);
                splitBucket(localDepth, bucketName,accesosADisco);
                safeAddAfterSplit(record, hashValue, decimal, file, localDepth, bucketName,accesosADisco);
            }
            if(print)
                cout<<accesosADisco<<endl;
            return accesosADisco;
        }
        else{
            if(print)
                cout<<accesosADisco<<endl;
            return accesosADisco;
        }
    }

    bool removeRecord(KeyType key){
        pair<RecordType*,string> recordAndBuffer=getRecordAndBuffer(key);
        if(recordAndBuffer.first!=nullptr){
            deleteRecordFromFile(key,recordAndBuffer.second);
            tryCombine(key,recordAndBuffer.second);
            return true;
        }
        else{
            return false;
        }
    }

    ~ExtendibleHash()=default;
};




void write_csv(std::string filename, std::vector<long> vals){
    std::ofstream myFile(filename);
    for(long i = 0; i < vals.size(); ++i)
        myFile << vals.at(i) << "\n";
    myFile.close();
}



int main(){
    remove("000.dat");
    remove("1.dat");
    remove("0.dat");
    remove("10.dat");
    remove("010.dat");
    remove("100.dat");
    remove("110.dat");
    remove("test.dat");
    ExtendibleHash<Player<long>> hash(1,1,"test.dat");

    vector<long> time_for_search;

    for(long i=0;i<10000;++i){
        hash.insertRecord(Player<long>(i/2,"APELLIDO" ,"EQUIPO","POSICION",i,i,i,i,i),false);
    }

    for(long i=0;i<10000;++i){
        auto t1 = std::chrono::high_resolution_clock::now();
        hash.searchRecord(i/2);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        time_for_search.push_back(duration);
    }
    write_csv("time_search_hash.csv", time_for_search);


//    hash.insertRecord(Record<long>(16),true);
//    hash.insertRecord(Record<long>(24),true);
//    hash.insertRecord(Record<long>(10),true);
//    hash.insertRecord(Record<long>(26),true);
//    hash.insertRecord(Record<long>(6),true);
//    hash.insertRecord(Record<long>(22),true);
//    hash.insertRecord(Record<long>(31),true);
//    hash.insertRecord(Record<long>(7),true);



    return 0;
}