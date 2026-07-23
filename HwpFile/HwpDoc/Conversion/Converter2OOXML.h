/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/CtrlShapeVideo.h"
#include "../Paragraph/CtrlCharacter.h"
#include "../Paragraph/CtrlShapePic.h"
#include "../Paragraph/CtrlShapeOle.h"
#include "../Paragraph/CtrlEqEdit.h"
#include "../Paragraph/CtrlTable.h"
#include "../Paragraph/CtrlNote.h"
#include "../Paragraph/ParaText.h"

#include "../Paragraph/TblCell.h"

#include "FootnoteConverter.h"
#include "OleConverter.h"
#include "NumberingConverter.h"
#include "StyleConverter.h"

#include "../Common/WriterContext.h"

namespace HWP
{
struct TContentType
{
	HWP_STRING m_wsName;
	HWP_STRING m_wsType;
};

struct TLineData
{
	ELineStyle2 m_eStyle;
	int m_nColor;
	int m_nThick;
	HWP_BYTE m_nCompoundLineType;

	ELineArrowStyle m_eHeadStyle;
	ELineArrowSize  m_eHeadSize;
	ELineArrowStyle m_eTailStyle;
	ELineArrowSize  m_eTailSize;

	TLineData(ELineStyle2 eStyle, int nColor,
	          int nThick, HWP_BYTE nCompoundLineType,
	          ELineArrowStyle eHeadStyle, ELineArrowSize eHeadSize,
	          ELineArrowStyle eTailStyle, ELineArrowSize eTailSize);

	TLineData(ELineStyle2 eStyle, int nColor,
	          int nThick, HWP_BYTE nCompoundLineType);
};

enum class ECellCreator
{
	FILE,
	EMPTY,
	NOT_CREATED
};

class CConverter2OOXML
{
	friend class CNumberingConverter;

	CWriterContext * m_pContext;
	HWP_STRING m_sTempDirectory;

	NSStringUtils::CStringBuilder m_oDocXml;      // document.xml
	NSStringUtils::CStringBuilder m_oNoteXmlRels; // footnotes.xml.rels
	NSStringUtils::CStringBuilder m_oWebSettings; // webSettings.xml

	VECTOR<TContentType> m_arDefaultContentType;
	VECTOR<TContentType> m_arContentTypes;
	VECTOR<TRelationship> m_arRelationships;

	CNumberingConverter m_oNumberingConverter;
	CFootnoteConverter m_oFootnoteConverter;
	COleConverter m_oOleConverter;
	CStyleConverter m_oStyleConverter;

	unsigned short m_ushShapeCount;
	unsigned short m_ushPageCount;
	unsigned short m_ushTableCount;
	unsigned short m_ushEquationCount;

	unsigned short m_ushBookmarkCount;

	void CreateEmptyFiles();
	void FillDefaultData();
	void Close();
	void Convert();

	bool IsRasterFormat(const HWP_STRING& sFormat);

	void WriteParagraphProperties(short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteTable(const CCtrlTable* pTable, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteTableProperties(const CCtrlTable* pTable, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteCell(const CTblCell* pCell, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, ECellCreator eCellCreator);
	void WriteCellProperties(short shBorderFillID, NSStringUtils::CStringBuilder& oBuilder);
	void WriteBorder(const TBorder& oBorder, const HWP_STRING& sBorderName, NSStringUtils::CStringBuilder& oBuilder);

	void WriteGeometryShape(const CCtrlGeneralShape* pGeneralShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, const CCtrlContainer *pContainer = nullptr);
	void WriteEqEditShape(const CCtrlEqEdit* pEqEditShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteOleShape(const CCtrlShapeOle* pOleShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteContainer(const CCtrlContainer* pContainer, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteSectionSettings(TConversionState& oState);
	void WritePicture(const CCtrlShapePic* pCtrlPic, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteVideo(const CCtrlShapeVideo* pCtrlVideo, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	bool SaveSVGFile(const HWP_STRING& sSVG, HWP_STRING& sFileName);
	HWP_STRING SavePicture(const HWP_STRING& sBinItemId, TConversionState& oState);

	void WriteParaShapeProperties(short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteRunnerStyle(short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, const CRunnerStyle& sExternStyles = CRunnerStyle());
	void WriteTextBorderStyle(short shBorderFillId, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void OpenDrawingNode(const CCtrlObjElement* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);
	void CloseDrawingNode(const CCtrlObjElement* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);

	void WriteShapePosition(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);
	void WriteShapeExtent(const CCtrlObjElement* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);
	void WriteShapeWrapMode(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);
	void WriteShapeProperty(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);

	void OpenParagraph(short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void CloseParagraph(NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteText(const CParaText* pParaText, const VECTOR<TRangeTag>& arRangeTags, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteText(const HWP_STRING& wsText, short shParaShapeID, short shParaStyleID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, const CRunnerStyle& oExternalStyle = CRunnerStyle());
	void WriteLineSettings(const CCtrlGeneralShape* pCtrlGeneralShape, NSStringUtils::CStringBuilder& oBuilder);
	void WriteLineSettings(const TLineData& oLineData, NSStringUtils::CStringBuilder& oBuilder);
	void WriteBorderSettings(const CCtrlShapePic* pCtrlPic, NSStringUtils::CStringBuilder& oBuilder);

	void WriteAutoNumber(const CCtrlAutoNumber* pAutoNumber, short shParaShapeID, short shParaStyleID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteCharacter(const CCtrlCharacter* pCharacter, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteShape(const CCtrlGeneralShape* pShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, const CCtrlContainer* pContainer = nullptr);

	void WriteNote(const CCtrlNote* pNote, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteField(const CCtrlField* pHyperlink, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void CloseField(const CCtrlField* pHyperlink, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteCaption(const CCtrlCommon* pCtrlCommon, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteEmptyParagraph(short shParaShapeID, short shParaStyleID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	HWP_STRING AddRelationship(const HWP_STRING& wsType, const HWP_STRING& wsTarget, TConversionState* pState = nullptr);
	void AddContentType(const HWP_STRING& wsName, const HWP_STRING& wsType);
	void AddDefaultContentType(const HWP_STRING& wsName);
public:
	CConverter2OOXML();
	~CConverter2OOXML();

	void WriteParagraph(const CHWPPargraph* pParagraph, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void SetContext(CWriterContext* pContext);
	void SetTempDirectory(const HWP_STRING& sTempDirectory);
	bool ConvertToFile(const HWP_STRING& sFilePath);
	bool ConvertToDir(const HWP_STRING& sDirectoryPath);

	HWP_STRING GetTempDirectory() const;
};
}

#endif // CONVERTER2OOXML_H
