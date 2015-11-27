
#include "AXES.h"
#include <Logic/Biff_records/PlotArea.h>

#include <Logic/Biff_unions/IVAXIS.h>
#include <Logic/Biff_unions/DVAXIS.h>
#include <Logic/Biff_unions/SERIESAXIS.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/FRAME.h>

namespace XLS
{


AXES::AXES()
{
}


AXES::~AXES()
{
}


//  (DVAXIS DVAXIS)
class Parenthesis_AXES_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXES_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXES_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<DVAXIS>())
		{
			return false;
		}
		proc.mandatory<DVAXIS>();
		return true;
	};
};



// (IVAXIS DVAXIS [SERIESAXIS])
class Parenthesis_AXES_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXES_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXES_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<IVAXIS>())
		{
			return false;
		}

		proc.mandatory<DVAXIS>();
		proc.optional<SERIESAXIS>();
		return true;
	};
};



//  [IVAXIS DVAXIS [SERIESAXIS] / DVAXIS DVAXIS]
// this is equal to [(IVAXIS DVAXIS [SERIESAXIS]) / (DVAXIS DVAXIS)]
class Parenthesis_AXES_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return	proc.optional<Parenthesis_AXES_2>() ||
				proc.optional<Parenthesis_AXES_3>();
	};
};



BaseObjectPtr AXES::clone()
{
	return BaseObjectPtr(new AXES(*this));
}


// AXES = [IVAXIS DVAXIS [SERIESAXIS] / DVAXIS DVAXIS] *3ATTACHEDLABEL [PlotArea FRAME]
const bool AXES::loadContent(BinProcessor& proc)
{
	if (proc.optional<Parenthesis_AXES_1>())
	{
		int count = elements_.size();

		while(count > 0)
		{
			m_arAxes.insert(m_arAxes.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}
	}

	int count = proc.repeated<ATTACHEDLABEL>(0, 3);
	while(count > 0)
	{
		m_arATTACHEDLABEL.insert(m_arATTACHEDLABEL.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	if(proc.optional<PlotArea>())
	{
		m_PlotArea = elements_.back();
		elements_.pop_back();
	
		proc.mandatory<FRAME>();
		
		m_FRAME = elements_.back();
		elements_.pop_back();
	}

	return true;
}


} // namespace XLS

