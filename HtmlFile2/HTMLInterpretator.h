#ifndef HTMLINTERPRETATOR_H
#define HTMLINTERPRETATOR_H

namespace HTML
{
class IHTMLInterpretator
{
public:
	IHTMLInterpretator() = default;
	virtual ~IHTMLInterpretator() = default;

	virtual void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) = 0;

	#ifdef _DEBUG
	virtual void PrintData() = 0;
	#endif

	// virtual bool ReadText(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadA(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadAbbr(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadBold(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadBdo(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadBdi(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadBr(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadCenter(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadItalic(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadCode(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadKbd(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadSamp(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadStrike(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadFont(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool readImage(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadUnderline(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadMark(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadQ(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadSup(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadSub(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool readSVG(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool readInput(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadSpan(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadNobr(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadBasefont(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadDD(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadDiv(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadBlockquote(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadHr(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadListElement(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadList(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool readMenu(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadPre(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadTable(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadRuby(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadTextarea(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
	// virtual bool ReadDetails(NSStringUtils::CStringBuilder& oXml, std::vector<NSCSS::CNode>& sSelectors, TTextSettings& oTS) = 0;
};
}

#endif // HTMLINTERPRETATOR_H
