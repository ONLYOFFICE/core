#pragma once

template <class T> class Singleton
{
protected:

	Singleton() 
	{

	}

	virtual ~Singleton() 
	{
		m_pSelf = 0; 
	}

public:
	static T* Instance();
	void FreeInstance();

private:

	static T* m_pSelf;
};

template <class T> T* Singleton<T>::m_pSelf = 0;

template <class T> T* Singleton<T>::Instance ()
{
	if (0 == m_pSelf)
		m_pSelf		=	new T;

	return m_pSelf;
}

template <class T> void Singleton<T>::FreeInstance()
{
	delete this;
}