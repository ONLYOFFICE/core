#ifndef CHWPMLFILE_H
#define CHWPMLFILE_H

#include "Common/XMLReader.h"
#include "HWPSection.h"

namespace HWP
{
class CHWPMLFile
{
	HWP_STRING m_wsFilePath;
	VECTOR<CHWPSection*> m_arSections;

	void ReadHead(CXMLReader& oReader);
	void ReadBody(CXMLReader& oReader);
	void ReadTail(CXMLReader& oReader);
	
	void ReadSection(CXMLReader& oReader, int nVersion);
public:
	CHWPMLFile(const HWP_STRING& wsFilePath);
	~CHWPMLFile();

	bool Open();
	bool Detect() const;
	void Close();
	
	VECTOR<const CHWPSection*> GetSections() const;
};
}

#endif // CHWPMLFILE_H
