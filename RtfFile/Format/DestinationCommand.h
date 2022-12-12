/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once 
#include <boost/algorithm/string.hpp>

#include "RtfReader.h"
#include "RtfDocument.h"
#include "RtfChar.h"
#include "RtfField.h"
#include "RtfShape.h"
#include "RtfMath.h"
#include "RtfTable.h"
#include "RtfBookmark.h"
#include "RtfOle.h"

#include "../OOXml/Writer/OOXWriter.h"
#include "../OOXml/Writer/OOXRelsWriter.h"

#include "../../MsBinaryFile/Common/Vml/PPTShape/Enums.h"

#define COMMAND_RTF_BOOL( pattern, target, command, hasParameter, parameter )\
	else if( pattern == command )\
{\
	if( true == hasParameter && 0 == parameter)\
	target = 0;\
	else\
	target = 1;\
	}
#define COMMAND_RTF_INT( pattern, target, command, hasParameter, parameter )\
	else if( pattern == command )\
{\
	if( true == hasParameter )\
	target = parameter;\
	}
#define COMMAND_RTF_SPECIAL_CHAR( pattern, target, command, hasParameter, parameter )\
	else if( pattern == command  )\
{\
	RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );\
	pNewChar->m_eType = parameter;\
	pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;\
	target->AddItem( pNewChar );\
	}
//Command не имеет состояний
#include "math.h"

class RtfBorderCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter,RtfBorder& oOutput);
};

class RtfShadingCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput );
};

class RtfShadingCellCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput);
};

class RtfShadingCharCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput );
};

class RtfShadingRowCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput);
};

class RtfFrameCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfFrame& oOutput )
	{
		return true;
	}
};

//class RtfOldList;
class TextReader : public RtfAbstractReader
{
private: 
	std::wstring&	m_sName;
	bool			m_bErease;
public:
	TextReader( std::wstring& sName, bool bErease = true );

	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
};

class RtfCharPropsCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfCharProperty * props, bool bLookOnBorder = true);
	//bool bLookOnBorder; Надо ли читать свойства border( актуально для ParagraphReader )
};

class RtfParagraphPropsCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfParagraphProperty * props);
};

class RtfTableCellPropsCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfCellProperty * props);
};

class RtfTableRowPropsCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfRowProperty * props);
};

class RtfOldListReader : public RtfAbstractReader
{
private: 
	RtfOldList& m_oTarget;
public: 
	RtfOldListReader( RtfOldList& oTarget);

	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar );
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfHEXStringReader : public RtfAbstractReader
{
private:
	std::string sHexString;
public: 
	std::pair<boost::shared_array<unsigned char>, int> pDataArray;

	virtual void ExecuteTextInternal2(RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars);
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
};

class RtfThemeDataReader : public RtfHEXStringReader
{
public:
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
};

class RtfDataStoreReader : public RtfHEXStringReader
{
public:
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
};

class RtfColorSchemeReader : public RtfHEXStringReader
{
public:
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
};

class RtfFontTableReader: public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal, is_panose, is_altname };

	RtfFont			m_oFont;
	_InternalState	m_eInternalState;
public: 
	RtfFontTableReader();

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteTextInternal2( RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars);

	std::wstring RemoveLastUnchar(std::wstring str);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	void PopState(RtfDocument& oDocument, RtfReader& oReader);
};

class RtfColorTableReader : public RtfAbstractReader
{
private: 
	RtfColor oCurColor;
	bool m_bIsSet;

public: 
	RtfColorTableReader();

	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar );
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText );
};

class RtfDefCharPropReader: public RtfAbstractReader
{
public: 
	RtfDefCharPropReader();

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfRevisionTableReader : public RtfAbstractReader
{
public: 
	RtfRevisionTableReader();

	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar );
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText );

private:
	std::wstring m_sCurrent;
};

class RtfInfoReader: public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal, is_title, is_subject, is_author, is_manager, is_company, is_operator, is_category, is_keywords,
						 is_comment, is_doccomm, is_hlinkbase, is_creatim, is_revtim, is_printim, is_buptim } ;
	_InternalState m_eInternalState;
public: 

	RtfInfoReader();

	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText );
};

class RtfPictureReader :  public RtfAbstractReader
{
	class PLACEABLEMETAHEADER //заголовок для wmf из rtf ( в rtf wmf не содержит размеров картинки )
	{
	public:
		DWORD Key;           /* Magic number (always 9AC6CDD7h) */
		WORD  Handle;        /* Metafile HANDLE number (always 0) */
		SHORT Left;          /* Left coordinate in metafile units */
		SHORT Top;           /* Top coordinate in metafile units */
		SHORT Right;         /* Right coordinate in metafile units */
		SHORT Bottom;        /* Bottom coordinate in metafile units */
		WORD  Inch;          /* Number of metafile units per inch */
		DWORD Reserved;      /* Reserved (always 0) */
		WORD  Checksum;      /* Checksum value for previous 10 WORDs */

