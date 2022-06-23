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
   SingletonTemplate(const SingletonTemplate&);
   SingletonTemplate& operator=(const SingletonTemplate&);
   virtual ~SingletonTemplate() {}
};

template<class T>
inline T& SingletonInstance()
{
   return SingletonTemplate<T>::GetInstance();
}
