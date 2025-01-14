#ifndef HWPRECORDPARATEXT_H
#define HWPRECORDPARATEXT_H

#include "HWPRecord.h"
#include "../HWPStream.h"
#include "../Paragraph/Ctrl.h"

namespace HWP
{
class CHWPRecordParaText : public CHWPRecord
{
public:
	CHWPRecordParaText(int nTagNum, int nLevel, int nSize);

	static LIST<CCtrl*> Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDPARATEXT_H
