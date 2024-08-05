#ifndef GXVECTOR_H__
#define GXVECTOR_H__

#include <initializer_list>
#include <vector>
#include <algorithm>

#include "iterable.h"
#include "basic_type.h"

namespace Core {
    template<typename ValueType>
    class Vector : public std::vector<ValueType> {
    private:
        typedef std::vector<ValueType> BaseType;
    public:
        typedef typename BaseType::size_type SizeType;
        typedef typename BaseType::const_iterator ConstIterator;
        typedef typename BaseType::iterator Iterator;

        Vector()
            : BaseType() {
        }

        Vector(SizeType initSize)
            : BaseType(initSize) {
        }

        Vector(SizeType initSize, const ValueType& initValue)
            : BaseType(initSize, initValue) {
        }

        template<typename IteratorType>
        Vector(IteratorType begin, IteratorType end)
            : BaseType(begin, end) {
        }

        template<typename SrcType>
        Vector(const std::initializer_list<SrcType>& data) {
            auto iter = data.begin();
            auto end = data.end();

            for (; iter != end; ++iter) {
                BaseType::push_back(*iter);
            }
        }

        void copyToBuffer(ValueType* array, U32 count) const {
            count = std::min(count, BaseType::size());
            for (U32 i = 0; i < count; i++) {
                array[i] = (*this)[i];
            }
        }

        template<int ArraySize>
        void copyToArray(ValueType (& array)[ArraySize]) const {
            U32 count = std::min(ArraySize, BaseType::size());
            copyToBuffer(array, count);
        }

        bool contains(const ValueType& value) const {
            return find(value) != BaseType::end();
        }

        template<typename PredicateType>
        bool any(const PredicateType& predicate) const {
            bool result = false;
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();
            for (; iter != end; ++iter) {
                if (predicate(*iter)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        U32 count(const ValueType& value) const {
            U32 result = 0;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                result += *iter == value ? 1 : 0;
            }

            return result;
        }

        void remove(const ValueType& value) {
            Iterator iter = BaseType::begin();

            while (iter != BaseType::end()) {
                if (*iter == value) {
                    iter = BaseType::erase(iter);
                } else {
                    iter++;
                }
            }
        }

        template<typename PredicateType>
        void removeif(const PredicateType& predicate) {
            Iterator iter = BaseType::begin();

            while (iter != BaseType::end()) {
                if (predicate(*iter)) {
                    iter = BaseType::erase(iter);
                } else {
                    iter++;
                }
            }
        }

        S32 indexof(const ValueType& value) const {
            ConstIterator iter = find(value);

            return iter != BaseType::end() ? std::distance(BaseType::begin(), iter) : -1;
        }

        ConstIterator find(const ValueType& value) const {
            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                if (*iter == value) {
                    break;
                }
            }

            return iter;
            //return std::find(BaseType::begin(), BaseType::end(), value);
        }

        Iterator find(const ValueType& value) {
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();

            for (; iter != end; ++iter) {
                if (*iter == value) {
                    break;
                }
            }

            return iter;
            //return std::find(BaseType::begin(), BaseType::end(), value);
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

        template<typename PredicateType>
        ValueType first(const PredicateType& predicate) const {
            auto iter = findif(predicate);
            return iter != BaseType::end() ? *iter : ValueType();
        }

        template<typename PredicateType>
        ValueType choose(const PredicateType& predicate) const {
            ValueType result = ValueType();

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                if (iter == BaseType::begin() || predicate(result, *iter)) {
                    result = *iter;
                }
            }

            return result;
        }

        template<typename FuncType>
        Vector& sort(const FuncType& func) {
            std::sort(BaseType::begin(), BaseType::end(), func);
            return *this;
        }

        template<typename FuncType>
        void foreach(const FuncType& func) {
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();

            for (; iter != end; ++iter) {
                func(*iter);
            }
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
        Vector where(const PredicateType& predicate) const {
            Vector result;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                if (predicate(*iter)) {
                    result.push_back(*iter);
                }
            }

            return result;
        }

        template<typename FuncType>
        auto select(const FuncType& func) const -> Vector<decltype(func(std::declval<ValueType>()))> {
            typedef decltype(func(std::declval<ValueType>())) DstType;
            Vector<DstType> result;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                result.push_back(func(*iter));
            }

            return result;
        }

        template<typename ContainerType>
        Vector& concat(const ContainerType& other) {
            BaseType::insert(BaseType::end(), other.begin(), other.end());
            return *this;
        }

        template<typename ReducerValueType, typename ReducerType>
        ReducerValueType reduce(const ReducerValueType& init, const ReducerType& reducer) const {
            ReducerValueType result = init;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                result = reducer(result, *iter);
            }

            return result;
        }
    };
}

#endif // GXVECTOR_H__
