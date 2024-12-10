#ifndef CTRLSECTIONDEF_H
#define CTRLSECTIONDEF_H

#include "Ctrl.h"
#include "../Section/Page.h"

#include "CtrlHeadFoot.h"
#include "../Section/NoteShape.h"
#include "../Section/PageBorderFill.h"

namespace HWP
{
class CCtrlSectionDef : public CCtrl
{
	int m_nSize;

	bool m_bHideHeader;
	bool m_bHideFooter;
	bool m_bHideMasterPage;
	bool m_bHideBorder;
	bool m_bHideFill;
	bool m_bHidePageNumPos;
	bool m_bShowFirstBorder;
	bool m_bShowFirstFill;
	BYTE m_chTextDirection;
	bool m_bHideEmptyLine;
	BYTE m_chPageStartOn;

	short m_shSpaceColumns;
	short m_shLineGrid;
	short m_shCharGrid;
	int m_nTabStop;
	int m_nOutlineNumberingID;
	short m_shPageNum;
	short m_shFigure;
	short m_shTable;
	short m_shEquation;
	short m_shLang;

	// CPage m_oPage;
	LIST<CCtrlHeadFoot*> m_arHeaderFooter;
	// LIST<CNoteShape*> m_arNoteShapes;
	// LIST<CPageBorderFill*> m_arBorderFills;

	// LIST<CHWPPargraph*> m_arParas;

public:
	CCtrlSectionDef(const STRING& sCtrlID);
	CCtrlSectionDef(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	void AddHeadFoot(CCtrlHeadFoot* pHeadFoot);

	int GetSize() override;
};
}

#endif // CTRLSECTIONDEF_H
