#pragma once

#include <XmlUtils.h>

#include "X:\AVS\Sources\AVSImageStudio3\AVSImageEditor\AVSImageEditor\ViewManager.h"

namespace ImageRendering
{
	namespace Serialize
	{
		class XmlViewManagerSettings
		{
		public:

			inline static bool SetRullers		( ImageRendering::CViewManager& oManager, BSTR XmlString )
			{
				CXmlNode oXmlNode;
				if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
				{
					///
					/// <rullers type="0" left="1" top="1" right="1" bottom="1" unit_w="100" unit_h="100" unit_range="0" align_center="0"/>
					///

					long RullerType		=	_tstol ( oXmlNode.GetAttribute ( _T("type") ) );	
					long RullerLeft		=	_tstol ( oXmlNode.GetAttribute ( _T("left") ) );
					long RullerTop		=	_tstol ( oXmlNode.GetAttribute ( _T("top") ) );
					long RullerRight	=	_tstol ( oXmlNode.GetAttribute ( _T("right") ) );
					long RullerBottom	=	_tstol ( oXmlNode.GetAttribute ( _T("bottom") ) );

					long UnitW			=	_tstol ( oXmlNode.GetAttribute ( _T("unit_w"), _T("100") ) );
					long UnitH			=	_tstol ( oXmlNode.GetAttribute ( _T("unit_h"), _T("100") ) );
					long UnitRange		=	_tstol ( oXmlNode.GetAttribute ( _T("unit_range"), _T("50") ) );

					long AlignCenter	=	_tstol ( oXmlNode.GetAttribute ( _T("align_center"), _T("0")) );

					oManager.GetRullers ().SetRullerType	( RullerType );
					oManager.SetEnableRullerUp		( ( RullerTop )	? true : false );	
					oManager.SetEnableRullerDown	( ( RullerBottom ) ? true : false );	
					oManager.SetEnableRullerRight	( ( RullerRight )	? true : false );	
					oManager.SetEnableRullerLeft	( ( RullerLeft )	? true : false );	
					oManager.GetRullers ().SetAlignCenter	( ( AlignCenter ) ? true : false );

					if ( 3 == RullerType )
					{
						oManager.GetRullers ().SetCustomUnitW ( UnitW );
						oManager.GetRullers ().SetCustomUnitH ( UnitH );
						oManager.GetRullers ().SetCustomUnitRange (	( (double)UnitRange ) / 100.0 );
					}

					oManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );

					return true;
				}

				return false;
			}

			inline static bool SetGridOptions	( ImageRendering::CViewManager& oManager, BSTR XmlString )
			{
				CXmlNode oXmlNode;
				if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
				{
					///
					/// <grid visible="1" fullfill="1" size="10" forecolor="0x0"  backcolor="0xFF000000" width="1" align_center="0"/>
					///

					long Size			=	_tstol ( oXmlNode.GetAttribute ( _T("size") ) );
					long ShowLines		=	_tstol ( oXmlNode.GetAttribute ( _T("visible"), _T("0") ) );
					long FullFill		=	_tstol ( oXmlNode.GetAttribute ( _T("fullfill"), _T("0") ) );
					DWORD ForeColor		=	static_cast <DWORD> ( _tstof ( oXmlNode.GetAttribute ( _T("forecolor") ) ) );
					DWORD BackColor		=	static_cast <DWORD> ( _tstof ( oXmlNode.GetAttribute ( _T("backcolor") ) ) );
					double Width		=	_tstof ( oXmlNode.GetAttribute ( _T("width") ) );
					long AlignCenter	=	_tstol ( oXmlNode.GetAttribute ( _T("align_center"), _T("0") ) );

					oManager.SetEnableGrid ( ( ShowLines ) ? true : false );
					oManager.GetGridRender ().SetFullFillLines ( ( FullFill ) ? true : false ); 
					oManager.GetGridRender ().SetCellSize (	Size );
					oManager.GetGridRender ().UpdateLinesPen ( ForeColor, BackColor, (float)Width );
					oManager.GetGridRender ().SetAlignCenter ( ( AlignCenter ) ? true : false );

					oManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );

					return true;
				}
				return false;
			}

			inline static bool SetRenderModes	( ImageRendering::CViewManager& oManager, BSTR XmlString )
			{
				CXmlNode oXmlNode;
				if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
				{
					CXmlNodes oNodes;
					if ( oXmlNode.GetNodes ( _T("mode"), oNodes ) )
					{
						for ( int Ind = 0; Ind < oNodes.GetCount(); ++Ind )
						{
							CXmlNode oModeNode;
							if ( oNodes.GetAt ( Ind, oModeNode ) )
							{
								long Index		=	_tstol ( oModeNode.GetAttribute ( _T("ind") ) );
								long Enable		=	_tstol ( oModeNode.GetAttribute ( _T("enable") ) );

								oManager.SetRenderMode ( Index, Enable );
							}
						}
					}

					oManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );
					return true;
				}

				return false;
			}
		};
	}
}
