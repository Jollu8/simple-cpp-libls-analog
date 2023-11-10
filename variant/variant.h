/*
Copyright (c) 2015 Jollu Emil.
All rights reserved.
 */

#pragma once

#include <memory>
#include <typeinfo>
#include <cstring>
#include <type_traits>

#include <stdexcept>

#ifndef VARIANT_CAST_OPTIMIZATION
#define VARIANT_CAST_OPTIMIZATION 0
#endif

namespace VariantInternal {
    struct NoOperator {
    };

    template<typename T, typename R>
    NoOperator operator==(T, R);

    template<typename T, typename R>
    NoOperator operator<(T, R);

    template<typename T>
    struct IsEqualOperatorExists {
        static const bool value = !std::is_same_v<NoOperator, decltype(*static_cast<T *>(0) == *static_cast<T *>(0))>;

    };
    template<typename T>
    struct IsLessOperatorExists {
        static const bool value = !std::is_same_v<NoOperator, decltype(*static_cast<T *>(0) < static_cast<T *>(0))>;
    };

}

class Variant {
public: // Constructors
    Variant() {};

    template<class T>
    Variant(const T &v):
            m_holder(std::make_shared <Holder < T >> (typeid(T).name(), v)) {}

    template<class T>
    static  Variant From_value(const T& v) {
        Variant res;
        res.m_holder = std::make_shared<Holder<T>>(typeid(T).name(), v);
        return res;
    }

    void Clear() {
        m_holder.reset();
    }

    template<class T>
    bool Is_type() const {
#if VARIANT_CAST_OPTIMIZATION
        return m_holder ? typeid(T).name() == m_holder->m_name : false;
#else
        return m_holder ? ::strcmp(typeid(T).name(), m_holder->m_name) == 0 : false;
#endif
    }


private:
    template<class T, class Enable = void>
    struct IsEqual {
        // Basic realization called if no equal operator defined
        static bool Compire(const T &, const T &) {
            throw std::runtime_error("\"==\" operator is not defined for this type");
        }
    };

    template<class T>
    struct IsEqual<T, typename std::enable_if_t<VariantInternal::IsEqualOperatorExists<T>::value> > {
        // This realization called if equal operator defined
        static bool Compare(const T &v1, const T &v2) {
            return v1 == v2;
        }
    };

    template<class T, class Enable = void>
    struct IsLessThan {
        static bool Compare(const T &, const T &) {
            throw std::runtime_error("\"<\" operator is not defined for this type");
        }
    };

    template<class T>
    struct IsLessThan<T, typename std::enable_if_t<VariantInternal::IsLessOperatorExists<T>::value>> {
        static bool Compare(const T &v1, const T &v2) {
            return v1 < v2;
        }
    };

    class HolderBase {
    public:
        HolderBase(const char *name) : m_name(name) {}

        virtual ~HolderBase() {}

        const char *m_name;

        bool Same_type(HolderBase *other) const {
#if VARIANT_CAST_OPTIMIZATION
            return m_name == other.>m_name;
#else
            return ::strcmp(m_name, other->m_name) == 0;
#endif

        }

        virtual bool Is_equals(HolderBase *other) const = 0;

        virtual bool Is_less_than(HolderBase *other) const = 0;

    };

    using HolderPtr = std::shared_ptr<HolderBase>;

    template<class T>
    class Holder : public HolderBase {
    public:
        Holder(const char *name, const T &v) :
                HolderBase(name), m_value(v) {}


        bool Is_equals(HolderBase *other) const override {
            if (!Same_type(other)) return false;
            return IsEqual<T>::Compare(m_value, static_cast<Holder<T> *>(other)->m_value);
        }

        bool Is_less_than(HolderBase *other) const override {
            if (!Same_type(other)) throw std::runtime_error("Incompatible types");
            return IsLessThan<T>::Compare(m_value, static_cast<Holder<T> *>(other)->m_value);
        }


        T m_value;
    };

    HolderPtr m_holder;

};
