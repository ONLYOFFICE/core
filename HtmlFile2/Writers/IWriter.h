#ifndef IWRITER_H
#define IWRITER_H

#include "../../Common/3dParty/html/css/src/CNode.h"
#include "../Common.h"

namespace HTML
{
class IWriter
{
public:
	IWriter() = default;
	virtual ~IWriter() = default;

	virtual void Begin(const std::wstring& wsDst) = 0;
	virtual void End(const std::wstring& wsDst) = 0;

	virtual bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) = 0;

	virtual void PageBreak() = 0;

	virtual void BeginBlock() = 0;
	virtual void EndBlock(bool bAddBlock) = 0;

	virtual void SetDataOutput(XmlString* pOutputData) = 0; // Задаем место вывода для интерпретатора
	virtual void RevertDataOutput() = 0; // Возвращаем место вывода к исходному

	virtual XmlString* GetCurrentDocument() const = 0;

	//TODO:: перенести разруливание вложенных таблиц в конвертацию после изменения принципа работы с таблицами
	virtual bool SupportNestedTables() const = 0;
};
}

#endif // IWRITER_H
