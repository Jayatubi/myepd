#pragma once

#include "default_storage.h"
#include "default_allocator.h"
#include <utility>

namespace Core {
    template<
        typename ValueType,
        typename AllocatorPolicy,
        template<typename> class StoragePolicy = DefaultStorage
    >
    class BasePtr {
    public:
        typedef BasePtr Super;
        typedef StoragePolicy<ValueType> StorageType;
        typedef AllocatorPolicy AllocatorType;
        typedef typename StorageType::PointerType PointerType;
        typedef typename StorageType::ReferenceType ReferenceType;

        BasePtr()
            : m_storage(nullptr) {
        }

        BasePtr(PointerType pointer)
            : m_storage(pointer) {
        }

        BasePtr(const StorageType& storage)
            : m_storage(storage) {
        }

        BasePtr(BasePtr&& right)
            : m_storage(std::move(right.m_storage)) {
        }

        BasePtr& operator=(const BasePtr& right) {
            m_storage = right.m_storage;
            return *this;
        }

        BasePtr& operator=(BasePtr&& right) {
            swap(m_storage, right.m_storage);

            return *this;
        }

        virtual ~BasePtr() {
        };

        PointerType getPtr() {
            return m_storage.getPtr();
        }

        PointerType getPtr() const {
            return m_storage.getPtr();
        }

        PointerType* getPtrPtr() {
            return m_storage.getPtrPtr();
        }

        PointerType const* getPtrPtr() const {
            return m_storage.getPtrPtr();
        }

        ReferenceType getRef() {
            return m_storage.getRef();
        }

        ReferenceType getRef() const {
            return m_storage.getRef();
        }

        PointerType operator->() {
            return m_storage.getPtr();
        }

        PointerType operator->() const {
            return m_storage.getPtr();
        }

        ReferenceType operator*() {
            return m_storage.getRef();
        }

        ReferenceType operator*() const {
            return m_storage.getRef();
        }

        explicit operator bool() const {
            return getPtr() != 0;
        }

        bool operator==(const BasePtr& right) const {
            return getPtr() == right.getPtr();
        }

        bool operator!=(const BasePtr& right) const {
            return getPtr() != right.getPtr();
        }

        bool operator==(const PointerType& right) const {
            return getPtr() == right;
        }

        bool operator!=(const PointerType& right) const {
            return getPtr() != right;
        }

        void release() {
            free(m_storage.getPtr());
            m_storage = 0;
        }

        template<typename ... ArgTypes>
        static ValueType* allocDefault(ArgTypes ... args) {
            return zero_alloc<ValueType>(args ...);
        }

        template<typename ConcreteType, typename ... ArgTypes>
        static ConcreteType* alloc(ArgTypes ... args) {
            return AllocatorType::template alloc<ConcreteType>(args ...);
        }

        template<typename ConcreteType, typename ... ArgTypes>
        static ConcreteType* zero_alloc(ArgTypes ... args) {
            return AllocatorType::template zero_alloc<ConcreteType>(args ...);
        }

        template<typename ConcreteType>
        static void free(ConcreteType* pointer) {
            AllocatorType::template free<ConcreteType>(pointer);
        }

    public:
        void ManualRelease() {
            free(m_storage.getPtr());
            m_storage = 0;
        }

    protected:
        StorageType m_storage;
    };
}
