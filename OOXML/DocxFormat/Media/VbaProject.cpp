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
#include "VbaProject.h"

#include "../Docx.h"
#include "../../XlsxFormat/Xlsx.h"

#include "Media.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../../Binary/Presentation/imagemanager.h"

#include "../IFileContainer.h"
#include "../../XlsxFormat/FileTypes_Spreadsheet.h"
#include "../../../MsBinaryFile/Common/Vba/VbaReader.h"

namespace OOX
{
	void VbaData::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_sXml = oReader.GetOuterXml();
	}
	void VbaData::write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const
	{
		std::wstring sPath = filename.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, m_sXml);

		content.Registration(type().OverrideType(), directory, filename.GetFilename());
	}
	VbaProject::VbaProject( OOX::Document *pMain ) : Media(pMain), OOX::IFileContainer(pMain)
	{
		OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(pMain);
		if (docx)
		{
			docx->m_pVbaProject = this;
		}
		else
		{
			OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(pMain);
			if (xlsx)
			{
				xlsx->m_pVbaProject = this;
			}
		}
	}
	VbaProject::VbaProject(OOX::Document *pMain, const CPath& oRootPath, const CPath& filename) : Media(pMain), OOX::IFileContainer(pMain)
	{
		OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(pMain);
		if (docx)
		{
			docx->m_pVbaProject = this;
		}
		else
		{
			OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(pMain);
			if (xlsx)
			{
				xlsx->m_pVbaProject = this;
			}
		}
		read( oRootPath, filename );
	}
	void VbaProject::read(const CPath& oRootPath, const CPath& oPath)
	{
		IFileContainer::Read( oRootPath, oPath );
		
		Media::read(oPath);
	}
	void VbaProject::write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const
	{
		Media::write(filename, directory, content);

		content.Registration( type().OverrideType(), directory, filename.GetFilename() );
		IFileContainer::Write( filename, directory, content );
	}
	void VbaProject::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, m_filename.GetFilename());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		copy_to(pWriter->m_pCommon->m_pMediaManager->m_strDstMedia);

		CVbaReader vbaReader(m_filename.GetPath(), L"");
		std::wstring sXml = vbaReader.convert();

		if (false == sXml.empty())
		{
			pWriter->StartRecord(0);
			pWriter->WriteStringW2(sXml);
			pWriter->EndRecord();
		}

		smart_ptr<OOX::File> oFile = this->Find(OOX::FileTypes::VbaData);
		smart_ptr<OOX::VbaData> oVbaData = oFile.smart_dynamic_cast<OOX::VbaData>();
		if (oVbaData.IsInit())
		{
			pWriter->StartRecord(1);
			pWriter->WriteStringW2(oVbaData->m_sXml);
			pWriter->EndRecord();			
		}
	}
	void VbaProject::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		OOX::CPath outputPath = pReader->m_pRels->m_pManager->GetDstFolder();

		LONG _rec_size = pReader->GetRecordSize();
		LONG _end_rec = pReader->GetPos() + _rec_size;

		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			switch (_at)
			{
				case 0:
				{
					std::wstring file_name = pReader->GetString2();
					std::wstring inputPath = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media")  + FILE_SEPARATOR_STR + file_name;

					outputPath = outputPath / type().DefaultFileName();
					
					NSFile::CFileBinary::Copy(inputPath, outputPath.GetPath());
					set_filename(outputPath.GetPath(), false);
				}break;
				default:
					break;
			}
		}
		std::wstring sXml;
		while (pReader->GetPos() < _end_rec)
		{
			BYTE rec = pReader->GetUChar();

			switch (rec)
			{
				case 0:
				{
					_INT32 _len = pReader->GetRecordSize();
					sXml = pReader->GetString2();
				}break;
				case 1:
				{
					smart_ptr<OOX::VbaData> oVbaData = new OOX::VbaData(File::m_pMainDocument);
					
					_INT32 _len = pReader->GetRecordSize();
					oVbaData->m_sXml = pReader->GetString2();

					if (false == oVbaData->m_sXml.empty())
					{
						oVbaData->m_bDocument = m_bDocument;
						smart_ptr<OOX::File> oFile = oVbaData.smart_dynamic_cast<OOX::File>();
						this->Add(oFile);

						if (m_bDocument)
						{
							CPath oDirectory(L"/word");
							IFileContainer::Write(outputPath, oDirectory, *(pReader->m_pRels->m_pManager->m_pContentTypes));
						}
					}
				}
				default:
				{
					pReader->SkipRecord();
				}
			}
		}
		pReader->Seek(_end_rec);
	}
} // namespace OOX
