#pragma once

template <class T> class Singleton
{
  private:
    static T* _self;
  
  protected:
    Singleton() {}
	virtual ~Singleton() { _self = NULL; }
  
  public:
    static T* Instance();
    void FreeInstance();
};

template <class T> T* Singleton<T>::_self = NULL;

template <class T> T* Singleton<T>::Instance()
{
  if( _self == NULL )
  {
    _self = new T;
  }

  return _self;
}

template <class T> void Singleton<T>::FreeInstance()
{
  delete this;
}