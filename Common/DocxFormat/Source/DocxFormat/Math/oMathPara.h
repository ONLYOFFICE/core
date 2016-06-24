/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#include "oMathContent.h"
#include "../Logic/Run.h"

namespace OOX
{	
	namespace Logic
	{
		//typedef class		CMathArgNodes COMath;
		//--------------------------------------------------------------------------------
		// COMathPara 22.1.2.78 (Office Math Paragraph) 
		//--------------------------------------------------------------------------------
		
		class COMathPara : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElement_AdditionConstructors(COMathPara)
			COMathPara()
			{
			}
			virtual ~COMathPara()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();				
					WritingElement *pItem = NULL;

					if ( _T("m:oMath") == sName )
						pItem = new COMath( oReader );
					else if ( _T("m:oMathParaPr") == sName )
						pItem = new COMathParaPr( oReader );
					else if ( _T("w:r") == sName )
						pItem = new CRun( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}
			virtual CString      toXML() const;
			

			virtual EElementType getType() const
			{
				return et_m_oMathPara;
			}
		};
	}//namespace Logic
}//namespace OOX