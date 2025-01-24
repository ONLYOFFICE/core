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
#include "../Paragraph/CCtrlField.h"

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

	if (sText.empty())
	{
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
		return LIST<CCtrl*>();
	}

	//TODO:: перейти на обычный проход по символам
	std::wregex oRegex(L"([\\u0000-\\u001f]|.{2}[\\u0000-\u0017]{4})"); // [\\u0000\\u000a\\u000d\\u0018-\\u001f]|[\\u0001\\u0002-\\u0009\\u000b-\\u000c\\u000e-\\u0017].{6}[\\u0001\\u0002-\\u0009\\u000b-\\u000c\\u000e-\\u0017]
	std::wsregex_iterator itCurrent(sText.begin(), sText.end(), oRegex);
	std::wsregex_iterator itEnd = std::wsregex_iterator();

	int nPrevIndex = 0;

	LIST<CCtrl*> arParas;

	HWP_STRING sCurrentText;
	bool bEnd = false;

	#define UPDATE_CURRENT_TEXT() \
	do { \
	if (!sCurrentText.empty()) \
	{ \
		arParas.push_back(new CParaText(L"____", sCurrentText, nPrevIndex + 1)); \
		sCurrentText.clear(); \
	}} while (false)

	while(itCurrent != itEnd && !bEnd)
	{
		if (itCurrent->position() > nPrevIndex)
		{
			// write text
			sCurrentText += sText.substr(nPrevIndex, itCurrent->position() - nPrevIndex);
		}

		if (1 == itCurrent->length())
		{
			switch(itCurrent->str()[0])
			{
				case 0x00:
				case 0x02:
				// case 0x17:
				// case 0x19:
				{
					sCurrentText.clear();
					break;
				}
				case 0x03:
				case 0x04:
				{
					UPDATE_CURRENT_TEXT();
					break;
				}
				case 0x08:
				{
					if (!sCurrentText.empty())
						sCurrentText.pop_back();
					break;
				}
				case 0x09:
				{
					sCurrentText.push_back(L'\t');
					break;
				}
				case 0x0b:
				{
					sCurrentText.push_back(L'\v');
					break;
				}
				case 0x0a:
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
					break;
				}
				case 0x0d:
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));
					break;
				}
				case 0x18:
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
					break;
				}
				case 0x1e:
				case 0x1f:
				{
					UPDATE_CURRENT_TEXT();
					arParas.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
					break;
				}
			}
		}
		// Пока данный вариант невозможен
		else if (6 == itCurrent->length())
		{
			UPDATE_CURRENT_TEXT();

			//TODO:: Проверить
			HWP_STRING sInfo = sText.substr(itCurrent->position(), 2);
			std::wregex wrReplaceRegex(L"[\\x00-\\x20]+$");
			sInfo = std::regex_replace(sInfo, wrReplaceRegex, L"");

			HWP_STRING sType;
			sType.resize(4);

			sType[0] = (sInfo[0] & 0xFF);
			sType[1] = ((sInfo[0] >> 8) & 0xFF);
			sType[2] = (sInfo[1] & 0xFF);
			sType[3] = ((sInfo[1] >> 8) & 0xFF);

			if (0x00 == sType[3])
				sType[3] = L' ';

			//TODO:: более подробно разобраться в данном моменте
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
			else if (L"klh%" == sType ||
			         L"klh " == sType)
				arParas.push_back(new CCtrlField(sType));
		}

		nPrevIndex = itCurrent->position() + itCurrent->length();
		++itCurrent;
	}

	if (!bEnd)
		sCurrentText += sText.substr(nPrevIndex);

	if (!sCurrentText.empty())
		arParas.push_back(new CParaText(L"____", sCurrentText, nPrevIndex));

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return arParas;
}
}
