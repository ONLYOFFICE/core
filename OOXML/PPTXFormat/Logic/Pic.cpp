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

#include "Pic.h"

#include "../../../MsBinaryFile/XlsFile/Converter/ConvertXls2Xlsx.h"
#include "../../../MsBinaryFile/DocFile/Main/DocFormatLib.h"
#include "../../Binary/Document/BinWriter/BinEquationWriter.h"
#include "../../Binary/Document/BinWriter/BinWriters.h"
#include "../../Binary/Document/BinReader/Readers.h"
#include "../../Binary/Document/BinReader/FileWriter.h"
#include "../../Binary/Document/DocWrapper/FontProcessor.h"
#include "../../Binary/Document/DocWrapper/XlsxSerializer.h"
#include "../../Binary/Sheets/Reader/BinaryWriter.h"
#include "../../Binary/Sheets/Writer/BinaryReader.h"
#include "../../Binary/MathEquation/MathEquation.h"

#include "SpTree.h"
#include "Shape.h"

#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"

#include "Media/MediaFile.h"
#include "Media/WavAudioFile.h"

#include "../../DocxFormat/App.h"
#include "../../DocxFormat/Core.h"

#include "../../DocxFormat/Media/Audio.h"
#include "../../DocxFormat/Media/Video.h"
#include "../../DocxFormat/Media/OleObject.h"
#include "../../DocxFormat/Media/ActiveX.h"

#include "../../../Common/OfficeFileFormatChecker.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"

namespace PPTX
{
	namespace Logic
	{
		void COLEObject::fromXML(XmlUtils::CXmlNode& node)
		{
            XmlMacroReadAttributeBase(node, L"DrawAspect",	m_oDrawAspect);
            XmlMacroReadAttributeBase(node, L"r:id",		m_oId);
            XmlMacroReadAttributeBase(node, L"ObjectID",	m_sObjectId);
            XmlMacroReadAttributeBase(node, L"ProgID",		m_sProgId);
            XmlMacroReadAttributeBase(node, L"ShapeID",		m_sShapeId);
            XmlMacroReadAttributeBase(node, L"Type",		m_oType);
            XmlMacroReadAttributeBase(node, L"UpdateMode",	m_oUpdateMode);

			XmlMacroReadAttributeBase(node, L"w:drawAspect", m_oDrawAspect);
			XmlMacroReadAttributeBase(node, L"w:progId", m_sProgId);
			XmlMacroReadAttributeBase(node, L"w:shapeId", m_sShapeId);
			
			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( node, L"relationships:id", m_oId );
			}

			std::wstring ole_bin, ole_image, mspackage;
			
            XmlMacroReadAttributeBase(node, L"pathbin", ole_bin);
            XmlMacroReadAttributeBase(node, L"pathimg", ole_image);
            XmlMacroReadAttributeBase(node, L"mspackage", mspackage);

			if (m_OleObjectFile.IsInit() == false && !ole_bin.empty())
			{
				m_OleObjectFile = new OOX::OleObject(NULL, !mspackage.empty());
				m_OleObjectFile->set_filename		(ole_bin, false);
				m_OleObjectFile->set_filename_cache	(ole_image);
			}
		}