	public:
		PLACEABLEMETAHEADER();

		void CalculateChecksum();

		std::wstring ToString();
		std::wstring ByteToString( BYTE* pbData, int nSize, bool bLittleEnd = true );
	};

private: 
	RtfShape&		m_oShape;
	std::wstring	m_sFile;
	std::wstring	m_sData;
	bool			m_bBin;
	BYTE*			m_pbBin;
	size_t			m_nBinLength;

public: 
	RtfPictureReader( RtfReader& oReader, RtfShape& oShape );
	~RtfPictureReader();
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfOleReader:  public RtfAbstractReader
{
private: 
	RtfOle& m_oOle;
public: 
	RtfOleReader(RtfOle& oOle);

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfOleBinReader : public RtfAbstractReader
{
private: 
	std::vector<std::string>/*&*/ m_arData;
public: 
	RtfOleBinReader();

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	void GetData( BYTE** ppData, long& nSize);
};

class RtfTrackerChangesReader:  public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal,is_charBorder, is_borderTop, is_borderLeft, is_borderBottom, is_borderRight, is_borderBox, is_borderBar,
						 is_borderCellLeft,is_borderCellTop,is_borderCellRight,is_borderCellBottom,is_borderCellLR,is_borderCellRL,
						 is_borderRowLeft,is_borderRowTop,is_borderRowRight,is_borderRowBottom,is_borderRowVer,is_borderRowHor} ;
	//только для определения бордеров
	
	_InternalState				m_eInternalState;

	RtfCharPropertyPtr			m_pCharProps;
	RtfParagraphPropertyPtr		m_pParagraphProps;
	RtfRowPropertyPtr			m_pTableRowProps;
	//RtfCellPropertyPtr		m_pTableCellProps;
	RtfSectionPropertyPtr		m_pSectionProps;

public: 
	RtfTrackerChangesReader(RtfCharPropertyPtr			& prop) : m_pCharProps		(prop), m_eInternalState(is_normal) {}
	RtfTrackerChangesReader(RtfParagraphPropertyPtr		& prop) : m_pParagraphProps	(prop), m_eInternalState(is_normal) {}
	RtfTrackerChangesReader(RtfRowPropertyPtr			& prop) : m_pTableRowProps	(prop), m_eInternalState(is_normal) {}
	//RtfTrackerChangesReader(RtfCellPropertyPtr		& prop) : m_pTableCellProps	(prop), m_eInternalState(is_normal) {}
	RtfTrackerChangesReader(RtfSectionPropertyPtr		& prop) : m_pSectionProps	(prop), m_eInternalState(is_normal) {}

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
};

class RtfShapeReader : public RtfAbstractReader
{
public:
	class ShapePropertyReader : public RtfAbstractReader
	{
	public:
		class ShapePropertyValueReader : public RtfAbstractReader
		{
			std::wstring& m_sPropName;
			RtfShape& m_oShape;

		public:
			std::wstring m_sPropValue;

			ShapePropertyValueReader(std::wstring& sPropName, RtfShape& oShape);

			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
			void PopState( RtfDocument& oDocument, RtfReader& oReader );
			void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText);
		};

	private:
		std::wstring sPropName;
		RtfShape& m_oShape;

	public:
		ShapePropertyReader(RtfShape& oShape);

		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
	};

private: 
	RtfShape& m_oShape;

