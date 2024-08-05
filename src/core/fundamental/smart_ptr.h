#pragma once
/*
    This 'SmartPtr' class represents as a self reference count managed pointer. It would destroy the pointer stored
    inside it when it finds the reference count has been decreased to 0.
 */
#include "base_ptr.h"
#include "default_allocator.h"
#include "default_storage.h"
#include "../basic_type/basic_type.h"
#include "../meta/enable_if.h"

namespace Core {
    template<typename Allocator>
    class DefaultRefCount {
    public:
        DefaultRefCount() : m_pRefCount(0) {
            m_pRefCount = Allocator::template alloc<U32>();
            *m_pRefCount = 1;
        }

        DefaultRefCount(const DefaultRefCount& right)
            : m_pRefCount(right.m_pRefCount) {
            increase();
        }

        DefaultRefCount(DefaultRefCount&& right)
            : m_pRefCount(right.m_pRefCount) {
            right.m_pRefCount = 0;
        }

        DefaultRefCount& operator=(const DefaultRefCount& right) {
            release();

            m_pRefCount = right.m_pRefCount;

            increase();

            return *this;
        }

        DefaultRefCount& operator=(DefaultRefCount&& right) {
            std::swap(m_pRefCount, right.m_pRefCount);

            return *this;
        }

        ~DefaultRefCount() {
            release();
        }

        U32 release() {
            U32 result = 0;

            if (m_pRefCount != 0) {
                (*m_pRefCount)--;
                result = *m_pRefCount;

                if (*m_pRefCount == 0) {
                    Allocator::free(m_pRefCount);
                }

                m_pRefCount = 0;
            }

            return result;
        }

        U32 increase() {
            U32 result = 0;
            assert(m_pRefCount != NULL);
            if (m_pRefCount != 0) {
                (*m_pRefCount)++;
                result = *m_pRefCount;
            }
            return result;
        }

        U32 getRefCount() {
            return m_pRefCount != 0 ? *m_pRefCount : 0;
        }

    private:
        U32* m_pRefCount;
    };

    template<
        typename ValueType,
        typename AllocatorPolicy,
        template<typename> class StoragePolicy = DefaultStorage,
        typename OwnerShipPolicy = DefaultRefCount<AllocatorPolicy>>
    class SmartPtr : public BasePtr<ValueType, AllocatorPolicy, StoragePolicy> {
    public:
        typedef BasePtr<ValueType, AllocatorPolicy, StoragePolicy> Super;
        typedef typename Super::PointerType PointerType;

        template<
            typename OtherValueType,
            typename OtherAllocatorPolicy,
            template<typename> class OtherStoragePolicy,
            typename OtherOwnerShipPolicy>
        friend
        class SmartPtr;

        SmartPtr()
            : Super(0) {
        }

        SmartPtr(PointerType pointer)
            : Super(pointer) {
        }

        SmartPtr(const typename Super::StorageType& storage, OwnerShipPolicy refCount)
            : Super(storage), m_refCount(refCount) {
        }

        SmartPtr(const SmartPtr& right)
            : Super(right.m_storage), m_refCount(right.m_refCount) {
        }

        template<typename Derived, typename Meta::EnableIf<Meta::IsDerived<ValueType, Derived>::Result,
                                                           int>::Result = 0>
        SmartPtr(const SmartPtr<Derived, AllocatorPolicy>& right)
            : Super(right.m_storage.template Cast<ValueType>()), m_refCount(right.m_refCount) {
        }

        SmartPtr& operator=(const SmartPtr& right) {
            release();
            m_refCount = right.m_refCount;
            Super::m_storage = right.m_storage;

            return *this;
        }

        template<typename T, typename Meta::EnableIf<Meta::IsDerived<ValueType, T>::Result, int>::Result = 0>
        SmartPtr<T, AllocatorPolicy> Cast() const {
            return SmartPtr<T, AllocatorPolicy>(Super::m_storage.template Cast<T>(), m_refCount);
        }

        virtual ~SmartPtr() {
            release();
        }

        bool operator<(const SmartPtr& right) const {
            return this->m_storage.getPtr() < right.m_storage.getPtr();
        }

    private:
        void release() {
            if (m_refCount.release() == 0) {
                Super::release();
            }
        }

    private:
        OwnerShipPolicy m_refCount;
    };
}
