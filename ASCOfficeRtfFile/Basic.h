#pragma once
#include <atlcoll.h>
#include "XmlUtils.h"
#include "IdGenerator.h"
#include "RtfDefine.h"
#include <boost/shared_ptr.hpp>

 
typedef enum{ mu_none, mu_Auto, mu_Percent, mu_Twips } MetricUnits;

class IEventSource
{
public: virtual void RaiseOnProcess( long nValue, short* nCancel ) = 0;
};
class RenderParameter
{
public: void* poWriter;
public: void* poDocument;
public: void* poRels;
public: int nType;
public: int nValue;
public: CString sValue;
public: int RtfType;
public: RenderParameter()
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
public: virtual int GetType()
		{
			return TYPE_UNKNOWN;
		}
public: virtual bool IsValid()
		{
			return true;
		}
public: virtual CString RenderToRtf(RenderParameter oRenderParameter) = 0;
public: virtual CString RenderToOOX(RenderParameter oRenderParameter) = 0;
};
class IRenderableProperty: public IDocumentElement
{
public: virtual void SetDefault() = 0;
//public: virtual void SetDefaultRtf() = 0;
//public: virtual void SetDefaultOOX() = 0;
};

template<class T> class ItemContainer
{
protected: CAtlArray<T> m_aArray;
public: ItemContainer( )
		{
		}
public: ItemContainer( const ItemContainer& oItemContainer )
		{
			m_aArray.RemoveAll();
			m_aArray.Append( oItemContainer.m_aArray );
		}
public: const ItemContainer& operator=( const ItemContainer& oItemContainer )
		{
			m_aArray.RemoveAll();
			m_aArray.Append( oItemContainer.m_aArray );
			return (*this);
		}
public: T& operator[]( int nIndex )
		{
			return m_aArray[nIndex];
		}
public: int AddItem( T piRend)
		{
			m_aArray.Add(piRend);
			return (int)m_aArray.GetCount() - 1;
		}
public: int Find( T piRend )
		{
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				if( m_aArray[i] == piRend )
					return i;
			return -1;
		}
public: void RemoveItem( T piRend )
		{
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				if( m_aArray[i] == piRend )
					m_aArray.RemoveAt(i);
		}
public: void RemoveItem( int nIndex = -1 )
		{
			if( nIndex >= 0 && nIndex < (int)m_aArray.GetCount() )
				m_aArray.RemoveAt( nIndex );
			else if( -1 == nIndex && 0 < (int)m_aArray.GetCount() )
				m_aArray.RemoveAt( (int)m_aArray.GetCount() - 1 );

		}
public: void RemoveAll()
		{
			m_aArray.RemoveAll();
		}
public: bool GetItem(T& oOutput,int nIndex = -1)
		{
			if( -1 == nIndex && (int)m_aArray.GetCount() > 0 )
			{
				oOutput = m_aArray[m_aArray.GetCount() - 1];
				return true;
			}
			if( nIndex >= 0 && nIndex < (int)m_aArray.GetCount())
			{
				oOutput = m_aArray[nIndex];
				return true;
			}
			return false;
		}
public: void InsertItem(T& oOutput,int nIndex = -1)
		{
			if( -1 == nIndex )
			{
				AddItem( oOutput );
			}
			if( nIndex >= 0 && nIndex <= (int)m_aArray.GetCount())
			{
				m_aArray.InsertAt( nIndex, oOutput);
			}
		}
public: void SetItem(T& oOutput,int nIndex = -1)
		{
			if( -1 == nIndex )
			{
				AddItem( oOutput );
			}
			if( nIndex >= 0 && nIndex < (int)m_aArray.GetCount())
			{
				m_aArray[ nIndex ] = oOutput;
			}
		}
public: int GetCount()
		{
			return (int)m_aArray.GetCount();
		}
};
template<class T> class ItemSingleContainer: public ItemContainer<T>
{
public: int AddItem( T piRend)
		{
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				if( m_aArray[i] == piRend )
					return i;
			m_aArray.Add(piRend);
			return (int)m_aArray.GetCount() - 1;
		}
};
template<class T> class ItemWithLastContainer: public ItemContainer<T>
{
public: void RemoveItem( T piRend )
		{
			if( (int)m_aArray.GetCount() > 1 )
			{
				ItemContainer::RemoveItem( piRend );
			}
		}
public: void RemoveItem( int nIndex = -1 )
		{
			if( (int)m_aArray.GetCount() > 1 )
			{
				ItemContainer::RemoveItem( nIndex );
			}
		}
};
class ITextItem : public IDocumentElement
{
};
typedef boost::shared_ptr<ITextItem> ITextItemPtr;
class ITextItemContainer : public IDocumentElement,public ItemContainer< ITextItemPtr >
{
};

typedef boost::shared_ptr<IDocumentElement> IDocumentElementPtr;
typedef boost::shared_ptr<ITextItemContainer> ITextItemContainerPtr;

class TextItemContainer : public ITextItemContainer
{
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
			{
				sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
				if( TYPE_RTF_PARAGRAPH == m_aArray[i]->GetType() && i != (int)m_aArray.GetCount() - 1)
				{
					sResult.Append( _T("\\par") );
				}
			}
			return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				sResult.Append( m_aArray[i]->RenderToOOX(oRenderParameter) );
			return sResult;
		}
public: bool IsValid()
		{
			return m_aArray.GetCount() > 0;
		}
};
typedef boost::shared_ptr<TextItemContainer> TextItemContainerPtr;