#pragma once

#include <string>
#include <memory>

namespace std
{
    template<class T, class... Arg>
    std::unique_ptr<T> make_unique(Arg&&... args)
    {
        return std::unique_ptr<T>(new T(args...));
    }
}