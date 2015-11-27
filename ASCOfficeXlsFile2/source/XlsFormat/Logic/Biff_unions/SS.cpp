
#include "SS.h"
#include <Logic/Biff_records/DataFormat.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Chart3DBarShape.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_records/AreaFormat.h>
#include <Logic/Biff_records/PieFormat.h>
#include <Logic/Biff_records/SerFmt.h>
#include <Logic/Biff_records/MarkerFormat.h>
#include <Logic/Biff_records/AttachedLabel.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_unions/GELFRAME.h>
#include <Logic/Biff_unions/SHAPEPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>

namespace XLS
{


SS::SS()
{
}


SS::~SS()
{
}


BaseObjectPtr SS::clone()
{
	return BaseObjectPtr(new SS(*this));
}


/*
SS = DataFormat Begin [Chart3DBarShape] [LineFormat AreaFormat PieFormat] [SerFmt] [GELFRAME] 
		[MarkerFormat] [AttachedLabel] *2SHAPEPROPS [CRTMLFRT] End
*/
const bool SS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DataFormat>())
	{
		return false;
	}
	m_DataFormat = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();						elements_.pop_back();
	if (proc.optional<Chart3DBarShape>())
	{
		m_Chart3DBarShape = elements_.back();
		elements_.pop_back();
	}
	if(proc.optional<LineFormat>())
	{
		m_LineFormat = elements_.back();
		elements_.pop_back();
	
		if (proc.optional<AreaFormat>())
		{
			m_AreaFormat = elements_.back();
			elements_.pop_back();
		}
		
		if (proc.optional<PieFormat>())
		{
			m_PieFormat = elements_.back();
			elements_.pop_back();
		}
	}
	if (proc.optional<SerFmt>())
	{
		m_SerFmt = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<GELFRAME>())
	{
		m_GELFRAME = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<MarkerFormat>())
	{
		m_MarkerFormat = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AttachedLabel>())
	{
		m_AttachedLabel = elements_.back();
		elements_.pop_back();
	}
	
	int count = proc.repeated<SHAPEPROPS>(0, 2);
	while(count > 0)
	{
		m_arSHAPEPROPS.insert(m_arSHAPEPROPS.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();								elements_.pop_back();

	return true;
}

} // namespace XLS

