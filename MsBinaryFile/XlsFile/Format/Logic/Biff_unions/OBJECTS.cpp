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

#include "OBJECTS.h"
#include "TEXTOBJECT.h"
#include "OBJ.h"
#include "CHART.h"
#include "IMDATAOBJECT.h"

#include "../Biff_records/MsoDrawingSelection.h"
#include "../Biff_records/MsoDrawing.h"
#include "../Biff_records/Continue.h"
#include "../Biff_records/Obj.h"
#include "../Biff_records/IMDATA.h"

namespace XLS
{


OBJECTS::OBJECTS(const bool is_inside_chart_sheet)
:	is_inside_chart_sheet_(is_inside_chart_sheet)
{
}


OBJECTS::~OBJECTS()
{
}


class Parenthesis_OBJECTS_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_OBJECTS_2)
public:
	Parenthesis_OBJECTS_2(MsoDrawingPtr mso_drawing)  : mso_drawing_(mso_drawing)
	{
	}
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_OBJECTS_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		OBJ OBJ_(mso_drawing_);
		bool res1 =	proc.optional(OBJ_);

		TEXTOBJECT TEXTOBJECT_(mso_drawing_);
		bool res2 =	proc.optional(TEXTOBJECT_);

		CHART CHART_(mso_drawing_);
		bool res3 = proc.optional(CHART_);

		if (proc.getGlobalWorkbookInfo()->Version < 0x0600)
		{
			res3 = res3 || proc.optional<IMDATAOBJECT>();
		}

		return res1 || res2 || res3;
	}
	
	MsoDrawingPtr mso_drawing_;
};


class Parenthesis_OBJECTS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_OBJECTS_1)
public:

	Parenthesis_OBJECTS_1(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing)
	{
	}

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_OBJECTS_1(mso_drawing_));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return false;
	}


	const bool loadContentRead(BinReaderProcessor& proc)
	{
		bool res = proc.optional(*mso_drawing_);

        Parenthesis_OBJECTS_2 parenthesis_objects_2(mso_drawing_);
        
		int count2 = proc.repeated(parenthesis_objects_2, 0, 0);
		
		return res || (count2>0);
	}

	MsoDrawingPtr mso_drawing_;
};



BaseObjectPtr OBJECTS::clone()
{
	return BaseObjectPtr(new OBJECTS(*this));
}


const bool OBJECTS::loadContent(BinProcessor& proc)
{
	return false;
}


// OBJECTS = *(MSODRAWING *(TEXTOBJECT / OBJ / CHART)) [MsoDrawingSelection]
const bool OBJECTS::loadContentRead(BinReaderProcessor& proc)
{
	m_MsoDrawing = boost::shared_ptr<MsoDrawing>(new MsoDrawing(is_inside_chart_sheet_));
	
    Parenthesis_OBJECTS_1 parenthesis_objects_1(m_MsoDrawing);

    int count_1 = proc.repeated(parenthesis_objects_1, 0, 0);

    MsoDrawingSelection mso_drawing_selection;
    int count_4  = proc.optional(mso_drawing_selection);
	
	for (std::list<BaseObjectPtr>::iterator elem = elements_.begin(); elem != elements_.end(); elem++)
	{
		if (elem->get()->get_type() == typeMsoDrawing)
		{
			elements_.erase(elem);
			break;
		}
	}

	try
	{
		m_MsoDrawing->resetToBegin();
		m_MsoDrawing->readFields();		
	}
	catch(...)
	{
	}

	return count_1 > 0 || count_4 > 0;
}





} // namespace XLS

