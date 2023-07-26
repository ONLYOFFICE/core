/*
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

#include "XFPropGradient.h"
#include "Xnum.h"

#include "../../../../../OOXML/Base/Unit.h"
#include "../../../../../DesktopEditor/xml/include/xmlutils.h"

namespace XLS
{


BiffStructurePtr XFPropGradient::clone()
{
	return BiffStructurePtr(new XFPropGradient(*this));
}

void XFPropGradient::load(CFRecord& record)
{
	_UINT32 temp;	

    if(record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> temp >> numDegree >> numFillToLeft >> numFillToRight >> numFillToTop >> numFillToBottom;

        type1 = temp;
    }
    else
    {
        Xnum numDegree_;
        Xnum numFillToLeft_;
        Xnum numFillToRight_;
        Xnum numFillToTop_;
        Xnum numFillToBottom_;

        record >> temp >> numDegree_ >> numFillToLeft_ >> numFillToRight_ >> numFillToTop_ >> numFillToBottom_;

        type1 = temp;

        numDegree = numDegree_.data.value;
        numFillToLeft = numFillToLeft_.data.value;
        numFillToRight = numFillToRight_.data.value;
        numFillToTop = numFillToTop_.data.value;
        numFillToBottom = numFillToBottom_.data.value;
    }
}

void XFPropGradient::save(CFRecord& record)
{
	_UINT32 temp = 0;

	if (type1)
		temp = 1;

	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		record << temp << numDegree << numFillToLeft << numFillToRight << numFillToTop << numFillToBottom;
	}
	else
	{
		Xnum numDegree_;
		Xnum numFillToLeft_;
		Xnum numFillToRight_;
		Xnum numFillToTop_;
		Xnum numFillToBottom_;

		numDegree_.data.value = numDegree;
		numFillToLeft_.data.value = numFillToLeft;
		numFillToRight_.data.value = numFillToRight;
		numFillToTop_.data.value = numFillToTop;
		numFillToBottom_.data.value = numFillToBottom;

		record << temp << numDegree_ << numFillToLeft_ << numFillToRight_ << numFillToTop_ << numFillToBottom_;
	}
}

void XFPropGradient::serialize_attr(CP_ATTR_NODE)
{
	CP_XML_ATTR(L"type", type1);
	CP_XML_ATTR(L"degree", numDegree);
	CP_XML_ATTR(L"left", numFillToLeft);
	CP_XML_ATTR(L"right", numFillToRight);
	CP_XML_ATTR(L"top", numFillToTop);
	CP_XML_ATTR(L"bottom", numFillToBottom);
}

int XFPropGradient::deserialize(XmlUtils::CXmlLiteReader& oReader)
{
	type1 = false;
	numDegree = 0;
	numFillToLeft = 0;
	numFillToRight = 0;
	numFillToTop = 0;
	numFillToBottom = 0;

	if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute() == true)
	{
		std::wstring wsPropName = oReader.GetName();

		while (!wsPropName.empty())
		{
			if (wsPropName == L"type")
				type1 = XmlUtils::GetBoolean(oReader.GetText());
			else if (wsPropName == L"degree")
				numDegree = XmlUtils::GetDouble(oReader.GetText());
			else if (wsPropName == L"left")
				numFillToLeft = XmlUtils::GetDouble(oReader.GetText());
			else if (wsPropName == L"right")
				numFillToRight = XmlUtils::GetDouble(oReader.GetText());
			else if (wsPropName == L"top")
				numFillToTop = XmlUtils::GetDouble(oReader.GetText());
			else if (wsPropName == L"bottom")
				numFillToBottom = XmlUtils::GetDouble(oReader.GetText());

			if (!oReader.MoveToNextAttribute())
				break;

			wsPropName = oReader.GetName();
		}

		oReader.MoveToElement();
	}
	return 0;
}


} // namespace XLS
