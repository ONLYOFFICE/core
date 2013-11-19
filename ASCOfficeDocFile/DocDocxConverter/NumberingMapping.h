#pragma once

#include "WordDocument.h"
#include "ListTable.h"
#include "StyleSheetMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CharacterPropertiesMapping.h"
#include "RevisionData.h"
#include "AbstractOpenXmlMapping.h"
#include "ConversionContext.h"
#include "IMapping.h"

namespace DocFileFormat
{
	//typedef enum _LevelJustification
	//{
	//	left = 0,
	//	center,
	//	right
	//} LevelJustification;

	static const WCHAR LevelJustificationMap[3][7] =
	{
		_T("left"),
		_T("center"),
		_T("right")
	};

	class NumberingMapping: public AbstractOpenXmlMapping, public IMapping
	{
	public: 
		NumberingMapping(ConversionContext* ctx);
		void Apply(IVisitable* visited);
		static int FindIndexbyId(const list<ListData*>& listData, int id);
		/// Converts the number format code of the binary format.
		static wstring GetNumberFormatWideString(int nfc);
		virtual ~NumberingMapping();

	private:
		// Converts the number text of the binary format to the number text of OOXML.
		// OOXML uses different placeholders for the numbers.
		std::wstring GetLvlText(const ListLevel* lvl, bool bIsSymbol) const;
		static bool IsPlaceholder(WCHAR symbol);
		void LevelMapping(const ListLevel* lvl, unsigned int level, short styleIndex);
		void PictureBulletsMapping();
		void WriteLevelPictureBullet(const CharacterPropertyExceptions* grpprlChpx);
		bool IsPictureBullet(const CharacterPropertyExceptions* grpprlChpx);

	private:	
		ConversionContext*		m_context;
		WordDocument*			m_document;
		WordprocessingDocument*	m_xmldocument;	
	};
}