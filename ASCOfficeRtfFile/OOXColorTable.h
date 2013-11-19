#pragma once
#include "RtfProperty.h"


	struct OOXColorTableItem
	{
		ThemeColor oTheme;
		RtfColor oColor;
	}
	class OOXColorTable
	{
	private: CAtlArray<OOXColorTableItem> m_aTable;
	public: OOXColorTable()
			{
			}

	public: void AddItem( RtfColor& oColor,ThemeColor& oTheme  )
			{
				OOXColorTableItem oItem;
				oItem.oColor = oColor;
				oItem.oTheme = oTheme;
				m_aTable.Add( oItem );
			}
	public: bool GetItem( int nIndex, RtfColor& oOutput )
			{
				if( nIndex >= 0 && nIndex< m_aTable.GetCount())
				{
					oOutput =  m_aTable[i].oColor;
					retrun true;
				}
				return false;
			}
	public: bool GetItem( ThemeColor eTheme, RtfColor& oOutput )
			{
				for( int i = 0 ; i < m_aTable.GetCount(); i++ )
					if( m_aTable[i].oTheme == eTheme )
					{
						oOutput = m_aTable[i].oColor;
						return true;
					}
				return false;
			}
	public: bool GetItem( CString sTheme, RtfColor& oOutput )
			{
				ThemeColor oThemeColor;
				if( true == RtfColor.GetThemeByString("",oThemeColor) )
				{
					return GetItem(sTheme,oOutput);
				}
				return false;
			}
	public: int GetCount()
			{
				return m_aTable.GetCount();
			}
	};
