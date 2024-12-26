#ifndef PARATEXT_H
#define PARATEXT_H

#include "Ctrl.h"

namespace HWP
{
class CParaText : public CCtrl
{
	HWP_STRING m_sText;
	int m_nStartIDx;
	int m_nCharShapeID;
public:
	CParaText(const HWP_STRING& sCtrlID, const HWP_STRING& sText, int nStartIDx);
	CParaText(const HWP_STRING& sCtrlID, const HWP_STRING& sText, int nStartIDx, int nCharShapeID);

	int GetSize() override;
	int GetStartIDx() const;
	int GetCharShapeID() const;
	HWP_STRING GetText() const;

	void SetCharShapeID(int nCharShapeID);
	void SetText(const HWP_STRING& sText);
};
}

#endif // PARATEXT_H
