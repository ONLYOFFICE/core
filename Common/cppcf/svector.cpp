#include "svector.h"

template<class T>
SVector<T>::SVector(size_t reserve, T initValue) : array(reserve, initValue)
{
    init();
}

template<class T>
SVector<T>::SVector(bool needInit)
{
    isInit = needInit;
}

template<class T>
SVector<T>::SVector(size_t reserve) : array(reserve)
{
    init();
}

template<class T>
std::shared_ptr<T> SVector<T>::operator[](size_t pos) const
{
    if (!isInit || array.size() <= pos)
        return {};

    return array[pos];
}

template<class T>
void SVector<T>::push_back(T &&value)
{
    isInit = true;
    array.push_back(std::shared_ptr<T>(value));
}

template<class T>
void SVector<T>::push_back(std::shared_ptr<T> pValue)
{
    isInit = true;
    array.emplace_back(pValue);
}

template<class T>
void SVector<T>::push_back(const T &value)
{
    isInit = true;
    array.push_back(std::shared_ptr<T>(value));
}

template<class T>
std::shared_ptr<T> SVector<T>::back() const
{
    if (!isInit || array.empty())
        return {};

    return array.back();
}

template<class T>
std::shared_ptr<T> SVector<T>::front() const
{
    if (!isInit || array.empty())
        return {};

    return array.front();
}

template<class T>
std::shared_ptr<T> SVector<T>::dequeue()
{
    auto pElement = front();
    array.erase(array.begin());
    return pElement;
}

template<class T>
template<class P>
SVector<P> SVector<T>::cast() const
{
    SVector<P> res(array.size());
    for (auto& spT : array)
        res.push_back(std::dynamic_pointer_cast<P> (spT));

    return res;
}
