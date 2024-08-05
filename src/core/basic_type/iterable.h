#pragma once

namespace Core {
    template<typename ContainerType>
    class Iterable : public ContainerType {
    public:
        typedef ContainerType Super;

        Iterable() {
            rewind();
        }

        Iterable(const ContainerType& container)
            : Super(container) {
            rewind();
        }

        Iterable(const Iterable& right)
            : Super(right) {
            rewind();
        }

        Iterable& operator=(const ContainerType& container) {
            Super::operator=(container);
            rewind();
            return *this;
        }

        void rewind() {
            m_iterator = Super::begin();
        }

        bool hasNext() {
            return m_iterator != Super::end();
        }

        typename ContainerType::Iterator next() {
            typename ContainerType::Iterator result = current();
            m_iterator++;
            return result;
        }

        typename ContainerType::Iterator current() {
            return m_iterator;
        }

    private:
        typename ContainerType::Iterator m_iterator;
    };
}
