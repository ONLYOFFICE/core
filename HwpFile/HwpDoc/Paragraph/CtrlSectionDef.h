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
	bool m_bHideHeader;
	bool m_bHideFooter;
	bool m_bHideMasterPage;
	bool m_bHideBorder;
	bool m_bHideFill;
	bool m_bHidePageNumPos;
	bool m_bShowFirstBorder;
	bool m_bShowFirstFill;
	HWP_BYTE m_chTextDirection;
	bool m_bHideEmptyLine;
	HWP_BYTE m_chPageStartOn;

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

	CPage *m_pPage;
	VECTOR<CCtrlHeadFoot*> m_arHeaderFooter;
	VECTOR<CNoteShape*> m_arNoteShapes;
	VECTOR<CPageBorderFill*> m_arBorderFills;

	VECTOR<CHWPPargraph*> m_arParas;

public:
	CCtrlSectionDef(const HWP_STRING& sCtrlID);
	CCtrlSectionDef(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlSectionDef(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion);
	~CCtrlSectionDef();

	ECtrlObjectType GetCtrlType() const override;

	void SetPage(CPage* pPage);

	void AddHeadFoot(CCtrlHeadFoot* pHeadFoot);
	void AddParagraph(CHWPPargraph* pParagraph);
	void AddNoteShape(CNoteShape* pNoteShape);
	void AddPageBorderFill(CPageBorderFill* pPageBorderFill);

	const CPage* GetPage() const;
	VECTOR<const CCtrlHeadFoot*> GetHeaderFooters() const;
};
}

#endif // CTRLSECTIONDEF_H
