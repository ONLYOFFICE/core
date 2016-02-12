
#include "DVAXIS.h"
#include <Logic/Biff_records/Axis.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/ValueRange.h>
#include <Logic/Biff_unions/AXM.h>
#include <Logic/Biff_unions/AXS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_records/CatSerRange.h>
#include <Logic/Biff_records/Tick.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_records/AxisLine.h>
#include <Logic/Biff_records/LineFormat.h>

namespace XLS
{


DVAXIS::DVAXIS()
:	id(0)
{
	m_bSecondary = false;
}


DVAXIS::~DVAXIS()
{
}


BaseObjectPtr DVAXIS::clone()
{
	return BaseObjectPtr(new DVAXIS(*this));
}


const bool DVAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->GenerateAXESId();
	}
	return ok;
}

// DVAXIS = Axis Begin [ValueRange] [AXM] AXS [CRTMLFRT] End
const bool DVAXIS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	m_Axis = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();				elements_.pop_back();
	
	if (proc.optional<ValueRange>())
	{
		m_ValueRange = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AXM>())
	{
		m_AXM = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<AXS>())
	{
		m_AXS = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<CRTMLFRT>())
	{
		m_CRTMLFRT = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<End>())					elements_.pop_back();

	return true;
}

int DVAXIS::serialize(std::wostream & _stream)
{
	ValueRange	*value_range	= dynamic_cast<ValueRange*>	(m_ValueRange.get());
	Axis		*axis			= dynamic_cast<Axis*>		(m_Axis.get());
	CRTMLFRT	*crtMltFrt		= dynamic_cast<CRTMLFRT*>	(m_CRTMLFRT.get());
	
	int axes_type = axis->wType + 1;

	bool bLogarithScale = false;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:axId")
		{
			CP_XML_ATTR(L"val", id);
		}
		CP_XML_NODE(L"c:scaling")
		{
			if ((value_range) && (value_range->fLog == true))
			{
				CP_XML_NODE(L"c:logBase") {  CP_XML_ATTR(L"val", 10); }
				bLogarithScale = true;
			}

			CP_XML_NODE(L"c:orientation")
			{
				if ((value_range) && (value_range->fReversed)) CP_XML_ATTR(L"val", L"maxMin");
				else CP_XML_ATTR(L"val", L"minMax"); 
			}
			if ((value_range) && (value_range->fAutoMax == false))
			{
				CP_XML_NODE(L"c:max") 
				{ 
					if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMax));
					else				CP_XML_ATTR(L"val", value_range->numMax);
				}
			}			
			if ((value_range) && (value_range->fAutoMin == false))
			{
				CP_XML_NODE(L"c:min") 
				{
					if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMin));
					else				CP_XML_ATTR(L"val", value_range->numMin); 
				}
			}
		}
		CP_XML_NODE(L"c:delete"){  CP_XML_ATTR(L"val", 0); }
//-------------------------------------------------------------------------------
		CP_XML_NODE(L"c:axPos")
		{
			if (axes_type == 1)
			{
				if (m_bSecondary)	CP_XML_ATTR(L"val", L"t");
				else				CP_XML_ATTR(L"val", L"b");
			}
			else if (axes_type == 2)
			{
				if (m_bSecondary)	CP_XML_ATTR(L"val", L"r");
				else				CP_XML_ATTR(L"val", L"l");
			}
			else
			{
				if (m_bSecondary)	CP_XML_ATTR(L"val", L"t");
				else				CP_XML_ATTR(L"val", L"b");
			}
		}
//----------------------------------------------------------------------------------------------
		m_AXS->serialize(_stream);

		CP_XML_NODE(L"c:crosses")
		{
			if ((value_range) && (value_range->fMaxCross == true))	CP_XML_ATTR(L"val", L"max");
			else CP_XML_ATTR(L"val", L"autoZero");
		}

		if ((value_range) && (value_range->fAutoMajor == false))
		{
			CP_XML_NODE(L"c:majorUnit")
			{
				if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMajor));
				else				CP_XML_ATTR(L"val", value_range->numMajor);
			}
		}
		if ((value_range) && (value_range->fAutoMinor == false))
		{
			CP_XML_NODE(L"c:minorUnit")
			{
				if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMinor));
				else				CP_XML_ATTR(L"val", value_range->numMinor);
			}
		}
	}
	
	return axes_type;
}

} // namespace XLS

