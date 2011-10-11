#ifndef EG_UTILITY_STRING_DICTIONARY_H
#define EG_UTILITY_STRING_DICTIONARY_H

#include <vector>
#include <string>

namespace EG{
	namespace Utility{
		typedef std::vector<std::string>::iterator StringDictionaryKeysIterator;
		/* Hash Table Entry */
		template <class ValueType>
		class StringDictionaryEntry{
			public:
				StringDictionaryEntry(std::string key, unsigned int hashed_key, ValueType value);
				~StringDictionaryEntry(void);

				std::string GetKey(void);
				unsigned int GetHashedKey(void);
				ValueType GetValue(void);
				void SetValue(ValueType value);
				StringDictionaryEntry *GetNext(void);
				void SetNext(StringDictionaryEntry *next);
				//bool HasNext(void);
			private:
				std::string _key;
				unsigned int _hashed_key;
				ValueType _value;
				//bool _has_next;
				StringDictionaryEntry *_next;
		};

		template <class ValueType>
		StringDictionaryEntry<ValueType>::StringDictionaryEntry(std::string key, unsigned int hashed_key, ValueType value){
			_key = key;
			_hashed_key = hashed_key;
			_value = value;
			_next = NULL;
			//_has_ne
		}

		template <class ValueType>
		StringDictionaryEntry<ValueType>::~StringDictionaryEntry(void){
			//
		}

		template <class ValueType>
		unsigned int StringDictionaryEntry<ValueType>::GetHashedKey(void){
			return _hashed_key;
		}

		template <class ValueType>
		std::string StringDictionaryEntry<ValueType>::GetKey(void){
			return _key;
		}

		template <class ValueType>
		ValueType StringDictionaryEntry<ValueType>::GetValue(void){
			return _value;
		}

		template <class ValueType>
		void StringDictionaryEntry<ValueType>::SetValue(ValueType value){
			_value = value;
		}

		template <class ValueType>
		StringDictionaryEntry<ValueType> *StringDictionaryEntry<ValueType>::GetNext(void){
			return _next;
		}

		template <class ValueType>
		void StringDictionaryEntry<ValueType>::SetNext(StringDictionaryEntry *next){
			_next = next;
		}


		/* Hash Table */
		template <class HashTableValueType>
		class StringDictionary{
			public:
				StringDictionary(unsigned int map_size = 100000);
				~StringDictionary(void);

				void Clear(void);

				void Set(std::string key, HashTableValueType value);
				HashTableValueType Get(std::string key);
				//HashTableValueType operator[](std::string key);
				bool Has(std::string key);

				std::vector<std::string> *GetKeys(void);
				StringDictionaryKeysIterator GetKeysBegin(void);
				StringDictionaryKeysIterator GetKeysEnd(void);
			private:
				unsigned int _map_size;
				unsigned int HashingFunction(std::string key);
				StringDictionaryEntry<HashTableValueType> **_table;
				std::vector<std::string> _keys;
		};

		template <class HashTableValueType>
		StringDictionary<HashTableValueType>::StringDictionary(unsigned int map_size){
			_map_size = map_size;
			_table = new StringDictionaryEntry<HashTableValueType>*[_map_size];
			for (unsigned int i = 0; i < _map_size; i++){
				_table[i] = NULL;
			}
		}

		template <class HashTableValueType>
		StringDictionary<HashTableValueType>::~StringDictionary(void){
			Clear();
		}

		template <class HashTableValueType>
		void StringDictionary<HashTableValueType>::Clear(void){
			for (unsigned int i = 0; i < _map_size; i++){
				if (_table[i] != NULL){
					delete _table[i];
				}
			}
			delete []_table;
		}

		template <class HashTableValueType>
		void StringDictionary<HashTableValueType>::Set(std::string key, HashTableValueType value){
			unsigned int hashed_key = HashingFunction(key);
			if (_table[hashed_key] == NULL){
				_table[hashed_key] = new StringDictionaryEntry<HashTableValueType>(key, hashed_key, value);
				_keys.push_back(key);
			}else{
				StringDictionaryEntry<HashTableValueType> *entry = _table[hashed_key];
				while (entry->GetNext() != NULL){
					entry = entry->GetNext();
				}
				if (entry->GetKey() == key){
					entry->SetValue(value);
				}else{
					entry->SetNext(new StringDictionaryEntry<HashTableValueType>(key, hashed_key, value));
					_keys.push_back(key);
				}
			}
		}

		template <class HashTableValueType>
		HashTableValueType StringDictionary<HashTableValueType>::Get(std::string key){
			unsigned int hashed_key = HashingFunction(key);
			if (_table[hashed_key] == NULL){
				return 0;
			}else{
				StringDictionaryEntry<HashTableValueType> *entry = _table[hashed_key];
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
		bool StringDictionary<HashTableValueType>::Has(std::string key){
			unsigned int hashed_key = HashingFunction(key);
			if (_table[hashed_key] == NULL){
				return false;
			}else{
				StringDictionaryEntry<HashTableValueType> *entry = _table[hashed_key];
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
		std::vector<std::string> *StringDictionary<HashTableValueType>::GetKeys(void){
			return &_keys;
		}

		template <class HashTableValueType>
		StringDictionaryKeysIterator StringDictionary<HashTableValueType>::GetKeysBegin(void){
			return _keys.begin();
		}

		template <class HashTableValueType>
		StringDictionaryKeysIterator StringDictionary<HashTableValueType>::GetKeysEnd(void){
			return _keys.end();
		}

		template <class HashTableValueType>
		unsigned int StringDictionary<HashTableValueType>::HashingFunction(std::string key){
			unsigned int hash = 2166136261UL;
			const char *c_string = key.c_str();
			for (const char *s = c_string; *s; s++){
				hash = (16777619 * hash) ^ (*s);
			}
			return hash % _map_size;
		}

		/*template <class HashTableValueType>
		unsigned int StringDictionary<HashTableValueType>::HashingFunction(std::string key){
			unsigned long int hash = 5381;
			int c;
			const char *str = key.c_str();
			while (c = *str++){
				hash = ((hash << 5) + hash) + c; // hash * 33 + c
			}
			return hash % _map_size;
		}*/

		/*template <class HashTableValueType>
		unsigned int StringDictionary<HashTableValueType>::HashingFunction(std::string key){
			unsigned long int hash = 0;
			int c;
			const char *str = key.c_str();
			while (c = *str++){
				hash = c + (hash << 6) + (hash << 16) - hash;
			}
			return hash % _map_size;
		}*/
	}
}

#endif
