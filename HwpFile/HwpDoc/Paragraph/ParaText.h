#ifndef PARATEXT_H
#define PARATEXT_H

#include "Ctrl.h"

namespace HWP
{
class CParaText : public CCtrl
{
	STRING m_sText;
	int m_nStartIDx;
	// int m_nCharShapeID;
public:
	CParaText(const STRING& sCtrlID, const STRING& sText, int nStartIDx);

	int GetSize() override;
};
}

#endif // PARATEXT_H
