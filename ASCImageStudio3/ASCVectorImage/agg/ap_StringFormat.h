// ap_StringFormat.h: interface for the StringFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_STRINGFORMAT_H__7B4A52EC_28CA_4FBC_B3DE_00D3B4A7F46C__INCLUDED_)
#define Aggplus_STRINGFORMAT_H__7B4A52EC_28CA_4FBC_B3DE_00D3B4A7F46C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ap_AggPlusTypes.h"
#include "ap_AggPlusEnums.h"

namespace Aggplus
{
	class StringFormat  
	{
	public:
		StringFormat(INT formatFlags = 0, LANGID language = LANG_NEUTRAL)
		{
			m_alignment=StringAlignmentNear;
		}
		virtual ~StringFormat() {};

		Status SetAlignment(StringAlignment align) { m_alignment=align; return Ok; }
		StringAlignment GetAlignment() const { return m_alignment; }

	protected:
		StringAlignment m_alignment;
	};

} //namespace Aggplus


#endif // !defined(Aggplus_STRINGFORMAT_H__7B4A52EC_28CA_4FBC_B3DE_00D3B4A7F46C__INCLUDED_)
