#pragma once

#include "function.h"
#include "../basic_type/list.h"

namespace Core {
    template<typename ResultType, typename ... ArgTypes>
    class Signal {
    public:
        typedef Function<ResultType, ArgTypes ...> FuncType;

        void connect(const FuncType& func) {
            //GX_ASSERT(!has(func), "Duplicate signals");
            if (!has(func)) {
                m_pendingFuncs.push_back(func);
            }
        }

        void disconnect(const FuncType& func) {
            for (auto iter = m_funcs.begin(); iter != m_funcs.end(); ++iter) {
                if (*iter == func) {
                    iter->nullify();
                }
            }

            for (auto iter = m_pendingFuncs.begin(); iter != m_pendingFuncs.end(); ++iter) {
                if (*iter == func) {
                    iter->nullify();
                }
            }
        }

        bool has(const FuncType& func) {
            return m_funcs.contains(func) || m_pendingFuncs.contains(func);
        }

        operator bool() {
            return !m_funcs.empty() || !m_pendingFuncs.empty();
        }

        Signal& operator()(ArgTypes ... args) {
            if (!m_pendingFuncs.empty()) {
                m_funcs.splice(m_funcs.end(), m_pendingFuncs);
            }

            auto iter = m_funcs.begin();
            for (; iter != m_funcs.end();) {
                FuncType func = *iter;
                if (func) {
                    func(args ...);
                    ++iter;
                } else {
                    iter = m_funcs.erase(iter);
                }
            }

            return *this;
        }

    private:
        List<FuncType> m_funcs;
        List<FuncType> m_pendingFuncs;
    };
}
