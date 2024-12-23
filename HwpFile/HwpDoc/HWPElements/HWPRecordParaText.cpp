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

#include <regex>

namespace HWP
{
CHWPRecordParaText::CHWPRecordParaText(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

LIST<CCtrl*> CHWPRecordParaText::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	STRING sText;
	oBuffer.ReadString(sText, nSize, EStringCharacter::UTF16);

	if (sText.empty())
	{
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
		return LIST<CCtrl*>();
	}

	std::wregex oRegex(L"[\\u0000\\u000a\\u000d\\u0018-\\u001f]|[\\u0001\\u0002-\\u0009\\u000b-\\u000c\\u000e-\\u0017].{6}[\\u0001\\u0002-\\u0009\\u000b-\\u000c\\u000e-\\u0017]");
	std::wsregex_iterator itCurrent(sText.begin(), sText.end(), oRegex);
	std::wsregex_iterator itEnd = std::wsregex_iterator();

	int nPrevIndex = 0;

	LIST<CCtrl*> arParas;

	while(itCurrent != itEnd)
	{
		if (itCurrent->position() > nPrevIndex)
		{
			// write text
			arParas.push_back(new CParaText(L"____", sText.substr(nPrevIndex, itCurrent->position() - nPrevIndex), nPrevIndex));
		}

		if (1 == itCurrent->length())
		{
			switch(itCurrent->str()[0])
			{
				case 0x0a:
				{
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
					break;
				}
				case 0x0d:
				{
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));
					break;
				}
				case 0x18:
				{
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
					break;
				}
				case 0x1e:
				case 0x1f:
				{
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
					break;
				}
			}
		}
		else if (8 == itCurrent->length())
		{
			//TODO:: реализовать
			STRING sInfo = sText.substr(2, 4); //  new String(m.group().getBytes(StandardCharsets.UTF_16LE), 2, 12, StandardCharsets.US_ASCII).replaceAll("[\\x00-\\x20]+$", "");
			switch(itCurrent->str()[0])
			{
				case 0x04:
				case 0x08:
					break;
				case 0x09:
				{
					arParas.push_back(new CParaText(L"____", L"\t", 0));
					break;
				}
				case 0x10:
				{
					arParas.push_back(new CCtrlHeadFoot(sInfo));
					break;
				}
				case 0x12:
				{
					arParas.push_back(new CCtrlAutoNumber(sInfo));
					break;
				}
				case 0x15:
				{
					// if ("dhgp" == sInfo) break;
					if (L"pngp" == sInfo) arParas.push_back(new CCtrlPageNumPos(sInfo));
					else if (L"onwn" == sInfo) arParas.push_back(new CCtrlNewNumber(sInfo));
					break;
				}
				case 0x02:
				{
					if (L"dces" == sInfo) arParas.push_back(new CCtrlSectionDef(sInfo));
					else if (L"dloc" == sInfo) arParas.push_back(new CCtrlColumnDef(sInfo));
					break;
				}
				case 0x03:
				case 0x0e:
				case 0x0f:
					break;
				case 0x11:
				{
					arParas.push_back(new CCtrlNote(sInfo));
					break;
				}
				case 0x16:
				case 0x17:
					break;
				case 0x0b:
				{
					if (L" osg" == sInfo)
						arParas.push_back(new CCtrlGeneralShape(sInfo));
					else if (L" lbt" == sInfo)
						arParas.push_back(new CCtrlTable(sInfo));
					else if (L"deqe" == sInfo)
						arParas.push_back(new CCtrlEqEdit(sInfo));
					// else if ("mrof" == sInfo)
					break;
				}
				default:
					break;
			}
		}
		nPrevIndex = itCurrent->position() + itCurrent->length();
		++itCurrent;
	}

	if (nPrevIndex < sText.length())
	{
		// write final text
		arParas.push_back(new CParaText(L"____", sText.substr(nPrevIndex), nPrevIndex));
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return arParas;
}
}
