#ifndef EG_UTILITY_DICTIONARY_H
#define EG_UTILITY_DICTIONARY_H

#include <vector>
#include <string>
#include <iostream>

namespace EG{
    namespace Utility{
        template <class KeyType, class ValueType>
        class DictionaryEntry{
            public:
                DictionaryEntry(KeyType key, unsigned int hashed_key, ValueType value);
                ~DictionaryEntry(void);

                KeyType GetKey(void);
                unsigned int GetHashedKey(void);
                ValueType GetValue(void);
                void SetValue(ValueType value);
                DictionaryEntry *GetNext(void);
                void SetNext(DictionaryEntry *next);
                //bool HasNext(void);
            private:
                KeyType _key;
                unsigned int _hashed_key;
                ValueType _value;
                //bool _has_next;
                DictionaryEntry *_next;
        };

        template <class KeyType, class ValueType>
        DictionaryEntry<KeyType, ValueType>::DictionaryEntry(KeyType key, unsigned int hashed_key, ValueType value){
            _key = key;
            _hashed_key = hashed_key;
            _value = value;
            _next = NULL;
            //_has_ne
        }

        template <class KeyType, class ValueType>
        DictionaryEntry<KeyType, ValueType>::~DictionaryEntry(void){
            DictionaryEntry<KeyType, ValueType> *n = _next;
            while (n != NULL){
                DictionaryEntry<KeyType, ValueType> *d = n;
                n = d->GetNext();
                delete d;

            }
        }

        template <class KeyType, class ValueType>
        unsigned int DictionaryEntry<KeyType, ValueType>::GetHashedKey(void){
            return _hashed_key;
        }

        template <class KeyType, class ValueType>
        KeyType DictionaryEntry<KeyType, ValueType>::GetKey(void){
            return _key;
        }

        template <class KeyType, class ValueType>
        ValueType DictionaryEntry<KeyType, ValueType>::GetValue(void){
            return _value;
        }

        template <class KeyType, class ValueType>
        void DictionaryEntry<KeyType, ValueType>::SetValue(ValueType value){
            _value = value;
        }

        template <class KeyType, class ValueType>
        DictionaryEntry<KeyType, ValueType> *DictionaryEntry<KeyType, ValueType>::GetNext(void){
            return _next;
        }

        template <class KeyType, class ValueType>
        void DictionaryEntry<KeyType, ValueType>::SetNext(DictionaryEntry *next){
            _next = next;
        }



        template <class HashTableKeyType, class HashTableValueType>
        class Dictionary{
            public:
                Dictionary(unsigned int map_size = 100);
                ~Dictionary(void);

                void Clear(void);
                void Clean(void);

                void Set(HashTableKeyType key, HashTableValueType value);
                HashTableValueType Get(HashTableKeyType key);
                //HashTableValueType operator[](std::string key);
                bool Has(HashTableKeyType key);

                typename std::vector<HashTableKeyType> *GetKeys(void);
                typename std::vector<HashTableKeyType>::iterator GetKeysBegin(void);
                typename std::vector<HashTableKeyType>::iterator GetKeysEnd(void);
            private:
                bool initialized;
                unsigned int _map_size;
                unsigned int HashingFunction(std::string key);
                unsigned int HashingFunction(unsigned int key);
                DictionaryEntry<HashTableKeyType, HashTableValueType> **_table;
                std::vector<HashTableKeyType> _keys;
        };

        template <class HashTableKeyType, class HashTableValueType>
        Dictionary<HashTableKeyType, HashTableValueType>::Dictionary(unsigned int map_size){
            _map_size = map_size;
            _table = NULL;
            _table = new DictionaryEntry<HashTableKeyType, HashTableValueType>*[_map_size];
            for (unsigned int i = 0; i < _map_size; i++){
                _table[i] = NULL;
            }
            initialized = true;
        }

        template <class HashTableKeyType, class HashTableValueType>
        Dictionary<HashTableKeyType, HashTableValueType>::~Dictionary(void){
            Clear();
        }

        template <class HashTableKeyType, class HashTableValueType>
        void Dictionary<HashTableKeyType, HashTableValueType>::Clear(void){
            if (_table != NULL && initialized){
                for (unsigned int i = 0; i < _map_size; i++){
                    if (_table[i] != NULL){
                        DictionaryEntry<HashTableKeyType, HashTableValueType> *entry = _table[i];
                        while (entry->GetNext()){
                            DictionaryEntry<HashTableKeyType, HashTableValueType> *delete_me = entry;
                            delete delete_me;
                            entry = entry->GetNext();
                        }
                        delete _table[i];
                    }
                    _table[i] = NULL;
                }
            }
        }