		std::wstring COLEObject::toXML() const
		{
			return L"";
		}
		void COLEObject::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName = L"o:OLEObject";
			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"DrawAspect", m_oDrawAspect);
			if(m_oId.IsInit())
			{
				pWriter->WriteAttribute(L"r:id", m_oId->ToString());
			}
			pWriter->WriteAttribute(L"ObjectID", m_sObjectId);
			pWriter->WriteAttribute2(L"ProgID", m_sProgId);
			pWriter->WriteAttribute(L"ShapeID", m_sShapeId);
			pWriter->WriteAttribute(L"Type", m_oType);
			pWriter->WriteAttribute(L"UpdateMode", m_oUpdateMode);
			pWriter->EndAttributes();

			pWriter->EndNode(strName);
		}
		void COLEObject::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			smart_ptr<OOX::OleObject> ole_file = m_OleObjectFile;

			if (m_oId.IsInit() && ole_file.IsInit() == false)
			{
				OOX::IFileContainer* pRels = pWriter->GetRels().GetPointer();

				ole_file = GetOleObject(m_oId.get(), pRels);
			}
			std::wstring sData;
			std::wstring sProgID = m_sProgId.get_value_or(L"");

			//test xls ole_file for convert to xlsx

			COfficeFileFormatChecker checker;
			if (ole_file.IsInit() && (checker.isXlsFormatFile(ole_file->filename().GetPath()) ||
				checker.isDocFormatFile(ole_file->filename().GetPath())))
			{
				std::wstring sTemp = ole_file->filename().GetDirectory();
				
				std::wstring sResultOoxmlDir = sTemp + FILE_SEPARATOR_STR + _T("ooxml_unpacked");
				NSDirectory::CreateDirectory(sResultOoxmlDir);

				bool bMacro = true;
				_UINT32 nRes = 0;
				std::wstring ooxml_file;

				if (checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)
				{
					nRes = ConvertXls2Xlsx(ole_file->filename().GetPath(), sResultOoxmlDir, L"", L"", sTemp, 0, bMacro);

					ooxml_file = ole_file->filename().GetPath() + (bMacro ? L".xlsm" : L".xlsx");
				}
				else if (checker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)
				{
					COfficeDocFile docFile;
					docFile.m_sTempFolder = ole_file->filename().GetDirectory();

					nRes = docFile.LoadFromFile(ole_file->filename().GetPath(), sResultOoxmlDir, L"", bMacro);

					ooxml_file = ole_file->filename().GetPath() + (bMacro ? L".docm" : L".docx");
				}
				if (0 == nRes)
				{
					COfficeUtils oCOfficeUtils(NULL);
					nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultOoxmlDir, ooxml_file)) ? nRes : S_FALSE;
				}
				NSDirectory::DeleteDirectory(sResultOoxmlDir);
				
				if (0 == nRes && false == ooxml_file.empty())
				{
					ole_file->set_MsPackage(true);
					ole_file->set_filename(ooxml_file, false);
				}
			}

			if (ole_file.IsInit() && 0 == sProgID.find(L"asc."))
			{
				sData = GetOleData(ole_file->filename().GetPath());
			}
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, m_sProgId);
				if(!sData.empty())
					pWriter->WriteString1Data(1, sData.c_str(), sData.length());				
				pWriter->WriteInt2	(2, m_oDxaOrig);
				pWriter->WriteInt2	(3, m_oDyaOrig);
				pWriter->WriteLimit2(4, m_oDrawAspect);
				pWriter->WriteLimit2(5, m_oType);
				pWriter->WriteLimit2(6, m_oUpdateMode);				
				
				if (ole_file.IsInit() && ole_file->isMsPackage() == false)
				{
					std::wstring sExt = ole_file->filename().GetExtention(false);
					if (!sExt.empty())
						pWriter->WriteString1(7, L"maskFile." + sExt); //OleObject Binary FileName Extension (bin, xls, doc, ... other stream file)
				}
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			if (m_oMoveWithCells.IsInit())
			{
				pWriter->StartRecord(5);
				pWriter->WriteBYTE(*m_oMoveWithCells);
				pWriter->EndRecord();
			}
			if (m_oSizeWithCells.IsInit())
			{
				pWriter->StartRecord(6);
				pWriter->WriteBYTE(*m_oSizeWithCells);
				pWriter->EndRecord();
			}
			if (ole_file.IsInit() == false) return;

			if (ole_file->isMsPackage())
			{
				OOX::CPath oox_file		= ole_file->filename();
				OOX::CPath oox_unpacked = oox_file.GetDirectory(true) + L"Temp_" + oox_file.GetFilename();
				
				if (true == NSDirectory::CreateDirectory(oox_unpacked.GetPath()))
				{
					COfficeUtils oOfficeUtils(NULL);
					oOfficeUtils.ExtractToDirectory(oox_file.GetPath(), oox_unpacked.GetPath(), NULL, 0);

					COfficeFileFormatChecker office_checker;
					office_checker.isOOXFormatFile(oox_file.GetPath());
	//-----------------------------------------------------------------------------------------
					DocWrapper::FontProcessor oFontProcessor;
					NSBinPptxRW::CDrawingConverter oDrawingConverter;
				
					NSCommon::smart_ptr<OOX::IFileContainer>	old_rels	= pWriter->GetRels();
					NSCommon::smart_ptr<PPTX::Theme>            old_theme	= *pWriter->m_pTheme;

					NSShapeImageGen::CMediaManager* old_manager = oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager;
					oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager = pWriter->m_pCommon->m_pMediaManager;

					oDrawingConverter.SetFontPicker(pWriter->m_pCommon->m_pFontPicker);

					int type = 0;
					if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX ||
						office_checker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM)
					{
						type = 1;
						BinDocxRW::CDocxSerializer* old_serializer = pWriter->m_pMainDocument;

						BinDocxRW::CDocxSerializer oDocxSerializer;
						oDrawingConverter.m_pBinaryWriter->m_pMainDocument = &oDocxSerializer;
						oDocxSerializer.m_pParamsWriter = new BinDocxRW::ParamsWriter(oDrawingConverter.m_pBinaryWriter, &oFontProcessor, &oDrawingConverter, NULL);

						BinDocxRW::BinaryFileWriter oBinaryFileWriter(*oDocxSerializer.m_pParamsWriter);
						oBinaryFileWriter.intoBindoc(oox_unpacked.GetPath());

						pWriter->m_pMainDocument = old_serializer;
					}
					else if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX ||
						office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM || 
						office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB)
					{
						type = 2;					

						BinXlsxRW::BinaryFileWriter xlsxBinaryWriter(oFontProcessor);
						OOX::Spreadsheet::CXlsx *pXlsxEmbedded = NULL;
						NSBinPptxRW::CXlsbBinaryWriter oXlsbWriter;

						if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB)
							pXlsxEmbedded = new OOX::Spreadsheet::CXlsb();
						else
							pXlsxEmbedded = new OOX::Spreadsheet::CXlsx();
						
						oXlsbWriter.WriteReserved(xlsxBinaryWriter.GetMainTableSize());
						unsigned int nXlsbWriterStartPos = oXlsbWriter.GetPositionAbsolute();

						pXlsxEmbedded->m_pXlsbWriter = &oXlsbWriter;
						pXlsxEmbedded->m_bNeedCalcChain = false;
						
						pXlsxEmbedded->Read(oox_unpacked);
						pXlsxEmbedded->PrepareWorkbook();

						unsigned int nXlsbWriterEndPos = oXlsbWriter.GetPositionAbsolute();

						if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB)
						{
							dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsxEmbedded)->PrepareSi();
							dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsxEmbedded)->PrepareTableFormula();
                            dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsxEmbedded)->ReadSheetData();
						}
						xlsxBinaryWriter.WriteMainTableStart(*oDrawingConverter.m_pBinaryWriter);
						
						if (nXlsbWriterEndPos  > nXlsbWriterStartPos)
						{
							xlsxBinaryWriter.WriteBinaryTable(oXlsbWriter.GetBuffer() + nXlsbWriterStartPos, nXlsbWriterEndPos  - nXlsbWriterStartPos);
						}
						xlsxBinaryWriter.WriteContent(pXlsxEmbedded, NULL, &oDrawingConverter);					
						xlsxBinaryWriter.WriteMainTableEnd();

						pXlsxEmbedded->m_pXlsbWriter = NULL;
						
						delete pXlsxEmbedded;
					}
					//else if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)
					//{
					//}
					else
					{//unknown ms package
						oDrawingConverter.m_pBinaryWriter->WriteString1(2, ole_file->filename().GetFilename());
					}
		
					pWriter->SetRels(old_rels);
					*pWriter->m_pTheme = old_theme;
					oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager = old_manager;
