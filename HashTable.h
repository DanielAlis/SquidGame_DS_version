//
// Created by Alis Daniel.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H
/***
 * description:
 * a hash table data-structure, using chain-addressing, while the size varies.
 * size of table : 2^k-1
 * keeping load_factor = n/m <= 2
 * hash function based on constant GOLDEN_RATIO = (sqrt(5) -1)/2
 *
 * restrictions on type KEY:
 * assume it has no parameter c'tor.
 * assume it has operator ==.
 *
 * restriction on type VALUE:
 * assume it has operator() that return int which is the hash for the given KEY.
 *
 *
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include "DList.h"
#ifndef GOLDEN_RATIO
#define GOLDEN_RATIO 0.618033988
#endif // GOLDEN_RATIO


inline int hashCalculate(int x, int len)
{
    return floor(len*(fmod(x, GOLDEN_RATIO)));
}
template <typename KEY, typename VALUE>
class HashTable {
    DList<VALUE>* data;
    DList<KEY>* key_data;
    int len;
    int elem_num;
    void resize();
    /** FACTOR:
     * the size the arrays grow when alpha is no longer <= 2
     */
    static const int FACTOR = 2;
    static const int LOAD_FACTOR = 2;
    inline bool isTooManyElem() const;
    inline bool isTooLittleElem() const;
    int (*hashFunc)(const KEY&);
public:


    explicit HashTable(int (*hashFunc)(const KEY&));
    ~HashTable();
    HashTable(const HashTable& src) = delete;
    HashTable& operator= (const HashTable& src) = delete;
    const VALUE& operator[] (const KEY& key) const;
    VALUE& operator[](const KEY& key);
    void insert(const KEY& key, const VALUE& value);// increasing size by ((size+1)*FACTOR)-1

    void remove(const KEY& key);
    //exception classes:
    class keyNotFound : public std::exception{
    public:
        const char* what() const noexcept override{
            return "HashTable error: Key not found";
        }
    };

    class keyAlreadyExists : public std::exception{
    public:
        const char* what() const noexcept override{
            return "HashTable error: The key already exist in the table.";
        }
    };
    int getSize() const{
        return this->elem_num;
    }
};

/*
template <typename KEY, typename VALUE>
const double HashTable<KEY,VALUE>::GOLDEN_RATIO  = 0.618033988;
*/

template <typename KEY, typename VALUE>
inline bool HashTable<KEY,VALUE>::isTooManyElem() const
{
    return (double)elem_num/len >= (double)LOAD_FACTOR;
}

template <typename KEY, typename VALUE>
inline bool HashTable<KEY,VALUE>::isTooLittleElem() const
{
    return (double)elem_num/len <= 1/((double)FACTOR*FACTOR);
}




template <typename KEY, typename VALUE>
HashTable<KEY,VALUE>::HashTable(int (*hashFunc)(const KEY&)) :
        data(new DList<VALUE>[FACTOR*FACTOR-1]), key_data(new DList<KEY>[FACTOR*FACTOR-1]),
        len(FACTOR*FACTOR-1),elem_num(0), hashFunc(hashFunc) {}

template<typename KEY, typename VALUE>
HashTable<KEY,VALUE>::~HashTable()
{
    delete[] data;
    delete[] key_data;
}

template <typename KEY, typename VALUE>
const VALUE& HashTable<KEY,VALUE>::operator[] (const KEY& key) const
{
    int hash = hashCalculate(hashFunc(key), len);
    assert(hash<len && hash>=0);
    auto iter_value = data[hash].constBegin();
    auto iter_key = key_data[hash].constBegin();
    for( ;iter_value != data[hash].constEnd() && iter_key != key_data[hash].constEnd(); ++iter_key, ++iter_value){
        if(iter_key.getData() == key){
            return iter_value.getData();
        }
    }
    throw keyNotFound();
}

template <typename KEY, typename VALUE>
VALUE& HashTable<KEY,VALUE>::operator[](const KEY& key)
{
    return const_cast<VALUE&>(const_cast<const HashTable<KEY,VALUE>* > (this)->operator[](key));
}

template <typename KEY, typename VALUE>
void HashTable<KEY,VALUE>::insert(const KEY& key, const VALUE& value)
{
    //search if key exists
    int hash = hashCalculate(hashFunc(key), len);
    assert(hash<len && hash>=0);
    for(auto iter_key = key_data[hash].constBegin(); iter_key != key_data[hash].constEnd(); ++iter_key){
        if(iter_key.getData() == key){
            throw HashTable<KEY,VALUE>::keyAlreadyExists();
        }
    }
    //insert to the correct place
    data[hash].insertFirst(value);
    key_data[hash].insertFirst(key);
    elem_num++;
    if(isTooManyElem()){
        resize();
    }
}

template <typename KEY, typename VALUE>
void HashTable<KEY,VALUE>::resize()
{

    int new_size = ((len+1)*FACTOR)-1;
    if(isTooLittleElem()){
        new_size = ((len+1)/FACTOR)-1;
        assert(new_size>0);
    }
    DList<VALUE>* temp_data = new DList<VALUE>[new_size];
    DList<KEY>* temp_key_arr = new DList<KEY>[new_size];
    for(int i=0; i<len; ++i){
        auto data_iter = data[i].begin();
        auto key_iter = key_data[i].begin();
        for(; data_iter != data[i].end() && key_iter != key_data[i].end(); ++data_iter, ++key_iter){
            int hash = hashCalculate(hashFunc(key_iter.getData()), new_size);
            temp_data[hash].insertFirst(data_iter.getData());
            temp_key_arr[hash].insertFirst(key_iter.getData());
        }
    }
    delete[] data;
    delete[] key_data;
    data = temp_data;
    key_data = temp_key_arr;
    len = new_size;
}

template <typename KEY, typename VALUE>
void HashTable<KEY,VALUE>::remove(const KEY& key)
{
    int hash = hashCalculate(hashFunc(key), len);
    auto iter_value = data[hash].begin();
    auto iter_key = key_data[hash].begin();
    for( ;iter_key != key_data[hash].end(); ++iter_key, ++iter_value){
        assert(iter_value != data[hash].end());
        if(iter_key.getData() == key){
            key_data[hash].deleteNode(*iter_key);
            data[hash].deleteNode(*iter_value);
            elem_num--;
            if(isTooLittleElem() && len>(FACTOR*FACTOR-1)) // len check so to not get less then base length.
            {
                resize();
            }
            return;
        }
    }
    throw HashTable<KEY,VALUE>::keyNotFound();
}



#endif //HASHTABLE_H
