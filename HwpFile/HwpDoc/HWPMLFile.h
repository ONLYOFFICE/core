#ifndef CHWPMLFILE_H
#define CHWPMLFILE_H

#include "Common/XMLReader.h"
#include "HWPSection.h"

namespace HWP
{
class CHWPMLFile
{
	using BinMap  = std::map <HWP_STRING, CHWPStream*>;

	HWP_STRING           m_wsFilePath;
	CHWPDocInfo          m_oDocInfo;
	VECTOR<CHWPSection*> m_arSections;
	BinMap m_mBinDates;

	void ReadHead(CXMLReader& oReader);
	void ReadBody(CXMLReader& oReader);
	void ReadTail(CXMLReader& oReader);
	
	void ReadSection(CXMLReader& oReader);
	void ReadBinData(CXMLReader& oReader);
public:
	CHWPMLFile(const HWP_STRING& wsFilePath);
	~CHWPMLFile();

	bool Open();
	bool Detect() const;
	void Close();

	const CHWPDocInfo* GetDocInfo() const;
	VECTOR<const CHWPSection*> GetSections() const;
	bool GetBinData(const HWP_STRING& sId, CHWPStream& oBuffer) const;
};
}

#endif // CHWPMLFILE_H
