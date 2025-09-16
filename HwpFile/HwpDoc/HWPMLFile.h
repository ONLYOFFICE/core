#ifndef CHWPMLFILE_H
#define CHWPMLFILE_H

#include "Common/XMLReader.h"
#include "HWPSection.h"

namespace HWP
{
class CHWPMLFile
{
	HWP_STRING           m_wsFilePath;
	CHWPDocInfo          m_oDocInfo;
	VECTOR<CHWPSection*> m_arSections;
	std::map<unsigned int, BYTE*> m_mBinDates;
	
	void ReadHead(CXMLReader& oReader);
	void ReadBody(CXMLReader& oReader);
	void ReadTail(CXMLReader& oReader);
	
	void ReadSection(CXMLReader& oReader, int nVersion);
	void ReadBinData(CXMLReader& oReader);
public:
	CHWPMLFile(const HWP_STRING& wsFilePath);
	~CHWPMLFile();

	bool Open();
	bool Detect() const;
	void Close();

	const CHWPDocInfo* GetDocInfo() const;
	VECTOR<const CHWPSection*> GetSections() const;
	const BYTE* GetBinData(unsigned int unID) const;
};
}

#endif // CHWPMLFILE_H
