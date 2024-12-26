#ifndef COMMONOBJ_H
#define COMMONOBJ_H

#include "HWPPargraph.h"
#include <string>
#include <vector>
#include <list>

namespace HWP
{
struct TCommonObj
{
	HWP_STRING m_sCtrlID;

	int m_nObjAttr;
	int m_nYOffset;
	int m_nXOffset;
	int m_nObjWidth;
	int m_nObjHeight;
	int m_nZOrder;
	VECTOR<short> m_arObjSpaces;
	int m_nObjInstanceID;
	int m_nBlockPageBreak;
	HWP_STRING m_sObjDesc;

	LIST<CHWPPargraph*> m_arParas;
	int m_nCaptionAttr;
	int m_nCaptionWidth;
	int m_nCaptionSpacing;
	int m_nCaptionMaxW;
};
}

#endif // COMMONOBJ_H
