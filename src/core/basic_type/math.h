#pragma once

namespace Core {

    template<typename ValueType>
    ValueType min(const ValueType& valueA, const ValueType& valueB) {
        return valueA < valueB ? valueA : valueB;
    }

    template<typename ValueType>
    ValueType max(const ValueType& valueA, const ValueType& valueB) {
        return valueA > valueB ? valueA : valueB;
    }

    template<typename ValueType>
    ValueType clamp(const ValueType& value, const ValueType& valueA, const ValueType& valueB) {
        if (valueA < valueB) {
            return min(max(value, valueA), valueB);
        } else {
            return min(max(value, valueB), valueA);
        }
    }

    template<typename ValueType>
    bool inRange(const ValueType& value, const ValueType& lowerBound, const ValueType& upperBound) {
        return value >= lowerBound && value <= upperBound;
    }


    template<typename ValueType>
    Core::F32 normalize(const ValueType& value, const ValueType& lowerBound, const ValueType& upperBound) {
        return (value - lowerBound) / (upperBound - lowerBound);
    }
}
