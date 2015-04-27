#pragma once
#include "../RtfDocument.h"
#include "../RtfField.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Docx.h"

class ConvertationManager;

class OOXReader
{
public: 

	ConvertationManager *	m_convertationManager;
	CString					m_sPath;

	int						m_nCurItap; //для определение вложенности таблицы

	int						m_nCurFittextId;
	std::map<int, CString>	m_aBookmarks;

	std::map<int, int>					m_mapPictureBullet;
	std::map<int, TextItemContainerPtr> m_mapFootnotes;
	std::map<int, TextItemContainerPtr> m_mapEndnotes;

	OOXIdGenerator			m_oOOXIdGenerator;

//Theme Fonts
	CString					m_smajorAscii;
	CString					m_smajorBidi;
	CString					m_smajorEastAsia;
	CString					m_smajorHAnsi;
	CString					m_sminorAscii;
	CString					m_sminorBidi;
	CString					m_sminorEastAsia;
	CString					m_sminorHAnsi;

//defaultStyles
	RtfCharStylePtr			m_oDefCharStyle;
	RtfParagraphStylePtr	m_oDefParStyle;
	RtfTableStylePtr		m_oDefTableStyle;

//TempFolder
	CString m_sTempFolder;

	OOXReader( RtfDocument& oDocument, std::wstring  path );
	bool Parse();
private: 
	void ParseColorTable( RtfDocument& oDocument );
	CString GetFolder	( CString sDocPath );
	RtfDocument &		m_oDocument;
};