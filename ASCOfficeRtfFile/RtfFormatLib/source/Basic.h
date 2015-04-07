#pragma once

#include <vector>

#include "IdGenerator.h"
#include "RtfDefine.h"

#include <boost/shared_ptr.hpp>
 
typedef enum{ mu_none, mu_Auto, mu_Percent, mu_Twips } MetricUnits;


class RenderParameter
{
	public: 
		void* poWriter;
		void* poDocument;
		void* poRels;
		
		int nType;
		int nValue;
		CString sValue;
		int RtfType;
		
		RenderParameter()
		{
			poWriter = NULL;
			poDocument = NULL;
			poRels = NULL;
			nType = RENDER_TO_OOX_PARAM_UNKNOWN;
			nValue = PROP_DEF;
			RtfType = RTF_MAX;
		}
};

class IDocumentElement
{
public: 
		virtual int GetType()
		{
			return TYPE_UNKNOWN;
		}
		virtual bool IsValid()
		{
			return true;
		}
		virtual CString RenderToRtf(RenderParameter oRenderParameter) = 0;
		virtual CString RenderToOOX(RenderParameter oRenderParameter) = 0;
};

class IRenderableProperty: public IDocumentElement
{
public: 
	virtual void SetDefault() = 0;
	virtual void SetDefaultRtf() {}
	virtual void SetDefaultOOX() {}
};


template<class T> class ItemContainer
{
protected: 
	std::vector<T> m_aArray;
public: 
	ItemContainer( )
	{
	}
	ItemContainer( const ItemContainer& oItemContainer )
	{
		m_aArray.clear();
		m_aArray = oItemContainer.m_aArray;
	}
	const ItemContainer& operator=( const ItemContainer& oItemContainer )
	{
		m_aArray.clear();
		m_aArray = oItemContainer.m_aArray;
		return (*this);
	}
	T& operator[]( int nIndex )
	{
		return m_aArray[nIndex];
	}
	int AddItem( T piRend)
	{
		m_aArray.push_back(piRend);
		return (int)m_aArray.size() - 1;
	}
	int Find( T piRend )
	{
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			if( m_aArray[i] == piRend )
				return i;
		return -1;
	}
	void RemoveItem( T piRend )
	{
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			if( m_aArray[i] == piRend )
			{
				m_aArray.erase(m_aArray.begin()+i);
			}
	}
	void RemoveItem( int nIndex = -1 )
	{
		if( nIndex >= 0 && nIndex < (int)m_aArray.size() )
			m_aArray.erase(m_aArray.begin() + nIndex );
		else if( -1 == nIndex && 0 < (int)m_aArray.size() )
			m_aArray.pop_back();

	}
	void RemoveAll()
	{
		m_aArray.clear();
	}
	bool GetItem(T& oOutput,int nIndex = -1)
	{
		if( -1 == nIndex && (int)m_aArray.size() > 0 )
		{
			oOutput = m_aArray[m_aArray.size() - 1];
			return true;
		}
		if( nIndex >= 0 && nIndex < (int)m_aArray.size())
		{
			oOutput = m_aArray[nIndex];
			return true;
		}
		return false;
	}
	void InsertItem(T& oOutput,int nIndex = -1)
	{
		if( -1 == nIndex )
		{
			AddItem( oOutput );
		}
		if( nIndex >= 0 && nIndex <= (int)m_aArray.size())
		{
			m_aArray.insert(m_aArray.begin() + nIndex, oOutput);
		}
	}
	void SetItem(T& oOutput,int nIndex = -1)
	{
		if( -1 == nIndex )
		{
			AddItem( oOutput );
		}
		if( nIndex >= 0 && nIndex < (int)m_aArray.size())
		{
			m_aArray[ nIndex ] = oOutput;
		}
	}
	int GetCount()
	{
		return (int)m_aArray.size();
	}
};

template<class T> class ItemSingleContainer: public ItemContainer<T>
{
public: 
	int AddItem( T piRend)
	{
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			if( m_aArray[i] == piRend )
				return i;
		m_aArray.push_back(piRend);
		return (int)m_aArray.size() - 1;
	}
};

template<class T> class ItemWithLastContainer: public ItemContainer<T>
{
public:
	void RemoveItem( T piRend )
	{
		if( (int)m_aArray.size() > 1 )
		{
			ItemContainer::RemoveItem( piRend );
		}
	}
	void RemoveItem( int nIndex = -1 )
	{
		if( (int)m_aArray.size() > 1 )
		{
			ItemContainer::RemoveItem( nIndex );
		}
	}
};

class ITextItem : public IDocumentElement
{
};

typedef boost::shared_ptr<ITextItem> ITextItemPtr;
class ITextItemContainer : public IDocumentElement, public ItemContainer< ITextItemPtr >
{
};

typedef boost::shared_ptr<IDocumentElement> IDocumentElementPtr;
typedef boost::shared_ptr<ITextItemContainer> ITextItemContainerPtr;

class TextItemContainer : public ITextItemContainer
{
public: 
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
		{
			sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
			if( TYPE_RTF_PARAGRAPH == m_aArray[i]->GetType() && i != (int)m_aArray.size() - 1)
			{
				sResult += _T("\\par") ;
			}
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
		return sResult;
	}
	bool IsValid()
	{
		return m_aArray.size() > 0;
	}
};
typedef boost::shared_ptr<TextItemContainer> TextItemContainerPtr;
