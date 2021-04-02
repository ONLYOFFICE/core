#include "DocxConverter.h"

//#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/DocxFlat.h"

namespace Oox2Odf
{
	void DocxConverter::convert(OOX::Logic::COMathPara *oox_math_para)
	{
		if (!oox_math_para) return;

		for (size_t i = 0; i < oox_math_para->m_arrItems.size(); ++i)
		{
			convert(oox_math_para->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::COMathParaPr *oox_math_para_pr)
	{
		if (!oox_math_para_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::COMath *oox_math)
	{
		if (!oox_math) return;

		for (size_t i = 0; i < oox_math->m_arrItems.size(); ++i)
		{
			convert(oox_math->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::CSSup *oox_ssup)
	{
		if (!oox_ssup) return;
	}

	void DocxConverter::convert(OOX::Logic::CSSub *oox_ssub)
	{
		if (!oox_ssub) return;
	}

	void DocxConverter::convert(OOX::Logic::CMRun *oox_mrun)
	{
		if (!oox_mrun) return;
	}

	void DocxConverter::convert(OOX::Logic::CNary *oox_nary)
	{
		if (!oox_nary) return;
	}
}