#pragma once
#ifndef XPS_FOLDER_INCLUDE_H_
#define XPS_FOLDER_INCLUDE_H_

#include "Page.h"
#include <map>

namespace XPS
{
	class Folder
	{
	private:
		CString m_path;

		XmlUtils::CXmlNode	m_node;
		XmlUtils::CXmlNode	m_subnode;
		XmlUtils::CXmlNodes	m_nodes;

		std::map<int, XPS::Page*> m_pages;
		boost::shared_ptr<std::set<CString> > m_fontMap;

		CRITICAL_SECTION m_oFontCS;

		bool m_bIsClosed;
		long m_lDrawingPages;

	public:
		Folder();
		~Folder();

	public:
		void ReadFromPath(const CString& Path);
		int GetPageCount()const;
		void GetPageSize(int Number, int& w, int& h);
		void DrawPage(int Number, AVSGraphics::IAVSRenderer* pRenderer, BOOL* pBreak);
		void Close();
	};
} // namespace XPS

#endif //XPS_FOLDER_INCLUDE_H_