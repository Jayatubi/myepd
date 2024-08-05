#pragma once

#if 0

#include "basic_type.h"
#include "math.h"
#include "vector.h"
#include <cstring>
#include <sstream>

namespace Core {
    class BaseString : public std::basic_string<char, std::char_traits<char>, std::allocator<char>> {
    public:
        typedef std::basic_string<char, std::char_traits<char>, std::allocator<char>> BaseType;
        typedef std::basic_stringstream<char, std::char_traits<char>> StreamType;
        typedef typename BaseType::iterator Iterator;
        typedef typename BaseType::reverse_iterator ReverseIterator;
        typedef typename BaseType::const_iterator ConstIterator;
        typedef typename BaseType::const_reverse_iterator ConstReverseIterator;

    public:
        BaseString() {
        }

        BaseString(const char *pStr)
            : BaseType(pStr) {
        }

        BaseString(const char *pStr, U32 size)
            : BaseType(pStr, size) {
        }

        BaseString(const BaseString &right)
            : BaseType(right) {
        }

        BaseString(const BaseType &right)
            : BaseType(right) {
        }

        BaseString(U32 count, char charValue)
            : BaseType(count, charValue) {
        }

        template<typename T>
        explicit BaseString(const T &value) {
            fromValue(value);
        }

        BaseString &operator=(const char *right) {
            BaseType::operator=(right);
            return *this;
        }

        BaseString &operator=(const BaseString &right) {
            BaseType::operator=(right);
            return *this;
        }

        operator const char *() const {
            return cstr();
        }

        const char *cstr() const {
            return BaseType::c_str();
        }

        char *mutable_cstr() {
            return &BaseType::operator[](0);
        }

        bool compare(const BaseString &r) const {
            return 0 == compare(cstr(), r.cstr());
        }

        BaseString replaceEx(const BaseString &oldValue, const BaseString &newValue) const {
            BaseString result = *this;
            typename BaseType::size_type pos = result.find(oldValue);
            while (pos != BaseType::npos) {
                result.replace(pos, oldValue.length(), newValue);
                pos = result.find(oldValue, pos + newValue.length());
            }

            return result;
        }

        BaseString substring(S32 index, S32 length = -1) const {
            return BaseString(BaseType::substr(index, length));
        }

        // Vector<BaseString> split(const char& spliter) const
        // {
        //     Vector<BaseString> result;
        //     StreamType stream(*this);
        //     BaseString segment;
        //     while (std::getline(stream, segment, spliter))
        //     {
        //         result.push_back(segment);
        //     }
        //     return result;
        // }

        template<typename ValueType>
        Vector<ValueType> splitInto(const char &spliter) const;

        template<typename PredicateType>
        BaseString &trim_left(const PredicateType &predicate) {
            for (Iterator iter = BaseType::begin(); iter != BaseType::end(); ++iter) {
                if (!predicate(*iter)) {
                    if (iter != BaseType::begin()) {
                        BaseType::erase(BaseType::begin(), iter);
                    }

                    break;
                }
            }

            return *this;
        }

        template<typename PredicateType>
        BaseString &trim_right(const PredicateType &predicate) {
            for (ReverseIterator iter = BaseType::rbegin(); iter != BaseType::rend(); ++iter) {
                if (!predicate(*iter)) {
                    if (iter != BaseType::rbegin()) {
                        BaseType::erase(iter.base(), BaseType::end());
                    }

                    break;
                }
            }

            return *this;
        }

        template<typename PredicateType>
        BaseString &trim(const PredicateType &predicate) {
            trim_left(predicate);
            trim_right(predicate);
            return *this;
        }

        BaseString &trim() {
            return trim([](char c) -> bool {
                return std::isspace(c);
            });
        }

        U32 toUInt() const {
            return toUInt(cstr());
        }

        S32 toInt() const {
            return toInt(cstr());
        }

        F32 toFloat() const {
            return toFloat(cstr());
        }

