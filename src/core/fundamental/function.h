#pragma once

#include "../meta/util.h"
#include "../meta/is_derived.h"
#include "../meta/is_same.h"
#include "../basic_type/tuple.h"
#include "default_allocator.h"
#include "smart_ptr.h"

namespace Core {
    template<typename ReturnType, typename... ArgTypes>
    class FunctorImpl {
    public:
        virtual ~FunctorImpl() {
        }

        virtual ReturnType operator()(ArgTypes... args) const = 0;
    };

    template<typename FuncType, typename ReturnType, typename... ArgTypes>
    class CompatibleFuncImpl : public FunctorImpl<ReturnType, ArgTypes...> {
    public:
        CompatibleFuncImpl(const FuncType& func)
            : m_func(func) {
        }

        ReturnType operator()(ArgTypes... args) const override {
            // TODO: This is damn evil
            auto* mutable_this = const_cast<CompatibleFuncImpl*>(this);
            return mutable_this->m_func(args...);
        }

    private:
        FuncType m_func;
    };

    template<typename ObjectType, typename FuncType, typename ReturnType, typename... ArgTypes>
    class MemberFuncImpl : public FunctorImpl<ReturnType, ArgTypes...> {
    public:
        MemberFuncImpl(ObjectType* pObj, const FuncType& func)
            : m_pObj(pObj), m_func(func) {
        }

        ReturnType operator()(ArgTypes... args) const override {
            return (m_pObj->*m_func)(args...);
        }

    private:
        ObjectType* m_pObj;
        FuncType m_func;
    };

    template<bool>
    struct FunctionConstructHelper;

    template<>
    struct FunctionConstructHelper<true> {
        template<typename PtrType, typename FuncType, typename ReturnType, typename... ArgTypes>
        static PtrType construct(const FuncType& func) {
            return PtrType::template alloc<FuncType>(func);
        }
    };

    template<>
    struct FunctionConstructHelper<false> {
        template<typename PtrType, typename FuncType, typename ReturnType, typename... ArgTypes>
        static PtrType construct(const FuncType& func) {
            return PtrType::template alloc<CompatibleFuncImpl<FuncType, ReturnType, ArgTypes...>>(func);
        }
    };

    template<typename ReturnType, typename... ArgTypes>
    class Function {
    public:
        struct BindTag;

        typedef Tuple<ArgTypes...> ArgType;
        typedef FunctorImpl<ReturnType, ArgTypes...> FuncImplType;
        typedef SmartPtr<FuncImplType, DefaultAllocator> PtrType;

        Function()
            : m_pFunc(nullptr) {
        }

        Function(decltype(nullptr))
            : m_pFunc(nullptr) {
        }

        Function(const FuncImplType& func)
            : m_pFunc(nullptr) {
        }

        template<typename FuncType>
        Function(const FuncType& func)
            : m_pFunc(FunctionConstructHelper<Meta::IsDerived<FuncImplType, FuncType>::Result>::template construct<PtrType, FuncType, ReturnType, ArgTypes...>(func)) {
        }

        template<typename ObjectType, typename FuncType>
        Function(ObjectType& obj, const FuncType& func)
            : m_pFunc(PtrType::template alloc<MemberFuncImpl<ObjectType, FuncType, ReturnType, ArgTypes...>>(&obj, func)) {
        }

        ReturnType operator()(ArgTypes... args) const {
            return m_pFunc != nullptr ? (*m_pFunc)(args...) : ReturnType();
        }

        void nullify() {
            m_pFunc = nullptr;
        }

        operator bool() const {
            return m_pFunc != nullptr;
        }

    protected:
        PtrType m_pFunc;
    };

    template<typename ClassType, typename ReturnType, typename... ArgTypes>
    Function<ReturnType, ArgTypes...> GetLambdaFuncType(ReturnType (ClassType::*)(ArgTypes...) const);

    template<typename ClassType, typename ReturnType, typename... ArgTypes>
    Function<ReturnType, ArgTypes...> GetMutableLambdaFuncType(ReturnType (ClassType::*)(ArgTypes...));

    template<typename LambdaType>
    decltype(GetLambdaFuncType(&LambdaType::operator())) Bind(const LambdaType& func) {
        typedef decltype(GetLambdaFuncType(&LambdaType::operator())) FuncType;
        return FuncType(func);
    }

