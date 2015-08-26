
#include "OBJECTS.h"
#include <Logic/Biff_records/MsoDrawingSelection.h>
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_unions/TEXTOBJECT.h>
#include <Logic/Biff_unions/OBJ.h>
#include <Logic/Biff_unions/CHART.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/Obj.h>

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
		throw;// EXCEPT::LE::UnexpectedProgramPath("Wrong loadContent usage", __FUNCTION__);
	}


	const bool loadContentRead(BinReaderProcessor& proc)
	{
		bool res = proc.mandatory(*mso_drawing_);

        Parenthesis_OBJECTS_2 parenthesis_objects_2(mso_drawing_);
        
		int count			= proc.repeated(parenthesis_objects_2, 0, 0);
		
		return res || count>0;
	}

	MsoDrawingPtr mso_drawing_;
};



BaseObjectPtr OBJECTS::clone()
{
	return BaseObjectPtr(new OBJECTS(*this));
}


const bool OBJECTS::loadContent(BinProcessor& proc)
{
	throw;// EXCEPT::LE::UnexpectedProgramPath("Wrong loadContent usage", __FUNCTION__);
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

	//int count_2 = proc.repeated(parenthesis_objects_1, 0, 0);

	if (m_MsoDrawing->isReading == false)
	{
		try
		{
			m_MsoDrawing->resetToBegin();
			m_MsoDrawing->readFields();		
		}
		catch(...)
		{
		}
	}

	return count_1 > 0 || count_4 > 0;
}





} // namespace XLS