        bool toBool() const {
            return toBool(cstr());
        }

        BaseString toLower() const {
            BaseString result(*this);
            toLower(result.mutable_cstr());
            return result;
        }

        BaseString toUpper() const {
            BaseString result(*this);
            toUpper(result.mutable_cstr());
            return result;
        }

        template<int ArraySize>
        void copyToArray(char (&array)[ArraySize]) const {
            const U32 realLength = min<U32>(ArraySize, BaseType::length());
            copy(BaseType::c_str(), &array[0], realLength);
            array[realLength] = 0;
        }

        template<int ArraySize>
        BaseString operator>>(char (&array)[ArraySize]) const {
            copyToArray(array);

            return *this;
        }

        template<typename ValueType>
        static BaseString join(const ValueType &strs, const BaseString &spliter) {
            BaseString result;

            auto iter = strs.begin();
            auto end = strs.end();

            for (; iter != end; ++iter) {
                if (!result.empty()) {
                    result += spliter;
                }

                result += *iter;
            }

            return result;
        }

        static bool isEmpty(const char *pStr) {
            return pStr == nullptr || strLength(pStr) == 0;
        }

        S32 indexOf(const char *pStr, S32 index = 0) const {
            return BaseType::find_first_of(pStr, index);
        }

        S32 lastIndexOf(const char *pStr, S32 index = 0) const {
            return BaseType::find_last_of(pStr);
        }

        template<typename ValueType>
        void fromValue(const ValueType &value = nullptr) {
            fromValueImpl(value);
        }

    private:
        static void copy(const char *pSrc, char *pDst, U32 length) {
            strncpy(pDst, pSrc, length);
        }

        static void format(char *pDst, const char *pFormat, U32 length, va_list args) {
            vsnprintf(pDst, length, pFormat, args);
        }

        static S32 compare(const char *pStrA, const char *pStrB) {
            return strcmp(pStrA, pStrB);
        }

        static void toLower(char *pStr) {
            U32 length = strLength(pStr);
            for (U32 i = 0; i < length; i++) {
                pStr[i] = static_cast<Char>(tolower(pStr[i]));
            }
        }

        static void toUpper(char *pStr) {
            U32 length = strLength(pStr);
            for (U32 i = 0; i < length; i++) {
                pStr[i] = static_cast<Char>(toupper(pStr[i]));
            }
        }

        static U32 strLength(const char *pStr) {
            return strlen(pStr);
        }

        static U32 toUInt(const char *pStr) {
            return strtoul(pStr, 0, 0);
        }

        static S32 toInt(const char *pStr) {
            return atoi(pStr);
        }

        static F32 toFloat(const char *pStr) {
            return static_cast<F32>(atof(pStr));
        }

        static S32 toBool(const char *pStr) {
            return compare(pStr, "true") == 0;
        }

        void fromValueImpl(U32 value) {
            static char buffer[0x80] = {0};
            sprintf(buffer, "%u", value);
            assign(buffer);
        }

        void fromValueImpl(S32 value) {
            static char buffer[0x80] = {0};
            sprintf(buffer, "%d", value);
            assign(buffer);
        }

        void fromValueImpl(U64 value) {
            static char buffer[0x80] = {0};
            sprintf(buffer, "%llu", value);
            assign(buffer);
        }

        void fromValueImpl(S64 value) {
            static char buffer[0x80] = {0};
            sprintf(buffer, "%lld", value);
            assign(buffer);
        }

        void fromValueImpl(F32 value) {
            static char buffer[0x80] = {0};
            sprintf(buffer, "%f", value);
            assign(buffer);
        }

        void fromValueImpl(F64 value) {
            static char buffer[0x80] = {0};
            sprintf(buffer, "%lf", value);
            assign(buffer);
        }

        void fromValueImpl(bool value) {
            assign(value ? "true" : "false");
        }
    };

    typedef BaseString String;
}
#endif
