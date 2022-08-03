#pragma once

template<class T>
class SingletonTemplate{
public:
   static T& GetInstance()
   {
      static T instance;
      return instance;
   }
protected:
   SingletonTemplate(){}
   SingletonTemplate(const SingletonTemplate&) = delete;
   SingletonTemplate& operator=(const SingletonTemplate&) = delete;
   virtual ~SingletonTemplate() {}
};

template<class T>
inline T& SingletonInstance()
{
   return SingletonTemplate<T>::GetInstance();
}
