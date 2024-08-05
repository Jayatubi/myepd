#pragma once

#include <map>
#include "vector.h"

namespace Core {
    template
        <
            typename _KeyType,
            typename _ValueType,
            class _Comparer = std::less<_KeyType>
        >
    class Map : public std::map<_KeyType, _ValueType, _Comparer> {
    public:
        typedef _KeyType KeyType;
        typedef _ValueType ValueType;
        typedef std::map<KeyType, ValueType, _Comparer> BaseType;
        // typedef Function<bool, const KeyType&, const ValueType&> PredicateType;
        typedef typename BaseType::iterator Iterator;
        typedef typename BaseType::const_iterator ConstIterator;

        bool contains(const KeyType& key) {
            return BaseType::find(key) != BaseType::end();
        }

        ConstIterator find(const KeyType& key) const {
            return BaseType::find(key);
        }

        Iterator find(const KeyType& key) {
            return BaseType::find(key);
        }

        Iterator acquire(const KeyType& key) {
            Iterator result = find(key);

            if (result == BaseType::end()) {
                result = BaseType::insert(std::make_pair(key, ValueType())).first;
            }

            return result;
        }

        template<typename PredicateType>
        bool any(const PredicateType& predicate) const {
            bool result = false;
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->first, iter->second)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        bool anyKey(const PredicateType& predicate) const {
            bool result = false;
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->first)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        bool anyValue(const PredicateType& predicate) const {
            bool result = false;
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->second)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        ConstIterator findif(const PredicateType& predicate) const {
            ConstIterator result = BaseType::end();
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->first, iter->second)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        Iterator findif(const PredicateType& predicate) {
            Iterator result = BaseType::end();
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->first, iter->second)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        ConstIterator findifkey(const PredicateType& predicate) const {
            ConstIterator result = BaseType::end();
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->first)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        Iterator findifkey(const PredicateType& predicate) {
            Iterator result = BaseType::end();
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->first)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        ConstIterator findifvalue(const PredicateType& predicate) const {
            ConstIterator result = BaseType::end();
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->second)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename PredicateType>
        Iterator findifvalue(const PredicateType& predicate) {
            Iterator result = BaseType::end();
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(iter->second)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename FuncType>
        void foreachKey(const FuncType& func) {
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();

            for (; iter != end; ++iter) {
                func(iter->first);
            }
        }

        template<typename FuncType>
        void foreachValue(const FuncType& func) {
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();

            for (; iter != end; ++iter) {
                func(iter->second);
            }
        }

        template<typename FuncType>
        void foreach(const FuncType& func) {
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();

            for (; iter != end; ++iter) {
                func(iter->first, iter->second);
            }
        }

        Vector<KeyType> keys() {
            Vector<KeyType> result;

            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();
            for (; iter != end; ++iter) {
                result.push_back(iter->first);
            }

            return result;
        }

        Vector<ValueType> values() {
            Vector<ValueType> result;

            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();
            for (; iter != end; ++iter) {
                result.push_back(iter->second);
            }

            return result;
        }
    };
}
