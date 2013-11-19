#pragma once

template <class T> class Singleton
{
protected:

	Singleton() 
	{

	}

	virtual ~Singleton() 
	{
		m_pSelf = NULL; 
	}

public:
	static T* Instance();
	void FreeInstance();

private:

	static T* m_pSelf;
};

template <class T> T* Singleton<T>::m_pSelf = NULL;

template <class T> T* Singleton<T>::Instance ()
{
	if (NULL == m_pSelf)
		m_pSelf		=	new T;

	return m_pSelf;
}

template <class T> void Singleton<T>::FreeInstance()
{
	delete this;
}