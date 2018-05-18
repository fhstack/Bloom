#pragma once
#include <iostream>
#include <vector>
#include "BitSet.h"
using namespace std;
template <class K>
struct __HashFunc
{
    const K&  operator() (const K& key)
    {
        return key;
    }
};
struct HashFunc_1
{
    size_t BKDRHash(const char* str)  
    {  
        size_t hash = 0;  
        while (size_t ch = (size_t)*str++)  
        {         
            hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313..  
        }  
        return hash;  
    }
    size_t operator() (const char* str)
    {
    return BKDRHash(str);
    }
};
struct HashFunc_2
{
    size_t SDBMHash(const char *str)  
    {  
        size_t hash = 0;  
        while (size_t ch = (size_t)*str++)  
        {  
            hash = 65599 * hash + ch;         
        //hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;  
        }     
        return hash;  
    } 
    size_t operator () (const char* str)
    {
        return SDBMHash(str);
    }
};
struct HashFunc_3
{
    size_t RSHash(const char *str)
    {
        size_t hash = 0;
        size_t magic = 63689;
        while (size_t ch = (size_t)*str++)
        {
            hash = hash * magic + ch;
            magic *= 378551;
        }
        return hash;
    }
    size_t operator() (const char* str)
    {
        return RSHash(str);
    }
};
template <class K,class HashFunc1,class HashFunc2,class HashFunc3>
class BloomFilter
{
public:
   BloomFilter(size_t num)
       :_bt(num*5)
       ,_range(num*5)
   {}
   void Set(const K& key)
   {
     size_t hash1 = HashFunc1()(key);
     size_t hash2 = HashFunc2()(key);
     size_t hash3 = HashFunc3()(key);
     _bt.Set(hash1%_range);
     _bt.Set(hash2%_range);
     _bt.Set(hash3%_range);
   }
   bool Test(const K& key)
   {
       size_t hash1 = HashFunc1()(key);
       size_t hash2 = HashFunc2()(key);
       size_t hash3 = HashFunc3()(key);
       if(_bt.Test(hash1%_range) == false)
          return false;
       if(_bt.Test(hash2%_range) == false)
           return false;
       if(_bt.Test(hash3%_range) == false)
           return false;
       return true;
   }
protected:
    BitSet _bt;
    size_t _range;
};

void TestBloom()
{
    BloomFilter<const char*,HashFunc_1,HashFunc_2,HashFunc_3> bf(5);
    bf.Set("left");
    bf.Set("right");
    bf.Set("straight");
    cout<<bf.Test("right")<<endl;
    cout<<bf.Test("handler")<<endl;

}

