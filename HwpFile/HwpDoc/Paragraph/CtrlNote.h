#ifndef CTRLNOTE_H
#define CTRLNOTE_H

#include "HWPPargraph.h"
#include "Ctrl.h"
#include <list>

namespace HWP
{
class CCtrlNote : public CCtrl
{
	int m_nSize;

	VECTOR<CHWPPargraph*> m_arParas;
public:
	CCtrlNote();
	CCtrlNote(const STRING& sCtrlID);
	CCtrlNote(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;

	void AddParagraph(CHWPPargraph* pParagraph);
};
}

#endif // CTRLNOTE_H