    template<typename LambdaType>
    decltype(GetMutableLambdaFuncType(&LambdaType::operator())) Bind(const LambdaType& func) {
        typedef decltype(GetMutableLambdaFuncType(&LambdaType::operator())) FuncType;
        return FuncType(func);
    }

    template<typename ReturnType, typename... ArgTypes>
    Function<ReturnType, ArgTypes...> Bind(ReturnType (* func)(ArgTypes...)) {
        return Function<ReturnType, ArgTypes...>(func);
    }

    // For member function
    template<typename ObjectType, typename OwnerType, typename ReturnType, typename... ArgTypes>
    Function<ReturnType, ArgTypes...> Bind(ObjectType& obj, ReturnType (OwnerType::*func)(ArgTypes...)) {
        static_assert((Meta::IsDerived<OwnerType, ObjectType>::Result), "Invalid bind type");
        return Function<ReturnType, ArgTypes...>(obj, static_cast<ReturnType (ObjectType::*)(ArgTypes...)>(func));
    }

    // For member function
    template<typename ObjectType, typename OwnerType, typename ReturnType, typename... ArgTypes>
    Function<ReturnType, ArgTypes...> Bind(ObjectType* obj, ReturnType (OwnerType::*func)(ArgTypes...)) {
        return Bind(*obj, func);
    }

    template<bool>
    struct AutoBindHelper {
        template<typename FuncType>
        static FuncType AutoBind(const FuncType& func) {
            return func;
        }
    };

    template<>
    struct AutoBindHelper<false> {
        template<typename FuncType>
        static auto AutoBind(const FuncType& func) -> decltype(Bind(func)) {
            return Bind(func);
        }
    };

    template<typename ValueType>
    struct HasBound {
        template<typename TestType>
        static Meta::Yes Tester(typename TestType::BindTag*);

        template<typename TestType>
        static Meta::No Tester(...);

        enum {
            Result = Meta::IsSame<decltype(Tester<ValueType>(nullptr)), Meta::Yes>::Result
        };
    };

    template<typename UndeterminedType>
    decltype(AutoBindHelper<HasBound<UndeterminedType>::Result>::PooledAutoBind) AutoBind(const UndeterminedType& func) {
        return AutoBindHelper<HasBound<UndeterminedType>::Result>::AutoBind(func);
    }

    template<typename BindFuncType, typename _ArgType, typename ReturnType = void>
    class UnaryProxyFunc : public FunctorImpl<ReturnType, _ArgType> {
    public:
        typedef typename TupleAt<0, typename BindFuncType::ArgType>::ValueType ConcreteType;
        typedef typename Meta::RemoveAll<ConcreteType>::Result RawConcreteType;
        typedef _ArgType ArgType;

        UnaryProxyFunc(const BindFuncType& underlyingFunc)
            : m_underlyingFunc(underlyingFunc) {
        }

        virtual ReturnType operator()(ArgType arg) const {
            static_assert((Meta::IsConvertable<typename Meta::RemoveAll<ArgType>::Result, RawConcreteType>::Result), "Type dismatch");
            return m_underlyingFunc(static_cast<ConcreteType>(arg));
        }

        virtual bool operator==(const FunctorImpl<ReturnType, ArgType>& right) const {
            const UnaryProxyFunc* pProxy = static_cast<const UnaryProxyFunc*>(&right);

            return pProxy != nullptr && m_underlyingFunc == pProxy->m_underlyingFunc;
        }

    protected:
        BindFuncType m_underlyingFunc;
    };

    template<typename ProxyFuncType, bool>
    struct ProxyFuncSelectorImpl {
        template<typename BindFuncType>
        static auto select(const BindFuncType& func) {
            return ProxyFuncType(func);
        }
    };

    template<typename ProxyFuncType>
    struct ProxyFuncSelectorImpl<ProxyFuncType, true> {
        template<typename BindFuncType>
        static auto select(const BindFuncType& func) {
            return func;
        }
    };

    template<typename ProxyFuncType>
    struct ProxyFuncSelector {
        template<typename BindFuncType>
        static auto select(const BindFuncType& func) {
            typedef typename TupleAt<0, typename BindFuncType::ArgType>::ValueType ConcreteArgType;
            typedef typename ProxyFuncType::ArgType ExpectArgType;
            return ProxyFuncSelectorImpl<ProxyFuncType, Meta::IsSame<ExpectArgType, ConcreteArgType>::Result>::select(func);
        }
    };
}
