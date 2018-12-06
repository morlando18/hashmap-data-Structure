
#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <assert.h>

using namespace std;

//const int tableSize = 11;



template <class K, class V>
struct HashNode
{
    K key;
    V value;
    HashNode * next;
    HashNode()
    {
        key = K();
        value = V();
        next = NULL;
    }
    HashNode(const K& kkey, const V& vvalue)
    {
        key = kkey;
        value = vvalue;
        next = NULL;
    }
};

template <class K, class V>
class HashMap{
    friend class Iterator;
private:
    HashNode<K,V>** table;
    HashNode<K,V>* ptrToCurTableIndex;

    int bucketCounts;
    int hashFunction (const K& key)
    {
        int hashFunc = std::hash<K>()(key) % bucketCounts;
        return hashFunc;
    }

public:

    class Iterator{

        friend class HashMap<K,V>;

    private:
//        HashMap<K,V> * hashMapPtr ;
        HashNode<K,V>* hashNodePtr;
        int curBucket;
        Iterator(HashNode<K,V>* hnp)
        {
            hashNodePtr = hnp;

        }
        Iterator(HashNode<K,V>* hnp, int n)
        {
            hashNodePtr = hnp;
            curBucket = n;

        }

    public:
        //This is the constructor for Iterator.
        Iterator(){}


        //Returns the number of the bucket this iterator is at currently.
        unsigned int getCurrentBucket() const
        {
            //return hashMapPtr->hashFunction(hashNodePtr->key);
            return curBucket;
        }

        //Return the key/value pair at the iterator's current position in the map.
        std::pair<K,V> operator*() const{return pair<K,V>(hashNodePtr->key, hashNodePtr->value);}

        //Pre-increment overload; advance the operator one position in the map. Return this iterator. NOTE: if the
        //iterator has reached the end of the map (past the last pair), its data should be equal to
        //HashMap<K,V>::end().
        Iterator& operator++()
        {

            assert((++curBucket) != NULL);
            hashNodePtr = hashNodePtr->next;
//            if(hashNodePtr->next != NULL)
//            {
//
//                hashNodePtr = hashNodePtr->next;
//            }else{
//                assert((++curBucket) != NULL);
//            }


//            ++curBucket;
//            if(curBucket != NULL)
//            {
//                hashNodePtr = hashNodePtr->next;
//            }

//            HashNode<K,V>* hashNodePtrCur = hashNodePtr;
            // unsigned int index = hashMapPtr->hashFunction(hashNodePtrCur->key);

//            assert((++curBucket) != NULL);
//            hashNodePtr = hashNodePtr->next;
//            if(hashNodePtr == NULL){++curBucket; }
//            while(hashNodePtr == NULL && ++curBucket < hashMapPtr->bucketCounts)
//            {
//                hashNodePtr = hashMapPtr->table[curBucket];
//            }

            return *this;
        }

        //Returns true it both iterators point to the same location in the map, and false otherwise.
        bool operator==(Iterator const& rhs){return hashNodePtr == rhs.hashNodePtr;}

        //Returns false it both iterators point to the same location in the map, and true otherwise.
        bool operator!=(Iterator const& rhs){return hashNodePtr != rhs.hashNodePtr;}


    };




    //This is the constructor for HashMap, which allocates the number of buckets specified in bucketCount.
    HashMap<K,V>(unsigned int bucketCount)
    {
        this->bucketCounts = bucketCount;
//        table[bucketCount];
        table = new HashNode<K,V> *[bucketCount]();
        for(int i = 0; i < bucketCount; i++)
        {
            table[i] = NULL;

        }
    };

    //Return an Iterator pointing to the beginning of the map (first bucket, first key/value pair).
    Iterator begin() const {return Iterator(table[0], 0); }


    //Return an Iterator pointing past the end of the map (an invalid, unique state.)
    Iterator end() const{return Iterator(0);}

    HashNode<K,V>* atIndexx(K& key)
    {
        unsigned int index = hashFunction(key);
        HashNode<K, V> *hNode = table[index];
        return hNode;
    }

    //Remove all key/value pairs from the map.
    void clear()
    {
        for (int i = 0; i < bucketCounts; ++i) {
            HashNode<K, V> *hNode = table[i];
            while (hNode != NULL) {
                HashNode<K, V> *curNode = hNode;
                hNode = hNode->next;
                delete curNode;
            }
            table[i] = NULL;
        }
        delete [] table;
    }

    ~HashMap(){clear();}

    //Remove the key, and its corresponding value, from the map.
    void remove(K const& key)
    {
        unsigned int index = hashFunction(key);
        HashNode<K, V> *curNode = NULL;
        HashNode<K, V> *hNode = table[index];

        while (hNode != NULL && hNode->key != key) {
            curNode = hNode;
            hNode = hNode->next;
        }

        if (hNode == NULL) {
            return;
        }
        else
        {
            if (curNode == NULL) {
                // remove first bucket from table
                table[index] = hNode->next;
            } else {
                curNode->next =hNode->next;
            }
            delete hNode;
        }
    }
    HashNode<K,V>* nextTable(K const& key)
    {
        unsigned int index = hashFunction(key);
        ptrToCurTableIndex = table[index];
        HashNode<K, V> *hNode = table[index];
        return ++ptrToCurTableIndex;
    }
//CAREFUL – this one is tricky! This operator overload allows the use of the indexing (square braces)
//operator. If the key does not exist in the map, this function default-constructs the value and places the pair in
//the map. In all cases, it then returns a reference to the value associated with the key. This allows the
//operator to be used both for assignment and for checking value (like standard arrays.)

    V& operator[](K const& key)
    {
        unsigned int index = hashFunction(key);
        HashNode<K, V> *curNode = NULL;
        HashNode<K, V> *hNode = table[index];

        V value = V();
        while (hNode != NULL && hNode->key != key) {
            value = hNode->value;
            curNode = hNode;
            hNode = hNode->next;
        }

        if (hNode == NULL) {
            hNode = new HashNode<K, V>(key,  value);
            if (curNode == NULL) {
                // insert first bucket
                table[index]= hNode;
                ptrToCurTableIndex = table[index];
                return hNode->value;

            } else {
                curNode->next = hNode;
                return curNode->value;
            }
        } else {
            hNode->value = value;
            return hNode->value;
        }
    }

};


#endif