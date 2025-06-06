#ifndef CONVERSIONSTATE_H
#define CONVERSIONSTATE_H

#include "Types.h"

#include "../Paragraph/CtrlHeadFoot.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlPageNumPos.h"
#include "../Paragraph/CtrlNewNumber.h"
#include "../Paragraph/CtrlColumnDef.h"
#include "../Paragraph/CtrlField.h"

namespace HWP
{
struct TConversionState
{
	bool m_bOpenedP;
	bool m_bOpenedR;
	bool m_bIsNote;
	bool m_bInTable;
	bool m_bLastEmptyNode;

	bool m_bInTextBox; // TODO:: используется, чтобы в wps:txbx не появилась новая фигура (посмотреть этот момент нужно подробнее)

	unsigned short m_ushLastCharShapeId;
	unsigned short m_ushSecdIndex;
	unsigned int m_unParaIndex;

	VECTOR<const CCtrlHeadFoot*> m_arCtrlsHeadFoot; //only for hwpx
	std::stack<int> m_arOpenedBookmarks;

	const CCtrlSectionDef* m_pSectionDef;
	const CCtrlColumnDef*  m_pColumnDef;
	const CCtrlPageNumPos* m_pPageNum;
	const CCtrlNewNumber*  m_pNewNumber;

	VECTOR<TRelationship>* m_pRelationships;

	enum class EBreakType
	{
		Page,
		Column,
		TextWrapping,
		None
	} m_eBreakType;

	std::map<unsigned int, const CCtrlField*> m_mOpenField;

	TConversionState()
	    : m_bOpenedP(false), m_bOpenedR(false), m_bIsNote(false), m_bInTable(false), m_bLastEmptyNode(false), m_bInTextBox(false), m_ushLastCharShapeId(-1), m_ushSecdIndex(0), m_unParaIndex(0),
	      m_pSectionDef(nullptr), m_pColumnDef(nullptr), m_pPageNum(nullptr), m_pNewNumber(nullptr), m_pRelationships(nullptr), m_eBreakType(EBreakType::None)
	{}
};
}

#endif // CONVERSIONSTATE_H
