#ifndef CTRLNOTE_H
#define CTRLNOTE_H

#include "HWPPargraph.h"
#include "Ctrl.h"
#include <list>

namespace HWP
{
class CCtrlNote : public CCtrl
{
	VECTOR<CHWPPargraph*> m_arParas;
public:
	CCtrlNote();
	CCtrlNote(const HWP_STRING& sCtrlID);
	CCtrlNote(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	ECtrlObjectType GetCtrlType() const override;

	VECTOR<const CHWPPargraph*> GetParagraphs() const;

	void AddParagraph(CHWPPargraph* pParagraph);
};
}

#endif // CTRLNOTE_H
