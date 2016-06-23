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
#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// 21.1.2.2.11 spcPct (Spacing Percent)
		//--------------------------------------------------------------------------------	
		class CLineSpacingPercent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineSpacingPercent)
			CLineSpacingPercent()
			{
			}
			virtual ~CLineSpacingPercent()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    CString sName = oReader.GetName();

				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_spcPct;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("val"),        m_oVal )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<SimpleTypes::CTextSpacingPercentOrPercentString>		m_oVal;
		};
		//--------------------------------------------------------------------------------
		// 21.1.2.2.5 lnSpc (Line Spacing)
		//--------------------------------------------------------------------------------	
		class CLineSpacingPoints : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineSpacingPoints)
			CLineSpacingPoints()
			{
			}
			virtual ~CLineSpacingPoints()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    CString sName = oReader.GetName();
					
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_spcPts;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("val"),        m_oVal )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<SimpleTypes::CTextSpacingPoint<>>		m_oVal;

		};
		//--------------------------------------------------------------------------------
		// 21.1.2.2.5 lnSpc (Line Spacing), 21.1.2.2.9 spcAft (Space After), 21.1.2.2.10 spcBef (Space Before)
		//--------------------------------------------------------------------------------	
		class CLineSpacing : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineSpacing)
			CLineSpacing()
			{
			}
			virtual ~CLineSpacing()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{

				// TO DO
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    CString sName = oReader.GetName();
					
					if ( _T("a:spcPct") == sName )
						m_oLineSpacingPercent = oReader;
					if ( _T("a:spcPts") == sName )
						m_oLineSpacingPoints = oReader;
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_lnSpc;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			// Childs
			nullable<CLineSpacingPercent>		m_oLineSpacingPercent;
			nullable<CLineSpacingPoints>		m_oLineSpacingPoints;
		};
	} // namespace Drawing
} // namespace OOX
