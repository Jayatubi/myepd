#pragma once

//#include "Assert.h"
#include "basic_type.h"
#include "math.h"


namespace Core {
    inline bool isLittleEndian() {
        const U32 test = 1;
        return reinterpret_cast<const U8*>(&test)[0] == 1;
    }

    class BitsetProxy {
    public:
        BitsetProxy(const U8* pData, U32 bitIndex) : m_pData(const_cast<U8*>(pData)), m_bitIndex(bitIndex) {
        }

        operator bool() const {
            const U32 byteIndex = m_bitIndex / 8;
            const U32 bitIndex = m_bitIndex % 8;
            const U8 mask = static_cast<U8>(1 << bitIndex);

            return (m_pData[byteIndex] & mask) != 0;
        }

        BitsetProxy& operator=(bool flag) {
            const U32 byteIndex = m_bitIndex / 8;
            const U32 bitIndex = m_bitIndex % 8;
            const U8 mask = static_cast<U8>(1 << bitIndex);

            if (flag)
                m_pData[byteIndex] |= mask;
            else
                m_pData[byteIndex] &= ~mask;

            return *this;
        }

    private:
        U8* m_pData;
        U32 m_bitIndex;
    };

    template<int BitCount>
    class Bitset {
    public:
        template<typename ValueType>
        Bitset(const ValueType& value) {
            memset(m_data, 0, sizeof(m_data));
            operator=(value);
        }

        Bitset() {
            memset(m_data, 0, sizeof(m_data));
        }

        ~Bitset() {
        }

        void Clear() {
            memset(m_data, 0, sizeof(m_data));
        }

        BitsetProxy operator[](U32 bitIndex) const {
            assert(bitIndex < BitCount);
            return BitsetProxy(m_data, bitIndex);
        }

        template<typename ValueType>
        operator ValueType() {
            ValueType result;
            static const U32 ValueSize = sizeof(ValueType);

            U8* pAccessor = static_cast<U8*>(static_cast<void*>(&result));
            const bool littleEndian = isLittleEndian();
            for (U32 i = 0; i < ValueSize; i++) {
                pAccessor[i] = i < ByteSize ? m_data[littleEndian ? i : (ValueSize - i - 1)] : 0;
            }

            return result;
        }

        template<typename ValueType>
        Bitset& operator=(const ValueType& data) {
            const U32 srcSize = min<U32>(sizeof(ValueType), sizeof(m_data));

            const U8* pAccessor = reinterpret_cast<const U8*>(&data);
            const bool littleEndian = isLittleEndian();
            for (U32 i = 0; i < srcSize; i++) {
                m_data[i] = pAccessor[littleEndian ? i : (srcSize - i - 1)];
            }

            return *this;
        }

        template<typename ValueType>
        Bitset& operator|=(const ValueType& data) {
            const U32 srcSize = min<U32>(sizeof(ValueType), sizeof(m_data));

            const U8* pAccessor = reinterpret_cast<const U8*>(&data);
            const bool littleEndian = isLittleEndian();
            for (U32 i = 0; i < srcSize; i++) {
                m_data[i] |= pAccessor[littleEndian ? i : (srcSize - i - 1)];
            }

            return *this;
        }

        template<typename ValueType>
        Bitset& operator&=(const ValueType& data) {
            const U32 srcSize = min<U32>(sizeof(ValueType), sizeof(m_data));

            const U8* pAccessor = reinterpret_cast<const U8*>(&data);
            const bool littleEndian = isLittleEndian();
            for (U32 i = 0; i < srcSize; i++) {
                m_data[i] &= pAccessor[littleEndian ? i : (srcSize - i - 1)];
            }

            return *this;
        }

        U32 count() const {
            U32 result = 0;

            for (U32 i = 0; i < ByteSize; i++) {
                const U8& data = m_data[i];
                result +=
                    ((data & 1 << 0) ? 1 : 0) +
                        ((data & 1 << 1) ? 1 : 0) +
                        ((data & 1 << 2) ? 1 : 0) +
                        ((data & 1 << 3) ? 1 : 0) +
                        ((data & 1 << 4) ? 1 : 0) +
                        ((data & 1 << 5) ? 1 : 0) +
                        ((data & 1 << 6) ? 1 : 0) +
                        ((data & 1 << 7) ? 1 : 0);
            }

            return result;
        }

    private:
        static const U32 ByteSize = (BitCount + 7) / 8;
        U8 m_data[ByteSize];
    };
} // namespace Core
