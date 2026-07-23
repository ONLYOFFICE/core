/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "IdGenerator.h"
#include "RtfDefine.h"

#include "../../OdfFile/Common/CPOptional.h"

enum _MetricUnits{ mu_none, mu_Auto, mu_Percent, mu_Twips };

class RenderParameter
{
	public: 
		void*	poWriter;
		void*	poDocument;
		void*	poRels;

		int		nType;
		int		nValue;
		int		RtfType;
		int		nParaStyleForeColor;	// fore color index of the applied paragraph style (PROP_DEF if none) - to block OOXML color inheritance

        std::wstring sValue;

		RenderParameter() : poWriter(NULL), poDocument(NULL), poRels(NULL)
		{
			nType		= RENDER_TO_OOX_PARAM_UNKNOWN;
			nValue		= PROP_DEF;
			RtfType		= RTF_MAX;
			nParaStyleForeColor = PROP_DEF;
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
        virtual std::wstring RenderToRtf(RenderParameter oRenderParameter) = 0;
        virtual std::wstring RenderToOOX(RenderParameter oRenderParameter) = 0;
};

class IRenderableProperty: public IDocumentElement
{
public: 
	virtual void SetDefault() = 0;
	virtual void SetDefaultRtf() {}
	virtual void SetDefaultOOX() {}
};


template<class T>
class ItemContainer
{
public:
    std::vector<T> m_aArray;

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
		//todooo - add map for seach

		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			if( m_aArray[i] == piRend )
				return (int)i;
		}
		return -1;
	}
	void RemoveItem( T piRend )
	{
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			if( m_aArray[i] == piRend )
			{
				m_aArray.erase(m_aArray.begin()+i);
			}
		}
	}
	void RemoveItem( int nIndex = -1 )
	{
		if( nIndex >= 0 && nIndex < (int)m_aArray.size() )
		{
			m_aArray.erase(m_aArray.begin() + nIndex );
		}
		else if( -1 == nIndex && 0 < (int)m_aArray.size() )
		{
			m_aArray.pop_back();
		}

	}
	void RemoveAll()
	{
		m_aArray.clear();
	}
	bool GetItem(T& oOutput,int nIndex = -1)
	{
		if( -1 == nIndex && !m_aArray.empty() )
		{
			oOutput = m_aArray.back();
			return true;
		}
		if( nIndex >= 0 && nIndex < (int)m_aArray.size())
		{
			oOutput = m_aArray[nIndex];
			return true;
		}
		else if (!m_aArray.empty())
		{
			oOutput = m_aArray[0]; // default
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
	//todooo -> to map with hash
    int AddItem( T piRend)
	{
        for( int i = 0; i < (int)ItemContainer<T>::m_aArray.size(); i++ )
		{
            if( ItemContainer<T>::m_aArray[i] == piRend )
				return i;
		}
        ItemContainer<T>::m_aArray.push_back(piRend);
        return (int)ItemContainer<T>::m_aArray.size() - 1;
	}
};

template<class T> class ItemWithLastContainer: public ItemContainer<T>
{
public:
    void RemoveItem( T piRend )
	{
        if( ItemContainer<T>::m_aArray.size() > 1 )
		{
            ItemContainer<T>::RemoveItem( piRend );
		}
	}
    void RemoveItem( int nIndex = -1 )
	{
        if( ItemContainer<T>::m_aArray.size() > 1 )
		{
            ItemContainer<T>::RemoveItem( nIndex );
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter)
	{
        std::wstring sResult;
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToRtf( oRenderParameter );

			if( TYPE_RTF_PARAGRAPH == m_aArray[i]->GetType() && i != (int)m_aArray.size() - 1)
			{
				sResult += L"\\par";
			}
		}
		return sResult;
	}
    std::wstring RenderToOOX(RenderParameter oRenderParameter)
	{
        std::wstring sResult;
       
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
            sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
		}

		return sResult;
	}
	bool IsValid()
	{
		return m_aArray.size() > 0;
	}
};
typedef boost::shared_ptr<TextItemContainer> TextItemContainerPtr;
