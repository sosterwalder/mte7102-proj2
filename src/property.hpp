#pragma once

#include "main.hpp"

class BaseProperty
{
public:
    virtual ~BaseProperty() {}
};

template<typename T>
class Property : public BaseProperty
{
public:
    Property(T data) :
        BaseProperty(),
        mData(data) {}

    virtual ~Property() {}

    T data() { return mData; }

protected:
    T mData;
};

template<typename T>
class TypedProperties
{
    typedef std::map<std::string, std::pair<T, BaseProperty*>> mapping;

public:
    ~TypedProperties()
    {
        typename mapping::iterator iter;

        for(iter = mProperties.begin(); iter != mProperties.end(); ++iter) {
            auto p = (*iter).second;
            delete p.second;
        }

        mProperties.clear();
    }

    typename mapping::iterator begin()
    {
        return mProperties.begin();
    }

    typename mapping::iterator end()
    {
        return mProperties.end();
    }

    size_t size() { return mProperties.size(); }

    template<typename U>
    void add(std::string name, T type, U data)
    {
        mProperties[name] = std::make_pair(type, new Property<U>(data));
    }

    template<typename U>
    std::pair<std::string, U> data(std::string name)
    {
        auto property = mProperties[name];
        T type = property.first;
        Property<U>* p = (Property<U>*)property.second;

        return std::make_pair(type, p->data());
    }

protected:
    mapping mProperties;
};
