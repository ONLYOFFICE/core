﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
	namespace Spreadsheet
	{
            class CPivotCacheDefinitionExt : public WritingElement
            {
            public:
                WritingElement_AdditionMethods(CPivotCacheDefinitionExt)
                WritingElement_XlsbConstructors(CPivotCacheDefinitionExt)
                CPivotCacheDefinitionExt()
                {
                }
                virtual ~CPivotCacheDefinitionExt()
                {
                }

                virtual void fromXML(XmlUtils::CXmlNode& node)
                {
                }
                virtual std::wstring toXML() const
                {
                    return L"";
                }
                virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
                virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
                virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
                void fromBin(XLS::BaseObjectPtr& obj);
                XLS::BaseObjectPtr toBin();
                virtual EElementType getType () const
                {
                    return et_x_PivotCacheDefinitionExt;
                }
                void ReadAttributes(XLS::BaseObjectPtr& obj);
                void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        //----------
                nullable_bool					m_oSlicerData;
                nullable_bool					m_oSrvSupportSubQueryCalcMem;
				nullable_bool                   m_oSrvSupportSubQueryNonVisual;
                nullable_bool					m_oSrvSupportAddCalcMems;

				nullable_uint                   m_oPivotCacheId;

            };
	} //Spreadsheet
} // namespace OOX
