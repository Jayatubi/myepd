#pragma once

#include <set>
#include "vector.h"

namespace Core {
    template<typename ValueType>
    class Set : public std::set<ValueType> {
        typedef std::set<ValueType> BaseType;
    public:
        typedef typename BaseType::iterator Iterator;
        typedef typename BaseType::const_iterator ConstIterator;

        Set()
            : BaseType() {
        }

        bool contains(const ValueType& key) const {
            return BaseType::find(key) != BaseType::end();
        }

        ConstIterator find(const ValueType& key) const {
            return BaseType::find(key);
        }

        Iterator find(const ValueType& key) {
            return BaseType::find(key);
        }

        template<typename PredicateType>
        ConstIterator findif(const PredicateType& predicate) const {
            ConstIterator result = BaseType::end();

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                if (predicate(*iter)) {
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
                if (predicate(*iter)) {
                    result = iter;
                    break;
                }
            }

            return result;
        }

        template<typename FuncType>
        void foreach(const FuncType& func) const {
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                func(*iter);
            }
        }

        template<typename PredicateType>
        Vector<ValueType> where(const PredicateType& predicate) const {
            Vector<ValueType> result;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                if (predicate(*iter)) {
                    result.push_back(*iter);
                }
            }

            return result;
        }

        template<typename DstType, typename FuncType>
        Vector<DstType> select(const FuncType& func) const {
            Vector<DstType> result;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                result.push_back(func(*iter));
            }

            return result;
        }

        Vector<ValueType> values() {
            Vector<ValueType> result;

            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();
            for (; iter != end; ++iter) {
                result.push_back(*iter);
            }

            return result;
        }

        template<typename ContainerType>
        Set& concat(const ContainerType& other) {
            BaseType::insert(other.begin(), other.end());
            return *this;
        }
    };
}
