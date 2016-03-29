
#include "SERIESAXIS.h"
#include <Logic/Biff_records/Axis.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/CatSerRange.h>
#include <Logic/Biff_unions/AXS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


SERIESAXIS::SERIESAXIS()
{
	m_bSecondary = false;
}


SERIESAXIS::~SERIESAXIS()
{
}


BaseObjectPtr SERIESAXIS::clone()
{
	return BaseObjectPtr(new SERIESAXIS(*this));
}


const bool SERIESAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->GenerateAXESId();
	}
	return ok;
}

// SERIESAXIS = Axis Begin [CatSerRange] AXS [CRTMLFRT] End
const bool SERIESAXIS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	m_Axis = elements_.back();
	elements_.pop_back();

	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;
		if (type == rt_End) 
		{
			if (proc.mandatory<End>())
				elements_.pop_back();
			break;
		}

		switch(type)
		{
			case rt_Begin:
			{
				if (proc.optional<Begin>())
					elements_.pop_back();
			}break;
			case rt_CatSerRange:
			{
				if (proc.optional<CatSerRange>())
				{
					m_CatSerRange = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_IFmtRecord:
			case rt_Tick:
			case rt_FontX:
			case rt_AxisLine:
			{
				if (proc.mandatory<AXS>())
				{
					m_AXS = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_CrtMlFrt:		
			{
				proc.optional<CRTMLFRT>();		
			}break;
			default:
			{
				proc.SkipRecord();	
			}break;
		}
	}
	return true;
}
int SERIESAXIS::serialize(std::wostream & _stream)
{
	CatSerRange * cat_ser_range = dynamic_cast<CatSerRange*>(m_CatSerRange.get());
	Axis		* axis			= dynamic_cast<Axis*>		(m_Axis.get());

	int axes_type = axis->wType + 1;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:axId")
		{
			CP_XML_ATTR(L"val", id);
		}
		
		CP_XML_NODE(L"c:scaling")
		{
			if (cat_ser_range->fReversed)
			{
				CP_XML_NODE(L"c:orientation"){  CP_XML_ATTR(L"val", L"maxMin"); }
			}else
			{
				CP_XML_NODE(L"c:orientation"){  CP_XML_ATTR(L"val", L"minMax"); }
			}
		}
		CP_XML_NODE(L"c:auto")	{  CP_XML_ATTR(L"val", 0); }
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
//-----------------------------------------------------------------------------------
		m_AXS->serialize(_stream);

		CP_XML_NODE(L"c:crosses")
		{
			if ((cat_ser_range) && (cat_ser_range->fMaxCross == true))	CP_XML_ATTR(L"val", L"max");
			else CP_XML_ATTR(L"val", L"autoZero");
		}	
	}
	
	return 	axes_type;
}

} // namespace XLS

