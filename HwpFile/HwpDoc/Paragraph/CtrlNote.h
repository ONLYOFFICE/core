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

	std::list<CHWPPargraph*> m_arParas;
public:
	CCtrlNote(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLNOTE_H
