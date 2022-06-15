#pragma once

#include <memory>
#include <vector>
#include <iterator>

template <class T>
class SVector
{
public:
    SVector(bool needInit = true);
    SVector(size_t reserve);
    SVector(size_t reserve, T initValue);
    inline void init() {isInit = true;}

    inline size_t size()const {return array.size();}
    inline void clear() {array.empty();}
    inline bool empty() {array.empty();}
    std::shared_ptr<T> operator[](size_t pos)const;
    void push_back(T&& value);
    inline void pop_back() {array.pop_back();}
    std::shared_ptr<T> back()const;
    std::shared_ptr<T> front()const;

    inline void erase(typename std::vector<std::shared_ptr<T>>::iterator &beg,
                      typename std::vector<std::shared_ptr<T>>::iterator &end) {array.erase(beg, end);}
    inline void erase(typename std::vector<std::shared_ptr<T>>::iterator &iter) {array.erase(iter);}

    inline typename std::vector<std::shared_ptr<T>>::iterator begin()   const {return array.begin();}
    inline typename std::vector<std::shared_ptr<T>>::iterator end()     const {return array.end();}

    inline bool IsInit() const {return isInit;};

private:
    bool isInit = false;
    std::vector<std::shared_ptr<T>> array;

};