        template <class HashTableKeyType, class HashTableValueType>
        void Dictionary<HashTableKeyType, HashTableValueType>::Clean(void){
            if (_table != NULL && initialized){
                //std::cout << "Map Size: " << _map_size << std::endl;
                for (unsigned int i = 0; i < _map_size; i++){
                    //std::cout << i << std::endl;
                    if (_table[i] != NULL){
                        DictionaryEntry<HashTableKeyType, HashTableValueType> *entry = _table[i];
                        while (entry->GetNext()){
                            DictionaryEntry<HashTableKeyType, HashTableValueType> *delete_me = entry;
                            delete delete_me;
                            entry = entry->GetNext();
                        }
                        delete _table[i];
                        _table[i] = NULL;
                    }
                }
                //delete []_table;
                //initialized = false;
                //_table = NULL;
            }
            //std::cout << "Done Clearing STR" << std::endl;
        }

        template <class HashTableKeyType, class HashTableValueType>
        void Dictionary<HashTableKeyType, HashTableValueType>::Set(HashTableKeyType key, HashTableValueType value){
            unsigned int hashed_key = HashingFunction(key);
            if (_table[hashed_key] == NULL){
                _table[hashed_key] = new DictionaryEntry<HashTableKeyType, HashTableValueType>(key, hashed_key, value);
                _keys.push_back(key);
            }else{
                DictionaryEntry<HashTableKeyType, HashTableValueType> *entry = _table[hashed_key];
                while (entry->GetNext() != NULL){
                    entry = entry->GetNext();
                }
                if (entry->GetKey() == key){
                    entry->SetValue(value);
                }else{
                    entry->SetNext(new DictionaryEntry<HashTableKeyType, HashTableValueType>(key, hashed_key, value));
                    _keys.push_back(key);
                }
            }
        }

        template <class HashTableKeyType, class HashTableValueType>
        HashTableValueType Dictionary<HashTableKeyType, HashTableValueType>::Get(HashTableKeyType key){
            unsigned int hashed_key = HashingFunction(key);
            if (_table[hashed_key] == NULL){
                return HashTableValueType(0);
            }else{
                DictionaryEntry<HashTableKeyType, HashTableValueType> *entry = _table[hashed_key];
                while (entry != NULL && entry->GetKey() != key){
                    entry = entry->GetNext();
                }
                if (entry == NULL){
                    return HashTableValueType(0);
                }else{
                    return entry->GetValue();
                }
            }
        }

        template <class HashTableKeyType, class HashTableValueType>
        bool Dictionary<HashTableKeyType, HashTableValueType>::Has(HashTableKeyType key){
            unsigned int hashed_key = HashingFunction(key);
            if (_table[hashed_key] == NULL){
                return false;
            }else{
                DictionaryEntry<HashTableKeyType, HashTableValueType> *entry = _table[hashed_key];
                while (entry != NULL && entry->GetKey() != key){
                    entry = entry->GetNext();
                }
                if (entry == NULL){
                    return false;
                }else{
                    return true;
                }
            }
        }

        template <class HashTableKeyType, class HashTableValueType>
        typename std::vector<HashTableKeyType> *Dictionary<HashTableKeyType, HashTableValueType>::GetKeys(void){
            return &_keys;
        }

        template <class HashTableKeyType, class HashTableValueType>
        typename std::vector<HashTableKeyType>::iterator Dictionary<HashTableKeyType, HashTableValueType>::GetKeysBegin(void){
            return _keys.begin();
        }

        template <class HashTableKeyType, class HashTableValueType>
        typename std::vector<HashTableKeyType>::iterator Dictionary<HashTableKeyType, HashTableValueType>::GetKeysEnd(void){
            return _keys.end();
        }

        template <class HashTableKeyType, class HashTableValueType>
        unsigned int Dictionary<HashTableKeyType, HashTableValueType>::HashingFunction(std::string key){
            unsigned int hash = 2166136261UL;
            const char *c_string = key.c_str();
            for (const char *s = c_string; *s; s++){
                hash = (16777619 * hash) ^ (*s);
            }
            return hash % _map_size;
        }

        template <class HashTableKeyType, class HashTableValueType>
        unsigned int Dictionary<HashTableKeyType, HashTableValueType>::HashingFunction(unsigned int key){
            return key % _map_size;
        }

        /*template <class HashTableValueType>
        unsigned int Dictionary<HashTableKeyType, HashTableValueType>::HashingFunction(std::string key){
            unsigned long int hash = 5381;
            int c;
            const char *str = key.c_str();
            while (c = *str++){
                hash = ((hash << 5) + hash) + c; // hash * 33 + c
            }
            return hash % _map_size;
        }*/

        /*template <class HashTableValueType>
        unsigned int Dictionary<HashTableKeyType, HashTableValueType>::HashingFunction(std::string key){
            unsigned long int hash = 0;
            int c;
            const char *str = key.c_str();
            while (c = *str++){
                hash = c + (hash << 6) + (hash << 16) - hash;
            }
            return hash % _map_size;
        }*/

        //typedef std::vector<unsigned int>::iterator UnsignedIntDictionaryKeysIterator;
        //typedef std::vector<std::string>::iterator StringDictionaryKeysIterator;
    }
}

#endif
