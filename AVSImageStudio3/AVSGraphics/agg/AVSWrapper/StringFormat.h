#pragma once

#include "..\ap_AggPlusTypes.h"
#include "..\ap_AggPlusEnums.h"

namespace Aggplus
{

class CStringFormat  
{
public:
	CStringFormat(LONG formatFlags = 0, LANGID language = LANG_NEUTRAL)
	{
		m_alignment = StringAlignmentNear;
	}
	virtual ~CStringFormat() 
	{
	}

	Status SetAlignment(StringAlignment align)
	{
		m_alignment = align; 
		return Ok; 
	}
	
	StringAlignment GetAlignment() const
	{
		return m_alignment; 
	}

protected:
	StringAlignment m_alignment;
};

}