public: 
	RtfShapeReader( RtfShape& oShape );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfShapeGroupReader : public RtfShapeReader
{
public: 
	RtfShape&	m_oShapeGroup;
	bool		m_bHeader; //чтобы отличать заголовок от вложенных групп

	RtfShapeGroupReader( RtfShape& oShape );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfBackgroundReader : public RtfShapeReader
{
public: 
	RtfShape&		m_oShape;

	RtfBackgroundReader( RtfShape& oShape );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfOldShapeReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;

public: 
	RtfOldShapeReader( RtfShape& oShape );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfShppictReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;

public: 
	RtfShppictReader( RtfShape& oShape );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfAllPictReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;

public: 
	RtfAllPictReader( RtfShape& oShape );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfMathReader: public RtfAbstractReader
{
private: 
	RtfCharProperty m_oCharProp;

public: 
	RtfMathPtr								m_pMath;
	RtfParagraphProperty::ParagraphAlign	m_eParAlign;

	RtfMathReader(RtfMathPtr& pMath);

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	std::wstring ExecuteMathProp(RtfDocument& oDocument, std::string sCommand, int parameter);
	void ExitReader2( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfFieldReader: public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal, is_insert, is_datafield, is_formfield, is_result } InternalState;

	_InternalState	m_eInternalState;
	RtfField&		m_oField;

public: 
	RtfFieldReader( RtfField& oField );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );

	bool IsEmptyText( RtfDocument& oDocument );

private: 
	void TryToPepairResult( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfAnnotElemReader: public RtfAbstractReader
{
public: 
	RtfAnnotElem& m_oAnnot;

	RtfAnnotElemReader( RtfAnnotElem& oAnnot );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
};

class RtfBookmarkStartReader: public RtfAbstractReader
{
public: 
	RtfBookmarkStart& m_oBookmarkStart;

	RtfBookmarkStartReader( RtfBookmarkStart& oBookmark );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
};

class RtfBookmarkEndReader: public RtfAbstractReader
{
public: 
	RtfBookmarkEnd& m_oBookmarkEnd;

	RtfBookmarkEndReader( RtfBookmarkEnd& oBookmark );
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
};

class FootnoteReader;
//Destination имеет состояния
class RtfParagraphPropDestination // todo - последний параграф не обрабатывается
{
public:
	enum _InternalState{ is_normal,is_charBorder, is_borderTop, is_borderLeft, is_borderBottom, is_borderRight, is_borderBox, is_borderBar,
						 is_borderCellLeft,is_borderCellTop,is_borderCellRight,is_borderCellBottom,is_borderCellLR,is_borderCellRL,
						 is_borderRowLeft,is_borderRowTop,is_borderRowRight,is_borderRowBottom,is_borderRowVer,is_borderRowHor} ;
	//только для определения бордера
	
private: 
	_InternalState	m_eInternalState;
	RtfParagraphPtr m_oCurParagraph;

	//реальные параграфы и таблицы
	std::vector< ITextItemPtr >		aCellRenderables;
	std::vector< int >				aItaps;		//вложенность параграфов
	std::vector< RtfTableCellPtr >	aCells;
	std::vector< int >				aCellItaps; //вложенность cell
	std::vector< RtfTableRowPtr >	aRows;
	std::vector< int >				aRowItaps; //вложенность row
	RtfRowProperty oCurRowProperty;

	RtfReader* m_oReader;

public: 
	bool					m_bPar;				// если последняя команда была par, то не надо добавлять параграф
	TextItemContainerPtr	m_oTextItems;		//для разбивки на TextItem
	RtfTab					m_oCurTab;
	int						nTargetItap;		//уровень который считается не таблицей ( для того чтобы читать параграфы в таблицах )
	int						nCurItap;

	RtfParagraphPropDestination( );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,RtfAbstractReader& oAbstrReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);

	void AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow );
	
	void Finalize( RtfReader& oReader);

	void EndRows(RtfReader& oReader);
	void ExecuteNumberChar( RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, int nWinChar, int nMacChar );
};

class RtfFootnoteReader: public RtfAbstractReader
{
private: 
	RtfParagraphPropDestination m_oParPropDest;
public: 
	RtfFootnote&	m_oRtfFootnote;

	RtfFootnoteReader( RtfFootnote& oRtfFootnote );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText );
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfAnnotationReader: public RtfAbstractReader
{
private: 
	RtfParagraphPropDestination m_oParPropDest;

public: 
	RtfAnnotation&	m_oRtfAnnotation;

	RtfAnnotationReader( RtfAnnotation& oRtfAnnotation );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText );
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfDefParPropReader: public RtfAbstractReader
{
private: 
	RtfParagraphPropDestination m_oParPropDest;

public: 
	RtfDefParPropReader( );

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
};

//class LockedExeceptReader: public RtfAbstractReader
//{
//private: 
//	RtfStyleException m_oCurException;
//public:
//	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
//	{
//		if( "lsdlockedexcept" == sCommand )
//			return true;
//		else if( "lsdpriority" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nPriority = parameter;
//		}
//		else if( "lsdunhideused" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nHiddenWhenUse = parameter;
//		}
//		else if( "lsdqformat" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nQFormat = parameter;
//		}
//		else if( "lsdlocked" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nLocked = parameter;
//		}
//		else if( "lsdsemihidden" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nSemiHidden = parameter;
//		}
//		else
//		{
//			return false;
//		}
//		return true;
//	}
//	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText, std::string oTextOriginal )
//	{
//		if( sText.Find(';') != -1 )
//		{
//			sText.Remove(';');
//			m_oCurException.m_sName += sText;
//			oDocument.m_oLatentStyleTable.AddItem( m_oCurException );
//			m_oCurException.SetDefaultRtf();
//		}
//		else
//		{
//			m_oCurException.m_sName += sText;
//		}
//	}
//};
//
//class LatentstyleTableReader: public RtfAbstractReader
//{
//public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
//		{
//			if( "latentstyles" == sCommand )
//				return true;
//			else if( "lsdstimax" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nCount = parameter;
//			}
//			else if( "lsdlockeddef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nLocked = parameter;
//			}
//			else if( "lsdsemihiddendef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nSemiHidden = parameter;
//			}
//			else if( "lsdunhideuseddef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nUnHiddenWhenUse = parameter;
//			}
//			else if( "lsdqformatdef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nQFormat = parameter;
//			}
//			else if( "lsdprioritydef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nPriority = parameter;
//			}
//			else if( "lsdlockedexcept" == sCommand )
//			{
//				LockedExeceptReader oExceptionReader;
//				StartSubReader( oExceptionReader, oDocument, oReader );
//			}
//			else
//			{
//				return false;
//			}
//			return true;
//
//		}
//};
//

class RtfStyleTableReader: public RtfAbstractReader
{
	class RtfStyleReader: public RtfAbstractReader
	{
		//только для определения бордеров
	private:
		enum _InternalState{ is_normal, is_tsbrdrt, is_tsbrdrb, is_tsbrdrl, is_tsbrdrr, is_tsbrdrh, is_tsbrdrv } InternalState;
		
		_InternalState				m_eInternalState;
		RtfParagraphPropDestination	m_oParDest;
		RtfStylePtr					m_oCurStyle;

		//		RtfTableStyleProperty		m_oTableStyleProperty;

	public:
		RtfStyleReader();

		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
		void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
	};

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfListTableReader: public RtfAbstractReader
{
public: 
	class ListReader: public RtfAbstractReader
	{
	public:
		class ListLevelReader: public RtfAbstractReader
		{
		private:
			RtfListLevelProperty & m_oListLevelProp;

		public:
			ListLevelReader(RtfListLevelProperty & oListLevelProp);

			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
			void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
		};

		ListReader(RtfListProperty& oListProp);

		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);

	private:
		RtfListProperty& m_oListProp;
	};

	class ListPictureReader: public RtfAbstractReader
	{
	public:
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	};

	RtfListTableReader();

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfListOverrideTableReader: public RtfAbstractReader
{
private: 
	class ListOverrideReader : public RtfAbstractReader
	{
	private:
		class lfolevelReader : public RtfAbstractReader
		{
		private:
			RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& m_oOverrideLevel;

		public:
			lfolevelReader( RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& oOverrideLevel );

			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
		};
	private:
		RtfListOverrideProperty& m_oProperty;

	public:
		ListOverrideReader(RtfListOverrideProperty& oProperty);

		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	};

public: 
	RtfListOverrideTableReader();

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfDocumentCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
};

class RtfSectionCommand
{
private: 
	enum _InternalState{ is_none, is_border_left, is_border_top, is_border_right, is_border_bottom }InternalState;
	
	_InternalState m_eInternalState;
	int nCurCollumnNumber;

public: 
	RtfSectionCommand();

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfNormalReader : public RtfAbstractReader
{
public: 
	RtfParagraphPropDestination	oParagraphReaderDestination;
	RtfSectionCommand			oRtfSectionCommand;

	RtfNormalReader( RtfDocument& oDocument, RtfReader& oReader );

	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter );
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);

private: 
	void SectDef( RtfDocument& oDocument, RtfReader& oReader );
};

class RtfParagraphReader : public RtfAbstractReader
{
private:
	std::string m_sHeader;

public:
	RtfParagraphPropDestination m_oParPropDest;

	RtfParagraphReader(std::string sHeader, RtfReader& oReader);

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader);
};

class RtfFormFieldReader : public RtfAbstractReader
{
public:
	RtfFormFieldReader(RtfFormField& oFormField);
	
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);

private:
	enum _InternalState {
		is_none, is_name, is_datafield, is_deftext, is_format, is_helptext, is_stattext, is_entrymcr, is_exitmcr, is_list
	};

	_InternalState m_eInternalState;
	RtfFormField& m_oFormField;
};

class RtfFieldInstReader : public RtfAbstractReader, public RtfParagraphPropDestination
{
public: 
	RtfFieldInst& m_oFieldInst;

	RtfFieldInstReader( RtfFieldInst& oFieldInst );
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader , std::string sCommand, bool hasParameter, int parameter);

	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText );
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
};
