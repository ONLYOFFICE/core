/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "../DocxFormat/IFileContainer.h"
#include "../../DesktopEditor/common/Directory.h"

namespace OOX
{
	class CApp;
	class CCore;

	namespace Draw
	{
		class CDocumentFile;
//---------------------------------------------------------------------------------------------------------------------------------
		class CValidation : public OOX::File
		{
		public:
			CValidation(OOX::Document* pMain);
			CValidation(OOX::Document* pMain, const CPath& uri);
			CValidation(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CValidation();

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring m_strFilename;


		};
		class CComments : public OOX::File
		{
		public:
			CComments(OOX::Document* pMain);
			CComments(OOX::Document* pMain, const CPath& uri);
			CComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CComments();

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring m_strFilename;


		};
//---------------------------------------------------------------------------------------------------------------------------------
		class CVsdx : public OOX::Document, public OOX::IFileContainer
		{
		public:
			CVsdx();
			CVsdx(const CPath& oFilePath);
			virtual ~CVsdx();

			bool Read(const CPath& oFilePath);
			bool Write(const CPath& oDirPath, OOX::CContentTypes &oContentTypes);

			smart_ptr<Draw::CDocumentFile> m_pDocument;
			
			smart_ptr<CApp> m_pApp;
			smart_ptr<CCore> m_pCore;

			std::map<unsigned int, smart_ptr<OOX::File>> mapMastersByID;
			std::map<unsigned int, smart_ptr<OOX::File>> mapPagesByID;
		};

	} //Draw
} // OOX

