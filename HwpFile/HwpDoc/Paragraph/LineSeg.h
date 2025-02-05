#ifndef LINESEG_H
#define LINESEG_H

#include "../HWPStream.h"
#include "../Common/XMLNode.h"

namespace HWP
{
class CLineSeg
{
	int m_nStartPos;
	int m_nLineVerticalPos;
	int m_nLineHeight;
	int m_nTextHeight;
	int m_nLineDistanceToBase;
	int m_nLineSpacing;
	int m_nColumnStartPos;
	int m_nSegmentWidth;
	int m_nLineTag;
	bool m_bIsHeadingApplied;
public:
	CLineSeg(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CLineSeg(CXMLNode& oNode, int nVersion);

	int GetLineSpacing() const;
};
}

#endif // LINESEG_H
