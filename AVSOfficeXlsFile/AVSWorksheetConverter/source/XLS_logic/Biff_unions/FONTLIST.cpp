#include "stdafx.h"
#include "FONTLIST.h"
#include <XLS_logic/Biff_records/FrtFontList.h>
#include <XLS_logic/Biff_records/StartObject.h>
#include <XLS_logic/Biff_records/Font.h>
#include <XLS_logic/Biff_records/Fbi.h>
#include <XLS_logic/Biff_records/EndObject.h>

namespace XLS
{;


FONTLIST::FONTLIST()
{
}


FONTLIST::~FONTLIST()
{
}


class Parenthesis_FONTLIST_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FONTLIST_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FONTLIST_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<Font>())
		{
			return false;
		}
		proc.optional<Fbi>();
		return true;
	};
};



BaseObjectPtr FONTLIST::clone()
{
	return BaseObjectPtr(new FONTLIST(*this));
}


// FONTLIST = FrtFontList StartObject *(Font [Fbi]) EndObject
const bool FONTLIST::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<FrtFontList>())
	{
		return false;
	}
	proc.mandatory<StartObject>();
	proc.repeated<Parenthesis_FONTLIST_1>(0, 0);
	proc.mandatory<EndObject>();

	return true;
}

} // namespace XLS

