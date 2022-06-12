#ifndef SINGLETONTEMPLATE_H
#define SINGLETONTEMPLATE_H

#ifdef __cplusplus

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

#endif /*__cplusplus*/

#endif // SINGLETONTEMPLATE_H
