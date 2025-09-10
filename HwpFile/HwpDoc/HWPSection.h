#ifndef HWPSECTION_H
#define HWPSECTION_H

#include "Paragraph/HWPPargraph.h"
#include "Paragraph/CtrlContainer.h"

namespace HWP
{
class CHWPSection
{
	VECTOR<CHWPPargraph*> m_arParas;

	int ParseListAppend(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	int ParseListAppend(CCtrl& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
public:
	CHWPSection();
	~CHWPSection();
	
	bool Parse(CXMLReader& oReader, int nVersion, EHanType eType);
	bool Parse(CHWPStream& oBuffer, int nVersion);
	bool ParseHWPML(CXMLReader &oReader, int nVersion);
	int ParseRecurse(CHWPPargraph* oCurrPara, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion);
	int ParseCtrlRecurse(CCtrl* oCurrCtrl, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion);
	int ParseContainerRecurse(CCtrlContainer* oContainer, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion);

	VECTOR<const CHWPPargraph*> GetParagraphs() const;
};
}

#endif // HWPSECTION_H
