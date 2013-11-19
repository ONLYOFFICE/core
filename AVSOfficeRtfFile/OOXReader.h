#pragma once
#include "RtfDocument.h"
#include "OOXRelsReader.h"
#include "RtfField.h"
#include "OOXRelsReader.h"

[ event_source(native)]
class OOXReader
{
public: __event void OnCompleteItem();
public: __event void OnProgress( long nProgress, short* shCancel );
	public: CString m_sPath;
	public: CString m_sDocumentPath;

	public: int m_nCurItap; //для определение вложенности таблицы

	public: int m_nCurFittextId;
	public: CSimpleMap<int, CString> aBookmarks;

	public: OOXRelsReader m_oRelsReader;
	//public: OOXRelsReaderPtr m_oDocRelsReader;
	//public: OOXRelsReader m_oNumRelsReader;

	public: CAtlMap<int, int> m_mapPictureBullet;
	public: CAtlMap<int, TextItemContainerPtr> m_mapFootnotes;
	public: CAtlMap<int, TextItemContainerPtr> m_mapEndnotes;

	public: OOXIdGenerator m_oOOXIdGenerator;

//Theme Fonts
	public: CString m_smajorAscii;
	public: CString m_smajorBidi;
	public: CString m_smajorEastAsia;
	public: CString m_smajorHAnsi;
	public: CString m_sminorAscii;
	public: CString m_sminorBidi;
	public: CString m_sminorEastAsia;
	public: CString m_sminorHAnsi;
//defaultStyles
public: RtfCharStylePtr m_oDefCharStyle;
public: RtfParagraphStylePtr m_oDefParStyle;
public: RtfTableStylePtr m_oDefTableStyle;
//TempFolder
public: CString m_sTempFolder;

	public: OOXReader( RtfDocument& oDocument, CString path );
	public: bool Parse();
	private: void ParseColorTable( RtfDocument& oDocument );
	private: CString GetFolder( CString sDocPath );
	private: RtfDocument& m_oDocument;
};