//---------------------------------------------------------------------------------------------------------------------					
					pWriter->StartRecord(1);
					pWriter->WriteBYTE(type);
					pWriter->EndRecord();

					pWriter->StartRecord(2);
					pWriter->WriteBYTEArray(oDrawingConverter.m_pBinaryWriter->GetBuffer(), oDrawingConverter.m_pBinaryWriter->GetPosition());
					pWriter->EndRecord();

					NSDirectory::DeleteDirectory(oox_unpacked.GetPath());
				}
			}
			else if ( std::wstring::npos != sProgID.find(L"Equation"))
			{

                pWriter->StartRecord(1);
					pWriter->WriteBYTE(4);
				pWriter->EndRecord();

				MathEquation::CEquationReader		oReader		(ole_file->filename().GetPath().c_str());
                MathEquation::BinaryEquationWriter	oBinEqWriter(pWriter);
				
				oReader.SetOutputDev(&oBinEqWriter);
				
				pWriter->StartRecord(2);
					oReader.Parse();
				pWriter->EndRecord();
			}
		}

		void COLEObject::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		m_sProgId	= pReader->GetString2();
				else if (1 == _at)	m_sData		= pReader->GetString2();
				else if (2 == _at)	m_oDxaOrig	= pReader->GetLong();
				else if (3 == _at)	m_oDyaOrig	= pReader->GetLong();
				else if (4 == _at)	m_oDrawAspect = pReader->GetUChar();
				else if (5 == _at)	m_oType = pReader->GetUChar();
				else if (6 == _at)	m_oUpdateMode = pReader->GetUChar();
				else if (7 == _at)//OleObject Binary FileName (bin, xls, doc, ... other stream file)
				{
					m_OleObjectFile = new OOX::OleObject(NULL, false, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
					std::wstring strOlePath = pReader->GetString2(true);
					m_OleObjectFile->set_filename(strOlePath, false); //temp !!! for ImageManager original file name
				}
				else
					break;
			}

			if (m_sData.IsInit() && m_OleObjectFile.IsInit() == false)
			{
				m_OleObjectFile = new OOX::OleObject(NULL, false, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
			}

			BYTE embedded_type = 0;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 1:
					{
						pReader->GetLong(); //skip size
						embedded_type = pReader->GetUChar();				
					}break;
					case 2:
					{
						LONG _embed_data_size	= pReader->GetLong();
						LONG _end_embed_data	= pReader->GetPos() + _embed_data_size + 4;

						if (_embed_data_size < 1) 
							break;
//------------------------------------------------------------------
						std::wstring sDstEmbedded = pReader->m_pRels->m_pManager->GetDstMedia();
						int nPos = (int)sDstEmbedded.rfind(wchar_t('m'));
						if (-1 != nPos)
							sDstEmbedded = sDstEmbedded.substr(0, nPos);

						sDstEmbedded += L"embeddings";
//------------------------------------------------------------------
						if (true == NSDirectory::CreateDirectory(sDstEmbedded))
						{
							std::wstring sDstEmbeddedTemp = sDstEmbedded + FILE_SEPARATOR_STR + L"Temp";
							NSDirectory::CreateDirectory(sDstEmbeddedTemp);

							if (embedded_type == 0) //unknown ms package
							{
								m_OleObjectFile = new OOX::OleObject(NULL, true, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);

								BYTE type = pReader->GetUChar();
								std::wstring strOlePath = pReader->GetString2(true);
								m_OleObjectFile->set_filename(strOlePath, false); //temp !!! for ImageManager original file name
							}
							else if (embedded_type == 4)
							{
								pReader->Seek(pReader->GetPos() - 4); //roll back to size record
								std::wstring sXmlContent;
								if (pReader->m_pMainDocument)
								{
									pReader->m_pMainDocument->getXmlContentElem(OOX::et_m_oMathPara, *pReader, sXmlContent);
								}
								else
								{
									BinDocxRW::CDocxSerializer		oDocxSerializer;
									NSBinPptxRW::CDrawingConverter	oDrawingConverter;

									oDrawingConverter.SetMainDocument(&oDocxSerializer);

									//oDocxSerializer.m_pParamsWriter = new BinDocxRW::ParamsWriter(oDrawingConverter.m_pBinaryWriter, &oFontProcessor, &oDrawingConverter, NULL);
									oDocxSerializer.m_pCurFileWriter = new Writers::FileWriter(L"", L"", false, 111, &oDrawingConverter, L"");

									oDocxSerializer.getXmlContentElem(OOX::et_m_oMathPara, *pReader, sXmlContent);
								}

								if (!sXmlContent.empty())
								{
									m_sAlternateContenteXml = sXmlContent;
								}
							}
							else if (embedded_type == 1)
							{
								m_OleObjectFile = new OOX::OleObject(NULL, true, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);

								int id = pReader->m_nCountEmbedded++;

								BinDocxRW::CDocxSerializer		oDocxSerializer;
								NSBinPptxRW::CDrawingConverter	oDrawingConverter;

								std::wstring sThemePath, sMediaPath, sEmbedPath;
								oDocxSerializer.CreateDocxFolders(sDstEmbeddedTemp, sThemePath, sMediaPath, sEmbedPath);

								oDrawingConverter.m_pReader->Init(pReader->GetData() + pReader->GetPos(), 0, _embed_data_size);

								oDrawingConverter.SetMainDocument(&oDocxSerializer);

								oDrawingConverter.SetDstPath(sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"word");
								oDrawingConverter.SetSrcPath(pReader->m_strFolder, 1);

								oDrawingConverter.SetMediaDstPath(sMediaPath);
								oDrawingConverter.SetEmbedDstPath(sEmbedPath);

								std::wstring sDocxFilename = L"Microsoft_Word_Document" + std::to_wstring(id) + L".docx";

								NSBinPptxRW::CBinaryFileReader& oBufferedStream = *oDrawingConverter.m_pReader;

								oDocxSerializer.m_pCurFileWriter = new Writers::FileWriter(sDstEmbeddedTemp, L"", false, 111, &oDrawingConverter, sThemePath);

								BinDocxRW::BinaryFileReader oBinaryFileReader(pReader->m_strFolder, oBufferedStream, *oDocxSerializer.m_pCurFileWriter);
								oBinaryFileReader.ReadFile();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
												//themes
								oDocxSerializer.m_pCurFileWriter->m_oTheme.Write(sThemePath);

								OOX::CContentTypes *pContentTypes = oDrawingConverter.GetContentTypes();
								//docProps
								OOX::CPath pathDocProps = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"docProps";
								NSDirectory::CreateDirectory(pathDocProps.GetPath());

								OOX::CPath DocProps = std::wstring(L"docProps");

								OOX::CApp oApp(NULL);
								oApp.SetDefaults();
								oApp.write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, *pContentTypes);

								OOX::CCore oCore(NULL);
								oCore.SetDefaults();
								oCore.write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, *pContentTypes);

								oDocxSerializer.m_pCurFileWriter->Write();
								pContentTypes->Write(sDstEmbeddedTemp);

								COfficeUtils oOfficeUtils(NULL);
								oOfficeUtils.CompressFileOrDirectory(sDstEmbeddedTemp, sDstEmbedded + FILE_SEPARATOR_STR + sDocxFilename, true);
//------------------------------------------------------------------
															//std::wstring sEmbWorksheetRelsName	= L"embeddings/" + sDocxFilename;
															//std::wstring sEmbWorksheetRelType	= OOX::FileTypes::MicrosoftOfficeWordDocument.RelationType();
															//
															//m_oId = pReader->m_pRels->WriteRels(sEmbWorksheetRelType, sEmbWorksheetRelsName, std::wstring());
								m_OleObjectFile->set_filename(sDstEmbedded + FILE_SEPARATOR_STR + sDocxFilename, false);

								pReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(L"docx");
							}
							else if (embedded_type == 2)
							{
								m_OleObjectFile = new OOX::OleObject(NULL, true, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);

								int id = pReader->m_nCountEmbedded++; //todoooo -> countEmbeddedObjects

								OOX::Spreadsheet::CXlsx			oXlsx;
								BinXlsxRW::BinaryFileReader		oEmbeddedReader;
								NSBinPptxRW::CDrawingConverter	oDrawingConverter;

								std::wstring sDrawingsPath = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"drawings";
								std::wstring sThemePath = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"theme";
								std::wstring sEmbeddingsPath = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"embeddings";

								BinXlsxRW::SaveParams oSaveParams(sDrawingsPath, sEmbeddingsPath, sThemePath, oDrawingConverter.GetContentTypes(), NULL, true);

								std::wstring sXmlOptions, sMediaPath, sEmbedPath;
								BinXlsxRW::CXlsxSerializer::CreateXlsxFolders(sXmlOptions, sDstEmbeddedTemp, sMediaPath, sEmbedPath);

								boost::unordered_map<std::wstring, size_t>	old_enum_map = oXlsx.m_mapEnumeratedGlobal;

								oXlsx.m_mapEnumeratedGlobal.clear();

								oDrawingConverter.m_pReader->Init(pReader->GetData() + pReader->GetPos(), 0, _embed_data_size);

								oDrawingConverter.SetDstPath(sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl");
								oDrawingConverter.SetSrcPath(pReader->m_strFolder, 2);

								oDrawingConverter.SetMediaDstPath(sMediaPath);
								oDrawingConverter.SetEmbedDstPath(sEmbedPath);

								oEmbeddedReader.ReadMainTable(oXlsx, *oDrawingConverter.m_pReader, pReader->m_strFolder, sDstEmbeddedTemp, oSaveParams, &oDrawingConverter);

								oXlsx.PrepareToWrite();

								oXlsx.Write(sDstEmbeddedTemp, *oSaveParams.pContentTypes);

								COfficeUtils oOfficeUtils(NULL);
								std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(id) + (oSaveParams.bMacroEnabled ? L".xlsm" : L".xlsx");
								oOfficeUtils.CompressFileOrDirectory(sDstEmbeddedTemp, sDstEmbedded + FILE_SEPARATOR_STR + sXlsxFilename, true);

								oXlsx.m_mapEnumeratedGlobal = old_enum_map;
//------------------------------------------------------------------
															//std::wstring sEmbWorksheetRelsName	= L"embeddings/" + sXlsxFilename;
															//std::wstring sEmbWorksheetRelType	= OOX::FileTypes::MicrosoftOfficeExcelWorksheet.RelationType();
															//
															//m_oId = pReader->m_pRels->WriteRels(sEmbWorksheetRelType, sEmbWorksheetRelsName, std::wstring());
								m_OleObjectFile->set_filename(sDstEmbedded + FILE_SEPARATOR_STR + sXlsxFilename, false);

								pReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(oSaveParams.bMacroEnabled ? L"xlsm" : L"xlsx");
							}
							NSDirectory::DeleteDirectory(sDstEmbeddedTemp);
						}
						pReader->Seek(_end_embed_data);
					}break;
					case 5:
					{
						pReader->GetLong(); //skip size
						m_oMoveWithCells = (0 != pReader->GetUChar());
					}break;
					case 6:
					{
						pReader->GetLong(); //skip size
						m_oSizeWithCells = (0 != pReader->GetUChar());
					}break;
				}
			}
			pReader->Seek(_end_rec);
		}  
		void COLEObject::FillParentPointersForChilds()
		{
		}
		bool COLEObject::isValid()
		{
			return m_sProgId.IsInit() && (m_sData.IsInit() || m_oId.IsInit() || m_OleObjectFile.IsInit());
		}

		smart_ptr<OOX::OleObject> COLEObject::GetOleObject(const OOX::RId& oRId, OOX::IFileContainer* pRels) const
		{
			smart_ptr<OOX::OleObject> ole_file = m_OleObjectFile;
			if (ole_file.IsInit() == false)
			{
				if (pRels != NULL)
					ole_file = pRels->Get<OOX::OleObject>(oRId);
				else
				{
					OOX::IFileContainer* pContainer = dynamic_cast<OOX::IFileContainer*>(const_cast<PPTX::WrapperFile*>(parentFile));
					if (pContainer)
						ole_file = pContainer->Get<OOX::OleObject>(oRId);
				}
			}
			return ole_file;
		}
		std::wstring COLEObject::GetOleData(const std::wstring& sFilePath)const
		{
			std::wstring sRes;
			//EncodingMode.unparsed https://github.com/tonyqus/npoi/blob/master/main/POIFS/FileSystem/Ole10Native.cs
			POLE::Storage oStorage(sFilePath.c_str());
			if(oStorage.open(false, false))
			{
				POLE::Stream oStream(&oStorage, L"Ole10Native");
				if(oStream.size() > 4)
				{
					BYTE head[] = {0x00, 0x00, 0x00, 0x00};
					oStream.read(head, 4);
					_UINT32 nDataSize = (_UINT32)((head[0]<< 0) | ((head[1]) << 8) | ((head[2]) << 16) | ((head[3]) << 24));
					BYTE* aData = new BYTE[nDataSize];
					oStream.read(aData, nDataSize);
					sRes = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(aData, nDataSize);
					RELEASEARRAYOBJECTS(aData);
				}
			}
			return sRes;
		}

		Pic::Pic(std::wstring ns)
		{
			m_namespace = ns;
			m_pLevelUp	= NULL;
		}
		Pic::~Pic()
		{
		}
		Pic::Pic(XmlUtils::CXmlNode& node)
		{
			m_pLevelUp	= NULL;
			fromXML(node);
		}
		Pic::Pic(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pLevelUp	= NULL;
			fromXML(oReader);
		}		
		
		const Pic& Pic::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		const Pic& Pic::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		void Pic::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
			
			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;
					
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("nvPicPr") == strName)
					nvPicPr = oReader;
				else if (_T("blipFill") == strName)
					blipFill = oReader;
				else if (_T("spPr") == strName)
					spPr = oReader;
				else if (_T("style") == strName)
					style = oReader;
			}
			FillParentPointersForChilds();
		}

		void Pic::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"macro", macro);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode item;
					oNodes.GetAt(i, item);

					std::wstring strName = XmlUtils::GetNameNoNS(item.GetName());

					if (_T("nvPicPr") == strName)
						nvPicPr = item;
					else if (_T("blipFill") == strName)
						blipFill = item;
					else if (_T("spPr") == strName)
						spPr = item;
					else if (_T("style") == strName)
					{
						if (!style.IsInit())
							style = item;
					}
				}

				if (!blipFill.blip.is_init())
				{
					XmlUtils::CXmlNode oNodeMSAlternate;
					if (node.GetNode(_T("mc:AlternateContent"), oNodeMSAlternate))
					{
						XmlUtils::CXmlNode oNodeFallback;
						if (oNodeMSAlternate.GetNode(_T("mc:Fallback"), oNodeFallback))
						{
							blipFill = oNodeFallback.ReadNode(_T("p:blipFill"));
						}
					}
				}
			}			
			
			FillParentPointersForChilds();
		}

		std::wstring Pic::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"macro", macro);

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvPicPr);
			oValue.Write(blipFill);
			oValue.Write(spPr);
			oValue.WriteNullable(style);

			return XmlUtils::CreateNode(m_namespace + L":pic", oAttr, oValue);
		}
		
		void Pic::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if(oleObject.IsInit())
			{
				pWriter->StartRecord(SPTREE_TYPE_OLE);
				
				OOX::CVmlDrawing *pVml = NULL;
				
				if (parentFileIs<PPTX::Slide>())
				{
					pVml = parentFileAs<PPTX::Slide>().Vml.GetPointer();
				}
				else if (parentFileIs<PPTX::SlideMaster>())
				{
					pVml = parentFileAs<PPTX::SlideMaster>().Vml.GetPointer();
				}
				if (oleObject->m_sShapeId.IsInit() && pVml && !blipFill.blip->embed.IsInit() && blipFill.blip->oleFilepathImage.empty())
				{					
                    boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind = pVml->m_mapShapes.find(*oleObject->m_sShapeId);

                    if (pVml->m_mapShapes.end() != pFind)
					{
						pFind->second.bUsed = true;
						OOX::Vml::CVmlCommonElements* pShape = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pFind->second.pElement);

						if (pShape)
						{						
                            for(size_t i = 0; i < pShape->m_arrItems.size(); ++i)
							{
                                OOX::WritingElement* pChildElemShape = pShape->m_arrItems[i];

								if(OOX::et_v_imagedata == pChildElemShape->getType())
								{
									OOX::Vml::CImageData* pImageData = static_cast<OOX::Vml::CImageData*>(pChildElemShape);									
														
									std::wstring sIdImageFileCache;

									if (pImageData->m_oRelId.IsInit())		sIdImageFileCache = pImageData->m_oRelId->GetValue();
									else if (pImageData->m_rId.IsInit())	sIdImageFileCache = pImageData->m_rId->GetValue();
									else if (pImageData->m_rPict.IsInit())	sIdImageFileCache = pImageData->m_rPict->GetValue();
																		
									if (!sIdImageFileCache.empty())
									{
										//ищем физический файл ( rId относительно vml_drawing)									
										smart_ptr<OOX::File> pFile = pVml->Find(sIdImageFileCache);
										
										if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
										{
											OOX::Image*	pImageFileCache = static_cast<OOX::Image*>(pFile.GetPointer());
											
											blipFill.blip->oleFilepathImage = pImageFileCache->filename().GetPath();
										}
									}
								}
							}
						}
					}
				}
			}
			else if (nvPicPr.nvPr.media.is_init())
			{
				blipFill.additionalFile = GetMediaLink(pWriter);  

				smart_ptr<OOX::Media> mediaFile = blipFill.additionalFile.smart_dynamic_cast<OOX::Media>();
				if (mediaFile.IsInit() && blipFill.blip.IsInit())
				{
					blipFill.blip->mediaFilepath = mediaFile->filename().GetPath();
					blipFill.blip->mediaExternal = mediaFile->IsExternal();
				}

				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if (nvPicPr.nvPr.media.as<MediaFile>().name == L"audioFile")
						pWriter->StartRecord(SPTREE_TYPE_AUDIO);
					else if (nvPicPr.nvPr.media.as<MediaFile>().name == L"videoFile" || 
							 nvPicPr.nvPr.media.as<MediaFile>().name == L"quickTimeFile")
						pWriter->StartRecord(SPTREE_TYPE_VIDEO);
					else 
						pWriter->StartRecord(SPTREE_TYPE_PIC);
				}
				else if (nvPicPr.nvPr.media.is<WavAudioFile>() || nvPicPr.nvPr.media.is<AudioCD>())
				{
					pWriter->StartRecord(SPTREE_TYPE_AUDIO);
				}
				else
					pWriter->StartRecord(SPTREE_TYPE_PIC);
			}
			else
			{
				pWriter->StartRecord(SPTREE_TYPE_PIC);
			}

			if (blipFill.additionalFile.is<OOX::Media>())
			{
				smart_ptr<OOX::Media> mediaFile = blipFill.additionalFile.smart_dynamic_cast<OOX::Media>();
				
				pWriter->StartRecord(5);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					std::wstring sExt = mediaFile->filename().GetExtention(false);
					if (!sExt.empty() && !mediaFile->IsExternal())
						pWriter->WriteString1(0, L"maskFile." + sExt);
					else
						pWriter->WriteString1(0, mediaFile->filename().GetPath());

					pWriter->WriteBool1(1, mediaFile->IsExternal());
				//todoo start, end positions ..

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				pWriter->EndRecord();
			}

			pWriter->WriteRecord2(4, oleObject);
			pWriter->WriteRecord1(0, nvPicPr);
			pWriter->WriteRecord1(1, blipFill);
			pWriter->WriteRecord1(2, spPr);
			pWriter->WriteRecord2(3, style);

			if (macro.IsInit())
			{
				pWriter->StartRecord(SPTREE_TYPE_MACRO);
				pWriter->WriteString1(0, *macro);
				pWriter->EndRecord();
			}
			pWriter->EndRecord();
		}

		void Pic::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;
			bool bOle = false;
			
			if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					 pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_ = L"pic";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			if (pWriter->m_lDocType != XMLWRITER_DOC_TYPE_XLSX && 
				pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX &&
				pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				if(oleObject.IsInit() && oleObject->isValid())
				{
					bOle = true;
					pWriter->WriteString(L"<p:graphicFrame><p:nvGraphicFramePr><p:cNvPr id=\"0\" name=\"\"/><p:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></p:cNvGraphicFramePr><p:nvPr><p:extLst><p:ext uri=\"{D42A27DB-BD31-4B8C-83A1-F6EECF244321}\"><p14:modId xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"2157879785\"/></p:ext></p:extLst></p:nvPr></p:nvGraphicFramePr>");
					if(spPr.xfrm.IsInit())
					{
						std::wstring oldName = spPr.xfrm->node_name;
						spPr.xfrm->node_name = _T("p:xfrm");
						spPr.xfrm->toXmlWriter(pWriter);
						spPr.xfrm->node_name = oldName;
					}
					pWriter->WriteString(L"<a:graphic><a:graphicData uri=\"http://schemas.openxmlformats.org/presentationml/2006/ole\">");

					pWriter->StartNode(_T("p:oleObj"));
					pWriter->WriteAttribute(L"name", (std::wstring)L"oleObj");
					if(oleObject->m_oId.IsInit())
					{
						pWriter->WriteAttribute2(L"r:id", oleObject->m_oId->get());
					}
					if(oleObject->m_oDxaOrig.IsInit())
					{
						int nDxaOrig = oleObject->m_oDxaOrig.get();
						pWriter->WriteAttribute(L"imgW", 635 * nDxaOrig); //twips to emu
					}
					if(oleObject->m_oDyaOrig.IsInit())
					{
						int nDyaOrig = oleObject->m_oDyaOrig.get();
						pWriter->WriteAttribute(L"imgH", 635 * nDyaOrig); //twips to emu
					}
					if ((oleObject->m_oDrawAspect.IsInit()) && (oleObject->m_oDrawAspect->GetBYTECode() == 1))
					{
						pWriter->WriteAttribute(L"showAsIcon", 1); 
					}
					pWriter->WriteAttribute2(L"progId", oleObject->m_sProgId);
					pWriter->EndAttributes();

					pWriter->WriteString(L"<p:embed/>");
				}
			}
			pWriter->StartNode(namespace_ + L":pic");

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("xmlns:pic"), (std::wstring)_T("http://schemas.openxmlformats.org/drawingml/2006/picture"));
			}
			pWriter->WriteAttribute(L"macro", macro);
			pWriter->EndAttributes();

			nvPicPr.toXmlWriter(pWriter);

			blipFill.m_namespace = namespace_;
			blipFill.toXmlWriter(pWriter);

			pWriter->m_lFlag = 1;
			spPr.toXmlWriter(pWriter);
			pWriter->m_lFlag = 0;
			
			pWriter->Write(style);

			pWriter->EndNode(namespace_ + L":pic");
			
			if (pWriter->m_lDocType != XMLWRITER_DOC_TYPE_XLSX &&
				pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX && 
				pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				if(bOle)
				{
					pWriter->WriteString(L"</p:oleObj></a:graphicData></a:graphic></p:graphicFrame>");
				}
			}
		}


		void Pic::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			nvPicPr.cNvPr.id = -1;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						nvPicPr.fromPPTY(pReader);						
					}break;
					case 1:
					{
						blipFill.fromPPTY(pReader);
						if(oleObject.IsInit() && blipFill.blip.IsInit())
						{
							if (oleObject->m_OleObjectFile.IsInit() == false)
								oleObject->m_OleObjectFile = new OOX::OleObject(NULL, false, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
							
							oleObject->m_OleObjectFile->set_filename_cache (blipFill.blip->oleFilepathImage);

							if (NSFile::CFileBinary::Exists(oleObject->m_OleObjectFile->filename().GetPath()) == false)
							{
								oleObject->m_OleObjectFile->set_filename (blipFill.blip->oleFilepathBin, false);
							}
						}

						smart_ptr<OOX::Media> mediaFile = blipFill.additionalFile.smart_dynamic_cast<OOX::Media>();
						if (mediaFile.IsInit() && blipFill.blip.IsInit())
						{
							if (!mediaFile->IsExternal() && NSFile::CFileBinary::Exists(mediaFile->filename().GetPath()) == false)
							{
								mediaFile->set_filename (blipFill.blip->mediaFilepath, 
										false == NSFile::CFileBinary::Exists(blipFill.blip->mediaFilepath));
							}
						}
					}break;
					case 2:
					{
						spPr.fromPPTY(pReader);						
					}break;
					case 3:
					{
						style = new ShapeStyle(L"p");
						style->fromPPTY(pReader);						
					}break;
					case 4:
					{
						oleObject = new COLEObject();
						oleObject->fromPPTY(pReader);
						
						if(oleObject->m_sData.IsInit())
							blipFill.oleData = oleObject->m_sData.get();

						blipFill.additionalFile = oleObject->m_OleObjectFile.smart_dynamic_cast<OOX::File>();
					}break;
					case 5:
					{
						bool isExternal = false;
						std::wstring strMediaFileMask;

						LONG _end_rec1 = pReader->GetPos() + pReader->GetRecordSize() + 4;
						pReader->Skip(1); // start attributes
						while (true)
						{
							BYTE _at = pReader->GetUChar_TypeNode();
							if (_at == NSBinPptxRW::g_nodeAttributeEnd)
								break;

							if (0 == _at)		
							{
								strMediaFileMask = pReader->GetString2();
							}
							else if (1 == _at)		
							{
								isExternal = pReader->GetBool();
							}
							else
								break;
						}
						smart_ptr<OOX::Media> mediaFile = blipFill.additionalFile.smart_dynamic_cast<OOX::Media>();
						if (mediaFile.IsInit())
						{
							mediaFile->set_filename(strMediaFileMask, isExternal);
						}
						pReader->Seek(_end_rec1);
					}break;
					case SPTREE_TYPE_MACRO:
					{
						pReader->Skip(5); // type + size
						macro = pReader->GetString2();
					}break;
					default:
					{
						break;
					}
				}
			}
			if(blipFill.blip.IsInit()  && blipFill.additionalFile.IsInit())
			{
				if (!blipFill.blip->oleRid.empty() && oleObject.IsInit())
				{
					oleObject->m_oId = OOX::RId(blipFill.blip->oleRid);

					if (oleObject->m_OleObjectFile.IsInit() == false)
					{
						oleObject->m_OleObjectFile = new OOX::OleObject(NULL, false, pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
						
						oleObject->m_OleObjectFile->set_filename		(blipFill.blip->oleFilepathBin, false);
						oleObject->m_OleObjectFile->set_filename_cache	(blipFill.blip->oleFilepathImage);
					}
				}
				if (!blipFill.blip->mediaRid.empty())
				{
					smart_ptr<OOX::Media> mediaFile = blipFill.additionalFile.smart_dynamic_cast<OOX::Media>();                    
					bool bExternal = mediaFile->IsExternal();
					
                    std::wstring strMediaRelsPath;
                    if (pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX)	strMediaRelsPath = L"media/";
                    else														strMediaRelsPath = L"../media/";
					
					if (bExternal)
					{
						strMediaRelsPath = mediaFile->filename().GetFilename();
					}
					else
					{
						strMediaRelsPath += mediaFile->filename().GetFilename();

						PPTX::Logic::Ext ext;
						ext.link	= OOX::RId(blipFill.blip->mediaRid);
						nvPicPr.nvPr.extLst.push_back(ext);
					}

                    unsigned int nRId = 0;
					if (blipFill.additionalFile.is<OOX::Audio>())
					{
						nvPicPr.nvPr.media.Media = new PPTX::Logic::MediaFile(L"audioFile");
                        nRId = pReader->m_pRels->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio", strMediaRelsPath, bExternal ? L"External" : L"");

					}
					if (blipFill.additionalFile.is<OOX::Video>())
					{
						nvPicPr.nvPr.media.Media = new PPTX::Logic::MediaFile(L"videoFile");
						nRId = pReader->m_pRels->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video", strMediaRelsPath, bExternal ? L"External" : L"");
					}

					if (nvPicPr.nvPr.media.Media.IsInit() && nRId > 0)
					{
						PPTX::Logic::MediaFile& mediaFile = nvPicPr.nvPr.media.Media.as<PPTX::Logic::MediaFile>();
						mediaFile.link = OOX::RId((size_t)nRId);
					}
					if (nvPicPr.cNvPr.hlinkClick.IsInit() == false)
						nvPicPr.cNvPr.hlinkClick.Init();

					nvPicPr.cNvPr.hlinkClick->id		= L"";
					nvPicPr.cNvPr.hlinkClick->action	= L"ppaction://media";
				}
			}

			pReader->Seek(_end_rec);
		}

		void Pic::FillLevelUp()
		{
			if ((m_pLevelUp == NULL) && (nvPicPr.nvPr.ph.IsInit()))
			{
				if ((nvPicPr.nvPr.ph->type.IsInit()) || (nvPicPr.nvPr.ph->idx.IsInit()))
				{
					if (parentFileIs<Slide>())
					{
						parentFileAs<Slide>().Layout->GetLevelUp(this);
					}
					else if(parentFileIs<SlideLayout>())
					{
						parentFileAs<SlideLayout>().Master->GetLevelUp(this);
					}
					else if(parentFileIs<NotesSlide>())
					{
						parentFileAs<NotesSlide>().master_->GetLevelUp(this);
					}
				}
			}
		}

		void Pic::Merge(Pic& pic, bool bIsSlidePlaceholder)
		{
			if (m_pLevelUp)
				m_pLevelUp->Merge(pic, true);

			pic.nvPicPr		= nvPicPr;			
			pic.blipFill	= blipFill;
			pic.oleObject	= oleObject;

			spPr.Merge(pic.spPr);

			if (style.is_init())
			{
				pic.style = style;
				pic.style->SetParentFilePointer(parentFile);
			}
		}

		void Pic::FillParentPointersForChilds()
		{
			nvPicPr.SetParentPointer(this);
			blipFill.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if (style.IsInit())
				style->SetParentPointer(this);
			if (oleObject.IsInit())
				oleObject->SetParentPointer(this);
		}

		void Pic::GetRect(Aggplus::RECT& pRect)const
		{
			pRect.bottom	= 0;
			pRect.left		= 0;
			pRect.right		= 0;
			pRect.top		= 0;

			if (spPr.xfrm.IsInit())
			{
				pRect.left		= spPr.xfrm->offX.get_value_or(0);
				pRect.top		= spPr.xfrm->offY.get_value_or(0);
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		std::wstring Pic::GetFullPicName()const
		{
			if (blipFill.blip.IsInit())
				return blipFill.blip->GetFullPicName();
			return _T("");
		}
		smart_ptr<OOX::File> Pic::GetMediaLink(NSBinPptxRW::CBinaryFileWriter* pWriter)const
		{
			smart_ptr<OOX::File>  file;
			OOX::IFileContainer* pRels = NULL;
			
			if (parentFileIs<Slide>())
			{
				pRels = dynamic_cast<OOX::IFileContainer*>(const_cast<PPTX::WrapperFile*>(parentFile));
			}
			//else if ..layout, master
			else if (pWriter)
			{
				pRels = pWriter->GetRels().GetPointer();
			}
			if (pRels)
			{
				if (nvPicPr.nvPr.media.is<WavAudioFile>())
				{
					return pRels->Find(nvPicPr.nvPr.media.as<WavAudioFile>().embed.get());
				}

				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					file = pRels->Find(nvPicPr.nvPr.media.as<MediaFile>().link.get());		
					smart_ptr<OOX::Media> mediaFile = file.smart_dynamic_cast<OOX::Media>();
					
					if ( (mediaFile.IsInit() == false || mediaFile->filename().GetPath() == L"NULL") && !nvPicPr.nvPr.extLst.empty())
					{
						//todooo - почему везде нулевой то? - сделать поиск по всем uri
						file = pRels->Find(nvPicPr.nvPr.extLst[0].link.get());
					}		
				}

			}
			return file;
		}

		DWORD Pic::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			fill.SetParentFilePointer(parentFile);

			if (style.IsInit())
			{
				if (parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if (parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if (parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				BGRA = style->fillRef.Color.GetBGRA();
			}

			if (spPr.Fill.is_init())
				spPr.Fill.Merge(fill);
			return BGRA;
		}

		DWORD Pic::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			line.SetParentFilePointer(parentFile);

			if (style.is_init())
			{
				if (parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if (parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if (parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if (spPr.ln.IsInit())
				spPr.ln->Merge(line);
			return BGRA;
		}

		
		double Pic::GetStTrim () const
		{
			double trim = 0.0;
			
			if (parentFileIs<FileContainer>())
			{
				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if ((nvPicPr.nvPr.media.as<MediaFile>().name == _T("videoFile")) ||
						(nvPicPr.nvPr.media.as<MediaFile>().name == _T("quickTimeFile")) ||
						(nvPicPr.nvPr.media.as<MediaFile>().name == _T("audioFile")) )
					{
						if (std::wstring (_T("NULL")) == parentFileAs<FileContainer>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get()) )	//	HAVE TRIM
						{
							if(nvPicPr.nvPr.extLst.size())
							{
								if (nvPicPr.nvPr.extLst[0].st.is_init())
									trim = nvPicPr.nvPr.extLst[0].st.get();
							}
						}		
					}
				}
			}

			return trim;
		}

		double Pic::GetEndTrim () const
		{
			double trim = -1.0;
			
			if (nvPicPr.nvPr.media.is<MediaFile>())
			{
				if ((nvPicPr.nvPr.media.as<MediaFile>().name == _T("videoFile")) ||
					(nvPicPr.nvPr.media.as<MediaFile>().name == _T("quickTimeFile")) ||
					(nvPicPr.nvPr.media.as<MediaFile>().name == _T("audioFile")) )
				{
					if (parentFileIs<FileContainer>())
					{
						if (std::wstring (_T("NULL")) == parentFileAs<FileContainer>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get()))	//	HAVE TRIM
						{
							if(nvPicPr.nvPr.extLst.size())
							{
								if (nvPicPr.nvPr.extLst[0].end.is_init())
									trim = nvPicPr.nvPr.extLst[0].end.get();
							}
						}		
					}
				}
			}

			return trim;
		}

		long Pic::GetRefId() const
		{
			return (long) nvPicPr.cNvPr.id;
		}

		void Pic::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::Theme>& oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& oClrMap, bool in_group)
		{
			bool bOle = oleObject.IsInit() && oleObject->isValid();
			std::wstring sOleNodeName;
			
			if (XMLWRITER_DOC_TYPE_XLSX != pWriter->m_lDocType)
			{
				if(bOle && oleObject->m_oDxaOrig.IsInit() && oleObject->m_oDyaOrig.IsInit())
				{
					sOleNodeName = L"w:object";
					pWriter->StartNode(sOleNodeName);
					pWriter->StartAttributes();
					pWriter->WriteAttribute(L"w:dxaOrig", oleObject->m_oDxaOrig);
					pWriter->WriteAttribute(L"w:dyaOrig", oleObject->m_oDyaOrig);
					pWriter->EndAttributes();
				}
				else
				{
					//sOleNodeName = L"w:pict";
					//pWriter->StartNode(sOleNodeName);
					//pWriter->StartAttributes();
					//pWriter->EndAttributes();
				}
			}

			int nShapeId = pWriter->m_lObjectIdVML;
           
			std::wstring strId			= L"_x0000_i"	+ std::to_wstring(pWriter->m_lObjectIdVML);
			std::wstring strSpid		= L"_x0000_s"	+ std::to_wstring(pWriter->m_lObjectIdVML);

			std::wstring strObjectid	= L"_152504" + std::to_wstring(pWriter->m_lObjectIdVML);
			pWriter->m_lObjectIdVML++;

			int dL = 0, dT = 0, dW = 0, dH = 0;
			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->offX.is_init())	dL = *spPr.xfrm->offX;
				if (spPr.xfrm->offY.is_init())	dT = *spPr.xfrm->offY;
				if (spPr.xfrm->extX.is_init())	dW = *spPr.xfrm->extX;
				if (spPr.xfrm->extY.is_init())	dH = *spPr.xfrm->extY;
			}		
			
			NSBinPptxRW::CXmlWriter oStylesWriter;

            if(pWriter->m_strStyleMain.empty())
			{
				oStylesWriter.WriteAttributeCSS (L"position", L"absolute");
				if (in_group)
				{
					oStylesWriter.WriteAttributeCSS_int(L"left",	dL / 100);
					oStylesWriter.WriteAttributeCSS_int(L"top",		dT / 100);
					oStylesWriter.WriteAttributeCSS_int(L"width",	dW / 100);
					oStylesWriter.WriteAttributeCSS_int(L"height",	dH / 100);
				}
				else
				{
					oStylesWriter.WriteAttributeCSS_int_pt(L"left",		dL / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"top",		dT / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"width",	dW / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"height",	dH / 12700);
				}
			}

			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->rot.is_init())
				{
					int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
					oStylesWriter.WriteAttributeCSS_int(L"rotation", nRot);
				}
				bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
				bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
				if (bIsFH && bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"xy");
				}
				else if (bIsFH)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"x");
				}
				else if (bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"y");
				}
			}

            std::wstring strPath;
            std::wstring strTextRect;

			LONG lW = 43200;
			LONG lH = 43200;
			if (spPr.xfrm.is_init())
			{
				lW = spPr.xfrm->extX.get_value_or(43200);
				lH = spPr.xfrm->extY.get_value_or(43200);
			}

			spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);

			pWriter->StartNode(L"v:shapetype");
			pWriter->StartAttributes();
				pWriter->WriteAttribute(L"type", L"#_x0000_t75");
				pWriter->WriteAttribute(L"o:spt", L"75");
				pWriter->WriteAttribute(L"coordsize", L"21600,21600");
				pWriter->WriteAttribute(L"o:preferrelative", L"t");
				pWriter->WriteAttribute(L"path", L"m@4@5l@4@11@9@11@9@5xe");
			pWriter->EndAttributes();
			pWriter->StartNode(L"v:formulas");
				pWriter->EndAttributes();
				pWriter->WriteString(L"<v:f eqn=\"if lineDrawn pixelLineWidth 0\"/>\
<v:f eqn=\"sum @0 1 0\"/>\
<v:f eqn=\"sum 0 0 @1\"/>\
<v:f eqn=\"prod @2 1 2\"/>\
<v:f eqn=\"prod @3 21600 pixelWidth\"/>\
<v:f eqn=\"prod @3 21600 pixelHeight\"/>\
<v:f eqn=\"sum @0 0 1\"/>\
<v:f eqn=\"prod @6 1 2\"/>\
<v:f eqn=\"prod @7 21600 pixelWidth\"/>\
<v:f eqn=\"sum @8 21600 0\"/>\
<v:f eqn=\"prod @7 21600 pixelHeight\"/>\
<v:f eqn=\"sum @10 21600 0\"/>");
				pWriter->EndNode(L"v:formulas");
			pWriter->EndNode(L"v:shapetype");
			
			pWriter->StartNode(L"v:shape");
			pWriter->StartAttributes();

			if (pWriter->m_strId.empty())
			{
				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(L"id", strSpid); //??
				}
				else
				{
					pWriter->WriteAttribute(L"id", strId);
					pWriter->WriteAttribute(L"o:spid", strSpid);
				}
			}
			else
			{
				pWriter->WriteAttribute(L"id", pWriter->m_strId);
				pWriter->WriteAttribute(L"o:spid", strSpid);
				pWriter->m_strId.clear();
			}
			pWriter->WriteAttribute(L"type", L"#_x0000_t75");
			
			pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain + pWriter->m_strStyleWrap + oStylesWriter.GetXmlString());

			if (!pWriter->m_strAttributesMain.empty())
			{
				pWriter->WriteString(pWriter->m_strAttributesMain);
				pWriter->m_strAttributesMain.clear();
			}
			if(bOle)
			{
				pWriter->WriteAttribute(L"filled", L"f");
			}
			std::wstring strNodeVal;
			if (!spPr.ln.is_init())
			{
				pWriter->WriteAttribute(L"stroked", L"false");
			}
			else
			{
				std::wstring strPenAttr = _T("");
				nullable<ShapeStyle> pShapeStyle;
				
				CalculateLine(pWriter->m_lDocType, spPr, pShapeStyle, oTheme, oClrMap, strPenAttr, strNodeVal, bOle);
				pWriter->WriteString(strPenAttr);
			}

			pWriter->EndAttributes();

			pWriter->StartNode(L"v:path");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"textboxrect", strTextRect);
			pWriter->EndAttributes();
			pWriter->EndNode(L"v:path");

			pWriter->WriteString(pWriter->m_strNodes);
			pWriter->m_strNodes.clear();
			
			if (blipFill.blip.is_init() && blipFill.blip->embed.is_init())
			{
				pWriter->StartNode(L"v:imagedata");
				pWriter->StartAttributes();
				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(L"o:relid", blipFill.blip->embed->ToString());
				}
				else
				{
					pWriter->WriteAttribute(L"r:id", blipFill.blip->embed->ToString());
				}
				pWriter->WriteAttribute(L"o:title", L"");
				pWriter->EndAttributes();
				pWriter->EndNode(L"v:imagedata");
			}

			if (m_sClientDataXml.IsInit())
				pWriter->WriteString(*m_sClientDataXml);

			pWriter->EndNode(L"v:shape");

			pWriter->m_strStyleMain.clear();
			pWriter->m_strStyleWrap.clear();

			if (bOle)
			{
				oleObject->m_sObjectId = strObjectid;
				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					oleObject->m_sShapeId = std::to_wstring(nShapeId);
				}
				else
				{
					oleObject->m_sShapeId = strId;
					oleObject->toXmlWriter(pWriter);
				}
			}
			if (!sOleNodeName.empty())
			{
				pWriter->EndNode(sOleNodeName);
			}
		}
		void Pic::fromXMLOle(XmlUtils::CXmlNode& node)
		{
			oleObject.Init();
			
			XmlMacroReadAttributeBase(node, L"showAsIcon", oleObject->m_oShowAsIcon);
            XmlMacroReadAttributeBase(node, L"progId",	oleObject->m_sProgId);
            XmlMacroReadAttributeBase(node, L"r:id",	oleObject->m_oId);
			XmlMacroReadAttributeBase(node, L"name", oleObject->m_oName);

			if (false == oleObject->m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( node, L"relationships:id", oleObject->m_oId );
			}	
			if ((oleObject->m_oShowAsIcon.IsInit()) && (*oleObject->m_oShowAsIcon))
			{
				oleObject->m_oDrawAspect = (BYTE)1;
			}

			int imgW = node.GetAttributeInt(std::wstring(L"imgW"), 0);			
			if(imgW > 0)
			{
				oleObject->m_oDxaOrig = (int)Emu_To_Twips(imgW);
			}
			
			int imgH = node.GetAttributeInt(std::wstring(L"imgH"), 0);			
			if(imgH > 0)
			{
				oleObject->m_oDyaOrig = (int)Emu_To_Twips(imgH);
			}
			if(oleObject->m_oId.IsInit())
			{
				if (blipFill.blip.IsInit() == false)
				{
					blipFill.blip.Init();
					blipFill.stretch.Init();
					blipFill.stretch->fillRect.Init();
				}
				blipFill.blip->oleRid = oleObject->m_oId->get();
			}
            XmlMacroReadAttributeBase(node, L"spid",	oleObject->m_sShapeId);
		}
	} // namespace Logic
} // namespace PPTX
