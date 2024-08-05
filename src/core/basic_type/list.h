#pragma once

#include <list>
#include <algorithm>

namespace Core {
    template<typename ValueType>
    class List : public std::list<ValueType> {
    private:
        typedef std::list<ValueType> BaseType;
    public:
        typedef typename BaseType::size_type SizeType;
        typedef typename BaseType::const_iterator ConstIterator;
        typedef typename BaseType::iterator Iterator;

        List()
            : BaseType() {
        }

        List(SizeType initSize)
            : BaseType(initSize) {
        }

        List(SizeType initSize, const ValueType& initValue)
            : BaseType(initSize, initValue) {
        }

        bool contains(const ValueType& value) const {
            return find(value) != BaseType::end();
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
            for (auto& iter : (*this)) {
                if (predicate(*iter)) {
                    result = iter;
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
                }
            }

            return result;
        }

//        template<typename FuncType>
//        List& sort(const FuncType& func)
//        {
//            std::sort(BaseType::begin(), BaseType::end(), func);
//            return *this;
//        }

        template<typename FuncType>
        void foreach(const FuncType& func) {
            Iterator iter = BaseType::begin();
            Iterator end = BaseType::end();

            for (; iter != end; ++iter) {
                func(*iter);
            }
        }

        template<typename PredicateType>
        List where(const PredicateType& predicate) const {
            List result;

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
        List<DstType> select(const FuncType& func) const {
            List<DstType> result;

            ConstIterator iter = BaseType::begin();
            ConstIterator end = BaseType::end();

            for (; iter != end; ++iter) {
                result.push_back(func(*iter));
            }

            return result;
        }

        template<typename ContainerType>
        List& concat(const ContainerType& other) {
            BaseType::insert(BaseType::end(), other.begin(), other.end());
            return *this;
        }
    };
}
