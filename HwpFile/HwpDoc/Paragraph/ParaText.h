#ifndef PARATEXT_H
#define PARATEXT_H

#include "Ctrl.h"

namespace HWP
{
class CParaText : public CCtrl
{
	STRING m_sText;
	int m_nStartIDx;
	int m_nCharShapeID;
public:
	CParaText(const STRING& sCtrlID, const STRING& sText, int nStartIDx);
	CParaText(const STRING& sCtrlID, const STRING& sText, int nStartIDx, int nCharShapeID);

	int GetSize() override;
	int GetStartIDx() const;
	STRING GetText() const;

	void SetCharShapeID(int nCharShapeID);
	void SetText(const STRING& sText);
};
}

#endif // PARATEXT_H
