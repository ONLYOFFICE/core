
#include "OBJECTS.h"
#include <Logic/Biff_records/MsoDrawingSelection.h>
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_unions/TEXTOBJECT.h>
#include <Logic/Biff_unions/OBJ.h>
#include <Logic/Biff_unions/CHART.h>
//#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{;


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
		return	proc.optional<TEXTOBJECT>() ||
				proc.optional<OBJ>() ||
				proc.optional<CHART>();
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
		if(!proc.mandatory(*mso_drawing_))
		{
			return false;
		}
		proc.repeated(Parenthesis_OBJECTS_2(), 0, 0);
		return true;
	}



private:
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
	MsoDrawingPtr mso_drawing(new MsoDrawing(is_inside_chart_sheet_));
	bool res1 = proc.repeated(Parenthesis_OBJECTS_1(mso_drawing), 0, 0);
	bool res2 = proc.optional(MsoDrawingSelection());

	return res1 || res2;
}





} // namespace XLS

