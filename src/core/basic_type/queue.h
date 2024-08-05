#pragma once

#include <deque>
#include <algorithm>

#include "iterable.h"

namespace Core {
    template<typename ValueType>
    class Queue : public std::deque<ValueType> {
    private:
        typedef std::deque<ValueType> BaseType;
    public:
        typedef typename BaseType::size_type SizeType;
        typedef typename BaseType::const_iterator ConstIterator;
        typedef typename BaseType::iterator Iterator;

        Queue()
            : BaseType() {
        }

        Queue(SizeType initSize)
            : BaseType(initSize) {
        }

        Queue(SizeType initSize, const ValueType& initValue)
            : BaseType(initSize, initValue) {
        }

        template<typename IteratorType>
        Queue(IteratorType begin, IteratorType end)
            : BaseType(begin, end) {
        }

        template<typename SrcType>
        Queue(const std::initializer_list<SrcType>& data) {
            auto iter = data.begin();
            auto end = data.end();

            for (; iter != end; ++iter) {
                BaseType::push_back(*iter);
            }
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
        Queue& sort(const FuncType& func) {
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
        Queue where(const PredicateType& predicate) const {
            Queue result;

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
        auto select(const FuncType& func) const -> Queue<decltype(func(std::declval<ValueType>()))> {
            typedef decltype(func(std::declval<ValueType>())) DstType;
            Queue<DstType> result;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                result.push_back(func(*iter));
            }

            return result;
        }

        template<typename ContainerType>
        Queue& concat(const ContainerType& other) {
            BaseType::insert(BaseType::end(), other.begin(), other.end());
            return *this;
        }
    };
}
