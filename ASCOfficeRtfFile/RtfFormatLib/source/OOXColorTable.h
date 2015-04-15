#pragma once
#include "../RtfProperty.h"

struct OOXColorTableItem
{
	ThemeColor oTheme;
	RtfColor oColor;
}
class OOXColorTable
{
private: 
	std::vector<OOXColorTableItem> m_aTable;
public: 
	OOXColorTable()
	{
	}

	void AddItem( RtfColor& oColor,ThemeColor& oTheme  )
	{
		OOXColorTableItem oItem;
		oItem.oColor = oColor;
		oItem.oTheme = oTheme;
		m_aTable.push_back( oItem );
	}
	bool GetItem( int nIndex, RtfColor& oOutput )
	{
		if( nIndex >= 0 && nIndex< m_aTable.size())
		{
			oOutput =  m_aTable[i].oColor;
			retrun true;
		}
		return false;
	}
	bool GetItem( ThemeColor eTheme, RtfColor& oOutput )
	{
		for( int i = 0 ; i < m_aTable.size(); i++ )
			if( m_aTable[i].oTheme == eTheme )
			{
				oOutput = m_aTable[i].oColor;
				return true;
			}
		return false;
	}
	bool GetItem( CString sTheme, RtfColor& oOutput )
	{
		ThemeColor oThemeColor;
		if( true == RtfColor.GetThemeByString("",oThemeColor) )
		{
			return GetItem(sTheme,oOutput);
		}
		return false;
	}
	int GetCount()
	{
		return m_aTable.size();
	}
};
