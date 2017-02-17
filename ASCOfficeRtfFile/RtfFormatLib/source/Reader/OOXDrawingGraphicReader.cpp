/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "OOXPictureGraphicReader.h"
#include "OOXDrawingGraphicReader.h"
#include "OOXShapeReader.h"

#include "../../../../ASCOfficePPTXFile/ASCOfficeDrawingConverter.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

int OOXGraphicReader::Parse( ReaderParameter oParam , RtfShapePtr & pOutput)
{
	if (m_ooxGraphic == NULL) return 0;
	
	bool bTryPicture = false;
	switch(m_ooxGraphic->m_eGraphicType)
	{
		case OOX::Drawing::graphictypeShape:
		{
			for (size_t i = 0; i < m_ooxGraphic->m_arrItems.size(); i++)
			{
				if (m_ooxGraphic->m_arrItems[i] == NULL) continue;

				if (m_ooxGraphic->m_arrItems[i]->getType() == OOX::et_w_Shape)
				{
					OOXShapeReader shapeReader(dynamic_cast<OOX::Logic::CShape*>(m_ooxGraphic->m_arrItems[i]));
					return (shapeReader.Parse(oParam, pOutput) ? 1 : 0);
				}
			}
		}break;
		case OOX::Drawing::graphictypeGroupShape:
		{
			for (size_t i = 0; i < m_ooxGraphic->m_arrItems.size(); i++)
			{
				if (m_ooxGraphic->m_arrItems[i] == NULL) continue;

				if (m_ooxGraphic->m_arrItems[i]->getType() == OOX::et_w_GroupShape)
				{
					OOXShapeGroupReader groupReader(dynamic_cast<OOX::Logic::CGroupShape*>(m_ooxGraphic->m_arrItems[i]));
					return (groupReader.Parse(oParam, pOutput) ? 1 : 0);
				}
			}
		}break;
		case OOX::Drawing::graphictypePicture:
		case OOX::Drawing::graphictypeLockedCanvas:
		case OOX::Drawing::graphictypeChart:		
		case OOX::Drawing::graphictypeDiagram:
		{//find picture or replacement picture 
			for (size_t i = 0; i < m_ooxGraphic->m_arrItems.size(); i++)
			{
				if (m_ooxGraphic->m_arrItems[i] == NULL) continue;

				if (m_ooxGraphic->m_arrItems[i]->getType() == OOX::et_pic_pic)
				{
					pOutput->m_nShapeType = 75;
				
					OOX::Drawing::CPicture *picture = dynamic_cast<OOX::Drawing::CPicture *>(m_ooxGraphic->m_arrItems[i]);
					OOXShapeReader::Parse(oParam, pOutput, &picture->m_oBlipFill); // тут если false приходит - картинка-потеряшка
					return 1;
				}
			}
		}break;
	}

	return 2;
}

//OOX::Logic::CPicture*
OOX::Logic::CDrawing* OOXDrawingGraphicConverter::Convert( ReaderParameter oParam , RtfShapePtr pOutput)
{
	NSBinPptxRW::CDrawingConverter drawingConverter;

	OOX::CTheme *pTheme = oParam.oDocx->GetTheme();
	if (pTheme)
	{
		NSCommon::smart_ptr<PPTX::Theme> theme(new PPTX::Theme());
		PPTX::FileMap map;
		theme->read(pTheme->m_oReadPath, map);
	
		(*drawingConverter.m_pTheme) = theme.smart_dynamic_cast<PPTX::WrapperFile>(); 
	}

	drawingConverter.SetRelsPath(oParam.oDocx->m_pDocument->m_oReadPath.GetPath());

	std::wstring sXml;
	sXml = drawingConverter.ObjectToDrawingML(m_sXml, XMLWRITER_DOC_TYPE_DOCX);

//	sXml = drawingConverter.ObjectToVML(m_sXml);

	if (sXml.empty())return NULL;

	OOX::CPath pathDrawingRels(drawingConverter.m_strCurrentRelsPath);
	
	if (m_ooxGraphicRels)
	{
		m_ooxGraphicRels->Read(pathDrawingRels, pathDrawingRels);
	}

	std::wstring sBegin	(L"<main xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:p=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\">");
	std::wstring sEnd	(L"</main>");
	
	std::wstring strXml = sBegin + sXml + sEnd;
	
	XmlUtils::CXmlLiteReader oSubReader;
	
	if (oSubReader.FromString(strXml) == false) return NULL;						
	oSubReader.ReadNextNode();

	int nStylesDepth1 = oSubReader.GetDepth();
	while ( oSubReader.ReadNextSiblingNode( nStylesDepth1 ) )
	{
		std::wstring sName = oSubReader.GetName();
		//if (sName == L"w:pict")
		//{
		//	return new OOX::Logic::CPicture(oSubReader);
		//}
		if (sName == L"w:drawing")
		{
			return new OOX::Logic::CDrawing(oSubReader);
		}
	}

	return NULL;
	//return pPict;
}
