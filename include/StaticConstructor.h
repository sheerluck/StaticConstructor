/* MIT License

Copyright (c) 2016 Kevin Lalumiere

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#ifndef STATICCONSTRUCTOR_H
#define STATICCONSTRUCTOR_H

#include <cassert>
#include <utility>
#include <type_traits>

template<class Type>
class StaticConstructor {
public:
    StaticConstructor() = default;
    template<class FirstArgumentType, typename std::enable_if<!std::is_same<StaticConstructor<Type>,
            typename std::remove_cv<typename std::remove_reference<FirstArgumentType>::type>::type>::value,
        bool>::type = true, typename... Args>
    explicit StaticConstructor(FirstArgumentType&& x, Args&&... args)
    { construct(std::forward<FirstArgumentType>(x),std::forward<Args>(args)...); }
    StaticConstructor(const StaticConstructor<Type> &rhs) {
        if(rhs.isConstructed()) construct(*rhs.data);
    }
    StaticConstructor(StaticConstructor<Type> &&rhs) {
        if(rhs.isConstructed()) construct(std::move(*rhs.data));
    }
    ~StaticConstructor() {
        destruct();
    }
    StaticConstructor &operator=(const StaticConstructor<Type> &rhs) {
        if(&rhs == this) return *this;
        destruct();
        if(rhs.isConstructed()) construct(*rhs.data);
        return *this;
    }
    StaticConstructor &operator=(StaticConstructor<Type> &&rhs) {
        if(&rhs == this) return *this;
        destruct();
        if(rhs.isConstructed()) construct(std::move(*rhs.data));
        return *this;
    }
    template<typename... Args>
    void construct(Args&&... args) {
        assert(!isConstructed());
        data = new(&stackMemory) Type{std::forward<Args>(args)...};
    }
    void destruct() {
        if(isConstructed()) data->~Type();
        data = nullptr;
    }
    Type &get() const {
        assert(isConstructed());
        return *data;
    }
    bool isConstructed() const {
        return data != nullptr;
    }

private:
    typename std::aligned_storage<sizeof(Type), alignof(Type)>::type stackMemory;
    typename std::add_pointer<Type>::type data = nullptr;
};

template<class Type, typename... Args>
inline StaticConstructor<Type> make_StaticConstructor(Args&&... args) {
    return StaticConstructor<Type>{std::forward<Args>(args)...};
}

#endif  /* STATICCONSTRUCTOR_H */
