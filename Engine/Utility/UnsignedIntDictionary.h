#ifndef EG_UTILITY_UINT_DICTIONARY_H
#define EG_UTILITY_UINT_DICTIONARY_H

#include <vector>
#include <string>
#include <iostream>

namespace EG{
    namespace Utility{
        typedef std::vector<unsigned int>::iterator UnsignedIntDictionaryKeysIterator;
        /* Hash Table Entry */
        template <class ValueType>
        class UnsignedIntDictionaryEntry{
            public:
                UnsignedIntDictionaryEntry(unsigned int key, unsigned int hashed_key, ValueType value);
                ~UnsignedIntDictionaryEntry(void);

                unsigned int GetKey(void);
                unsigned int GetHashedKey(void);
                ValueType GetValue(void);
                void SetValue(ValueType value);
                UnsignedIntDictionaryEntry *GetNext(void);
                void SetNext(UnsignedIntDictionaryEntry *next);
                //bool HasNext(void);
            private:
                unsigned int _key;
                unsigned int _hashed_key;
                ValueType _value;
                //bool _has_next;
                UnsignedIntDictionaryEntry *_next;
        };

        template <class ValueType>
        UnsignedIntDictionaryEntry<ValueType>::UnsignedIntDictionaryEntry(unsigned int key, unsigned int hashed_key, ValueType value){
            _key = key;
            _hashed_key = hashed_key;
            _value = value;
            _next = NULL;
            //_has_ne
        }

        template <class ValueType>
        UnsignedIntDictionaryEntry<ValueType>::~UnsignedIntDictionaryEntry(void){
            //
        }

        template <class ValueType>
        unsigned int UnsignedIntDictionaryEntry<ValueType>::GetHashedKey(void){
            return _hashed_key;
        }

        template <class ValueType>
        unsigned int UnsignedIntDictionaryEntry<ValueType>::GetKey(void){
            return _key;
        }

        template <class ValueType>
        ValueType UnsignedIntDictionaryEntry<ValueType>::GetValue(void){
            return _value;
        }

        template <class ValueType>
        void UnsignedIntDictionaryEntry<ValueType>::SetValue(ValueType value){
            _value = value;
        }

        template <class ValueType>
        UnsignedIntDictionaryEntry<ValueType> *UnsignedIntDictionaryEntry<ValueType>::GetNext(void){
            return _next;
        }

        template <class ValueType>
        void UnsignedIntDictionaryEntry<ValueType>::SetNext(UnsignedIntDictionaryEntry *next){
            _next = next;
        }


        /* Hash Table */
        template <class HashTableValueType>
        class UnsignedIntDictionary{
            public:
                UnsignedIntDictionary(unsigned int map_size = 100);
                ~UnsignedIntDictionary(void);

                void Clear(void);

                void Set(unsigned int key, HashTableValueType value);
                HashTableValueType Get(unsigned int key);
                //HashTableValueType operator[](unsigned int key);
                bool Has(unsigned int key);

                std::vector<unsigned int> *GetKeys(void);
                UnsignedIntDictionaryKeysIterator GetKeysBegin(void);
                UnsignedIntDictionaryKeysIterator GetKeysEnd(void);
            private:
                bool initialized;
                unsigned int _map_size;
                unsigned int HashingFunction(unsigned int key);
                UnsignedIntDictionaryEntry<HashTableValueType> **_table;
                std::vector<unsigned int> _keys;
        };

        template <class HashTableValueType>
        UnsignedIntDictionary<HashTableValueType>::UnsignedIntDictionary(unsigned int map_size){
            _map_size = map_size;
            _table = NULL;
            _table = new UnsignedIntDictionaryEntry<HashTableValueType>*[_map_size];
            for (unsigned int i = 0; i < _map_size; i++){
                _table[i] = NULL;
            }
            initialized = true;
        }

        template <class HashTableValueType>
        UnsignedIntDictionary<HashTableValueType>::~UnsignedIntDictionary(void){
            Clear();
        }

        template <class HashTableValueType>
        void UnsignedIntDictionary<HashTableValueType>::Clear(void){
            if (_table != NULL && initialized){
                for (unsigned int i = 0; i < _map_size; i++){
                    if (_table[i] != NULL){
                        delete _table[i];
                    }
                }
                delete []_table;
                initialized = false;
                _table = NULL;
            }
        }

        template <class HashTableValueType>
        void UnsignedIntDictionary<HashTableValueType>::Set(unsigned int key, HashTableValueType value){
            unsigned int hashed_key = HashingFunction(key);
            if (_table[hashed_key] == NULL){
                _table[hashed_key] = new UnsignedIntDictionaryEntry<HashTableValueType>(key, hashed_key, value);
                _keys.push_back(key);
            }else{
                UnsignedIntDictionaryEntry<HashTableValueType> *entry = _table[hashed_key];
                while (entry->GetNext() != NULL){
                    entry = entry->GetNext();
                }
                if (entry->GetKey() == key){
                    entry->SetValue(value);
                }else{
                    entry->SetNext(new UnsignedIntDictionaryEntry<HashTableValueType>(key, hashed_key, value));
                    _keys.push_back(key);
                }
            }
        }

        template <class HashTableValueType>
        HashTableValueType UnsignedIntDictionary<HashTableValueType>::Get(unsigned int key){
            unsigned int hashed_key = HashingFunction(key);
            if (_table[hashed_key] == NULL){
                return 0;
            }else{
                UnsignedIntDictionaryEntry<HashTableValueType> *entry = _table[hashed_key];
                while (entry != NULL && entry->GetKey() != key){
                    entry = entry->GetNext();
                }
                if (entry == NULL){
                    return 0;
                }else{
                    return entry->GetValue();
                }
            }
        }

        template <class HashTableValueType>
        bool UnsignedIntDictionary<HashTableValueType>::Has(unsigned int key){
            unsigned int hashed_key = HashingFunction(key);
            if (_table[hashed_key] == NULL){
                return false;
            }else{
                UnsignedIntDictionaryEntry<HashTableValueType> *entry = _table[hashed_key];
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

        template <class HashTableValueType>
        std::vector<unsigned int> *UnsignedIntDictionary<HashTableValueType>::GetKeys(void){
            return &_keys;
        }

        template <class HashTableValueType>
        UnsignedIntDictionaryKeysIterator UnsignedIntDictionary<HashTableValueType>::GetKeysBegin(void){
            return _keys.begin();
        }

        template <class HashTableValueType>
        UnsignedIntDictionaryKeysIterator UnsignedIntDictionary<HashTableValueType>::GetKeysEnd(void){
            return _keys.end();
        }

        template <class HashTableValueType>
        unsigned int UnsignedIntDictionary<HashTableValueType>::HashingFunction(unsigned int key){
            return key % _map_size;
        }
    }
}

#endif
