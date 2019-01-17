/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace OOX
{	
	namespace Logic
	{	
		//--------------------------------------------------------------------------------
		// COMath 22.1.2.77 
		//--------------------------------------------------------------------------------	
		class CMathArgNodes : public WritingElementWithChilds<>
		{
		public:
			CMathArgNodes()
			{
				eType		= et_Unknown;
			}
			CMathArgNodes(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
				sNodeName	= GetMathNodeName(getType());
			}
			CMathArgNodes(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
				sNodeName	= GetMathNodeName(getType());
			}
			virtual ~CMathArgNodes()
			{
			}
			const CMathArgNodes &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CMathArgNodes &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const
			{
				if (sNodeName.empty()) return L"";

				std::wstring sResult = _T("<") + sNodeName + _T(">");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += _T("</") + sNodeName + _T(">");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return eType;
			}
		private:
			EElementType	eType;
		public:
			std::wstring	sNodeName;

			std::wstring GetMathNodeName(const EElementType & enumType)  const
			{//todooo вытащить в одно место - пересекается с MathBottomNodes
				switch(enumType)
				{
					case OOX::et_m_deg:		return L"m:deg";
					case OOX::et_m_den:		return L"m:den";
					case OOX::et_m_e:		return L"m:e";
					case OOX::et_m_fName:	return L"m:fName";
					case OOX::et_m_lim:		return L"m:lim";
					case OOX::et_m_num:		return L"m:num";
					case OOX::et_m_oMath:	return L"m:oMath";
					case OOX::et_m_sub:		return L"m:sub";
					case OOX::et_m_sup:		return L"m:sup";
                    default:
                        break;
                }
				return L"";
			}
			// Childs
		};		
	}//namespace Logic
}//namespace OOX
