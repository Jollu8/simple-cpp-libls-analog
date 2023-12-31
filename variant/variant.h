/*
Copyright (c) 2015 Jollu Emil.
All rights reserved.
 */



#pragma once

#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <cstring>
#include <type_traits>

#ifndef VARIANT_CAST_OPTIMIZATION
#define VARIANT_CAST_OPTIMIZATION 0
#endif

namespace VariantInternal
{

    struct NoOperator {};

    template<typename T, typename R> NoOperator operator==(T, R);
    template<typename T, typename R> NoOperator operator<(T, R);

    template <typename T>
    struct IsEqualOperatorExists
    {
        static const bool value = !std::is_same<NoOperator, decltype(*static_cast<T*>(0) == *static_cast<T*>(0))>::value;
    };

    template <typename T>
    struct IsLessOperatorExists
    {
        static const bool value = !std::is_same<NoOperator, decltype(*static_cast<T*>(0) < *static_cast<T*>(0))>::value;
    };

}

/**

\brief The boost::any analog.

Short example:

\code
    Variant v;
    v = 10;
    if (v.isType<int>())
        cout << v.value<int>() << endl;
\endcode

*/
class Variant
{
public:
    Variant() {}

    template <typename T>
    explicit Variant(const T& v)
            : holder_(std::make_shared<Holder<T>>(typeid(T).name(), v))
    {
    }

    template <typename T>
    static Variant fromValue(const T& v)
    {
        Variant res;
        res.holder_ = std::make_shared<Holder<T>>(typeid(T).name(), v);
        return res;
    }

    void clear()
    {
        holder_.reset();
    }

    template <typename T>
   bool isType() const
    {
#if VARIANT_CAST_OPTIMIZATION
        return holder_ ? typeid(T).name() == holder_->name : false;
#else
        return holder_ ? ::strcmp(typeid(T).name(), holder_->name) == 0 : false;
#endif
    }

    template <typename T>
    const T& valueRef() const
    {
        if (!isType<T>())
            throw std::bad_cast();
        return std::static_pointer_cast<Holder<T>>(holder_)->value;
    }

    template <typename T>
    T value() const
    {
        if (!holder_)
            return T();
        else
            return valueRef<T>();
    }

    template <typename T>
    explicit operator T() const
    {
        return valueRef<T>();
    }

    template <typename T>
    void setValue(const T& v)
    {
        holder_ = std::make_shared<Holder<T>>(typeid(T).name(), v);
    }

    template <typename T>
    void operator=(const T& v)
    {
        setValue(v);
    }

    template <typename T>
    bool tryGetValue(T*& val)
    {
        if (!isType<T>())
            return false;
        if (!holder_.unique())
            holder_ = std::make_shared<Holder<T>>(typeid(T).name(), std::static_pointer_cast<Holder<T>>(holder_)->value);
        val = &(std::static_pointer_cast<Holder<T>>(holder_)->value);
        return true;
    }

    template <typename T>
    bool tryGetValue(T& val)
    {
        return tryGetValue(&val);
    }

    [[nodiscard]] bool hasValue() const { return static_cast<bool>(holder_); }
    [[nodiscard]] bool isEmpty() const { return !holder_; }

    bool operator==(const Variant& other) const
    {
        return holder_->isEquals(other.holder_.get());
    }

    bool operator!=(const Variant& other) const
    {
        return !holder_->isEquals(other.holder_.get());
    }

    bool operator<(const Variant& other) const
    {
        return holder_->isLessThan(other.holder_.get());
    }

    bool operator>(const Variant& other) const
    {
        return other.holder_->isLessThan(holder_.get());
    }

    bool operator<=(const Variant& other) const
    {
        return !holder_->isLessThan(other.holder_.get());
    }

    bool operator>=(const Variant& other) const
    {
        return !other.holder_->isLessThan(holder_.get());
    }

private:
    template<class T, class Enable = void>
    struct IsEqual {
        // basic realization called if no equal operator defined
        static bool compare(const T&, const T&)
        {
            throw std::runtime_error("\"==\" operator is not defined for this type");
        }
    };

    template<class T>
    struct IsEqual<T, typename std::enable_if<VariantInternal::IsEqualOperatorExists<T>::value >::type> {
        // this realization called if equal operator defined
        static bool compare(const T& v1, const T& v2)
        {
            return v1 == v2;
        }
    };

    template<class T, class Enable = void>
    struct IsLessThan {
        static bool compare(const T&, const T&)
        {
            throw std::runtime_error("\"<\" operator is not defined for this type");
        }
    };

    template<class T>
    struct IsLessThan<T, typename std::enable_if<VariantInternal::IsLessOperatorExists<T>::value >::type> {
        static bool compare(const T& v1, const T& v2)
        {
            return v1 < v2;
        }
    };

    class HolderBase
    {
    public:
        explicit HolderBase(const char* name) : name(name) {}
        virtual ~HolderBase() = default;
        const char* name;

        bool sameType(HolderBase* other) const
        {
#if VARIANT_CAST_OPTIMIZATION
            return name == other->name;
#else
            return ::strcmp(name, other->name) == 0;
#endif
        }

        virtual bool isEquals(HolderBase* other) const = 0;
        virtual bool isLessThan(HolderBase* other) const = 0;
    };
    typedef std::shared_ptr<HolderBase> HolderPtr;

    template <typename T>
    class Holder : public HolderBase
    {
    public:
        Holder(const char* name, const T& v)
                : HolderBase(name)
                , value(v)
        {}

        bool isEquals(HolderBase* other) const override
        {
            if (!sameType(other))
                return false;
            return IsEqual<T>::compare(value, static_cast<Holder<T>*>(other)->value);
        }

        bool isLessThan(HolderBase* other) const override
        {
            if (!sameType(other))
                throw std::runtime_error("Incompatible types");
            return IsLessThan<T>::compare(value, static_cast<Holder<T>*>(other)->value);
        }

        T value;
    };

    HolderPtr holder_;
};
