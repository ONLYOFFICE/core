#include "HWPRecordParaText.h"

#include "../Paragraph/ParaText.h"
#include "../Paragraph/CtrlCharacter.h"
#include "../Paragraph/CtrlHeadFoot.h"
#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/CtrlPageNumPos.h"
#include "../Paragraph/CtrlNewNumber.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlColumnDef.h"
#include "../Paragraph/CtrlNote.h"
#include "../Paragraph/CtrlGeneralShape.h"
#include "../Paragraph/CtrlTable.h"
#include "../Paragraph/CtrlEqEdit.h"
#include "../Paragraph/CtrlField.h"

#include <regex>

#include "../../../DesktopEditor/common/File.h"

namespace HWP
{
CHWPRecordParaText::CHWPRecordParaText(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

LIST<CCtrl*> CHWPRecordParaText::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	HWP_STRING sText;
	oBuffer.ReadString(sText, nSize, EStringCharacter::UTF16);
	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	if (sText.empty())
		return LIST<CCtrl*>();

	HWP_STRING::const_iterator itFound = std::find_if(sText.cbegin(), sText.cend(), [](wchar_t wChar){ return wChar < 0x20; });

	if (sText.cend() == itFound)
		return {new CParaText(L"____", sText, 0)};

	LIST<CCtrl*> arParas;

	HWP_STRING sCurrentText;
	int nStartText = -1;

	#define UPDATE_CURRENT_TEXT()\
	do{\
	if (!sCurrentText.empty())\
	{\
		arParas.push_back(new CParaText(L"____", sCurrentText, nStartText + 1));\
		sCurrentText.clear();\
	}}\
	while(false)

	for (unsigned int unIndex = 0; unIndex < sText.length(); ++unIndex)
	{
		if (0x20 > (int)sText[unIndex])
		{
			switch ((int)sText[unIndex])
			{
				case 0x00: // Unusable
					continue;
				case 0x01: // Scheduling
				case 0x02: // Define Zones/Define Units
				case 0x03: // Field start
				case 0x04: // End of field
				case 0x0b: // Drawing objects/tables
				case 0x0c: // Scheduling
				case 0x0e: // Scheduling
				case 0x0f: // Behind the scenes
				case 0x10: // Header/Footer
				case 0x11: // Footnotes/endnotes
				case 0x12: // Auto-numbering (footnotes, tables, etc.)
				case 0x15: // Page controls (hide, start with new number, etc.)
				case 0x16: // Bookmark/Browse Marks
				case 0x17: // Comment/Text Overlap
				{
					HWP_STRING sType;

					unsigned int unInfoLength = 0;

					if (sText.length() > unIndex + 6 && sText[unIndex] == sText[unIndex + 6] && sText[unIndex + 1] >= 0x20)
						unInfoLength = 4;
					else if (sText.length() > unIndex + 7 && sText[unIndex] == sText[unIndex + 7] && sText[unIndex + 1] >= 0x20)
						unInfoLength = 5;
					else if (6 >= unIndex && unIndex > 2) //Случай, когда запись в самом начале
					{
						nStartText = -1;
						sCurrentText.clear();

						sType.resize(4);
						sType[0] = (sText[0] & 0xFF);
						sType[1] = ((sText[0] >> 8) & 0xFF);
						sType[2] = (sText[1] & 0xFF);
						sType[3] = ((sText[1] >> 8) & 0xFF);

						unInfoLength = 4;
						unIndex = 0;
					}
					else
						continue;

					if (sType.empty())
					{
						UPDATE_CURRENT_TEXT();

						sType.resize(4);

						sType[0] = (sText[unIndex + 1] & 0xFF);
						sType[1] = ((sText[unIndex + 1] >> 8) & 0xFF);
						sType[2] = (sText[unIndex + 2] & 0xFF);
						sType[3] = ((sText[unIndex + 2] >> 8) & 0xFF);
					}

					if (0x17 >= sType[3])
						sType[3] = L' ';

					if (L"daeh" == sType ||
					    L"tood" == sType)
						arParas.push_back(new CCtrlHeadFoot(sType));
					else if (L"onta" == sType)
						arParas.push_back(new CCtrlAutoNumber(sType));
					else if (L"pngp" == sType)
						arParas.push_back(new CCtrlPageNumPos(sType));
					else if (L"onwn" == sType)
						arParas.push_back(new CCtrlNewNumber(sType));
					else if (L"dces" == sType)
						arParas.push_back(new CCtrlSectionDef(sType));
					else if (L"dloc" == sType)
						arParas.push_back(new CCtrlColumnDef(sType));
					else if (L"  nf" == sType ||
					         L"  ne" == sType)
						arParas.push_back(new CCtrlNote(sType));
					else if (L" osg" == sType)
						arParas.push_back(new CCtrlGeneralShape(sType));
					else if (L" lbt" == sType)
						arParas.push_back(new CCtrlTable(sType));
					else if (L"deqe" == sType)
						arParas.push_back(new CCtrlEqEdit(sType));
					else if (L"klh%" == sType ||  // hyperlink start
					         L"klh " == sType ||  // hyperlink end
					         L"kmb%" == sType ||  // bookmark start
					         L"kmb " == sType)    // bookmark end
						arParas.push_back(new CCtrlField(sType));

					unIndex += unInfoLength + 2;
					break;
				}
				case 0x05: // Scheduling
				case 0x06: // Scheduling
				case 0x07: // Scheduling
				case 0x08: // Title mark
				case 0x09: // Tabs
				case 0x13: // Scheduling
				case 0x14: // Scheduling
				{
					UPDATE_CURRENT_TEXT();

					if (sText.length() < unIndex + 6 || sText[unIndex] != sText[unIndex + 6])
						continue;
					unIndex += 6;
					break;
				}
				case 0x0a: // End of line (line break)
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
					break;
				}
				case 0x0d: // End of paragraph (para break)
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));
					break;
				}
				case 0x18:// hyphen
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
					break;
				}
				case 0x19: // Scheduling
				case 0x1a: // Scheduling
				case 0x1b: // Scheduling
				case 0x1c: // Scheduling
				case 0x1d: // Scheduling
				{
					UPDATE_CURRENT_TEXT();
					continue;
				}
				case 0x1e: // Tied blanks
				case 0x1f: // Fixed-width blanks
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
					break;
				}
			}
		}
		else
		{
			if (nStartText < 0)
				nStartText = unIndex;

			sCurrentText.push_back(sText[unIndex]);
		}
	}

	if (!sCurrentText.empty())
		arParas.push_back(new CParaText(L"____", sCurrentText, (0 == nStartText) ? 0 : (nStartText + 1)));

	return arParas;
}
}
