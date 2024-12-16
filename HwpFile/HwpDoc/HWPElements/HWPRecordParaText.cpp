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
	STRING sText;
	oBuffer.ReadString(sText, nSize);

	if (sText.empty())
		return LIST<CCtrl*>();

	std::regex oRegex("[\\u0000\\u000a\\u000d\\u0018-\\u001f]|[\\u0001\\u0002-\\u0009\\u000b-\\u000c\\u000e-\\u0017].{6}[\\u0001\\u0002-\\u0009\\u000b-\\u000c\\u000e-\\u0017]");
	std::sregex_iterator itCurrent(sText.begin(), sText.end(), oRegex);
	std::sregex_iterator itEnd;

	int nPrevIndex = 0;

	LIST<CCtrl*> arParas;

	while(itCurrent != itEnd)
	{
		if (itCurrent->position() > nPrevIndex)
		{
			// write text
			arParas.push_back(new CParaText("____", sText.substr(nPrevIndex, itCurrent->position() - nPrevIndex), nPrevIndex));
		}

		if (1 == itCurrent->length())
		{
			switch(itCurrent->str()[0])
			{
				case 0x0a:
				{
					arParas.push_back(new CCtrlCharacter("   _", ECtrlCharType::LINE_BREAK));
					break;
				}
				case 0x0d:
				{
					arParas.push_back(new CCtrlCharacter("   _", ECtrlCharType::PARAGRAPH_BREAK));
					break;
				}
				case 0x18:
				{
					arParas.push_back(new CCtrlCharacter("   _", ECtrlCharType::HARD_HYPHEN));
					break;
				}
				case 0x1e:
				case 0x1f:
				{
					arParas.push_back(new CCtrlCharacter("   _", ECtrlCharType::HARD_SPACE));
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
					arParas.push_back(new CParaText("____", "\t", 0));
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
					if ("pngp" == sInfo) arParas.push_back(new CCtrlPageNumPos(sInfo));
					else if ("onwn" == sInfo) arParas.push_back(new CCtrlNewNumber(sInfo));
					break;
				}
				case 0x02:
				{
					if ("dces" == sInfo) arParas.push_back(new CCtrlSectionDef(sInfo));
					else if ("dloc" == sInfo) arParas.push_back(new CCtrlColumnDef(sInfo));
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
					if (" osg" == sInfo)
						arParas.push_back(new CCtrlGeneralShape(sInfo));
					else if (" lbt" == sInfo)
						arParas.push_back(new CCtrlTable(sInfo));
					else if ("deqe" == sInfo)
						arParas.push_back(new CCtrlEqEdit(sInfo));
					// else if ("mrof" == sInfo)
					break;
				}
				default:
					break;
			}

			nPrevIndex = itCurrent->position() + itCurrent->length();
		}

		if (nPrevIndex < sText.length())
		{
			// write final text
			arParas.push_back(new CParaText("____", sText.substr(nPrevIndex), nPrevIndex));
		}
	}

	return arParas;
}
}
