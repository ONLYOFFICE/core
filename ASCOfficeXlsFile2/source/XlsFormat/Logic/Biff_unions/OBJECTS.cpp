
#include "OBJECTS.h"
#include <Logic/Biff_records/MsoDrawingSelection.h>
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_unions/TEXTOBJECT.h>
#include <Logic/Biff_unions/OBJ.h>
#include <Logic/Biff_unions/CHART.h>
#include <Logic/Biff_records/Continue.h>

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
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_OBJECTS_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		bool res =	proc.optional<OBJ>()		||
					proc.optional<TEXTOBJECT>()	||
					proc.optional<CHART>()		;

		return res;
	};
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

        Parenthesis_OBJECTS_2 parenthesis_objects_2;
        
		int count			= proc.repeated(parenthesis_objects_2, 0, 0);
		//int count_continue	= proc.repeated<Continue>(0,0);
		
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

	return count_1 > 0 || count_4 > 0;
}





} // namespace XLS

