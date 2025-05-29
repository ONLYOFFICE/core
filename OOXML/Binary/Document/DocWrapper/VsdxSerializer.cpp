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
#include "VsdxSerializer.h"

#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/Path.h"

#include "../../Draw/BinaryWriterV.h"
#include "../../Draw/BinaryReaderV.h"
#include "../../Presentation/FontPicker.h"

#include "../../../../OfficeUtils/src/OfficeUtils.h"

namespace BinVsdxRW
{
	int g_nCurFormatVersion = 0;

	CVsdxSerializer::CVsdxSerializer() : m_bIsMacro(false), m_bIsNoBase64(false)
	{
		m_pExternalDrawingConverter = NULL;
	}
	CVsdxSerializer::~CVsdxSerializer()
	{
	}
    void CVsdxSerializer::CreateVsdxFolders(const std::wstring& sDstPath,  std::wstring& sMediaPath, std::wstring& sEmbedPath)
	{
        OOX::CPath pathMediaDir = sDstPath + FILE_SEPARATOR_STR + _T("visio") + FILE_SEPARATOR_STR + _T("media");
		OOX::CPath pathEmbedDir = sDstPath + FILE_SEPARATOR_STR + _T("visio") + FILE_SEPARATOR_STR + _T("embeddings");
		
        //создавать папку надо даже при сохранении в csv, потому что когда читаем из бинарника тему, она записывается в файл.
        OOX::CPath pathVDir = sDstPath + FILE_SEPARATOR_STR + _T("visio");

        OOX::CPath pathThemeDir = pathVDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultDirectory().GetPath();

        OOX::CPath pathThemeFile = pathThemeDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultFileName().GetPath();

        OOX::CPath pathThemeThemeRelsDir = pathThemeDir + FILE_SEPARATOR_STR + _T("_rels");

        NSDirectory::CreateDirectory(pathVDir.GetPath());
        NSDirectory::CreateDirectory(pathThemeDir.GetPath());
        NSDirectory::CreateDirectory(pathThemeThemeRelsDir.GetPath());
        NSDirectory::CreateDirectory(pathMediaDir.GetPath());
        NSDirectory::CreateDirectory(pathEmbedDir.GetPath());

		sMediaPath = pathMediaDir.GetPath();
		sEmbedPath = pathEmbedDir.GetPath();
	}
    _UINT32 CVsdxSerializer::loadFromFile(const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sMediaDir, const std::wstring& sEmbedDir)
	{
        std::wstring strFileInDir = NSSystemPath::GetDirectoryName(sSrcFileName);

		NSBinPptxRW::CDrawingConverter oDrawingConverter;

		oDrawingConverter.SetDstPath(sDstPath + FILE_SEPARATOR_STR + L"visio");
		oDrawingConverter.SetSrcPath(strFileInDir, XMLWRITER_DOC_TYPE_VSDX);

		oDrawingConverter.SetMediaDstPath(sMediaDir);
		oDrawingConverter.SetEmbedDstPath(sEmbedDir);
		oDrawingConverter.SetTempPath(m_sTempDir);

		BinVsdxRW::BinaryFileReader oBinaryFileReader;
		return oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, &oDrawingConverter, m_bIsMacro);
	}
	_UINT32 CVsdxSerializer::saveToFile(const std::wstring& sDstFileName, const std::wstring& sSrcPath)
	{
		COfficeFontPicker* pFontPicker = new COfficeFontPicker();
		pFontPicker->Init(m_sFontDir);
		NSFonts::IFontManager* pFontManager = pFontPicker->get_FontManager();
		DocWrapper::FontProcessor fp;
		fp.setFontManager(pFontManager);

		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetFontManager(pFontManager);
		oOfficeDrawingConverter.SetFontDir(m_sFontDir);
		oOfficeDrawingConverter.SetFontPicker(pFontPicker);

		BinVsdxRW::BinaryFileWriter oBinaryFileWriter(fp);
		_UINT32 result = oBinaryFileWriter.Open(sSrcPath, sDstFileName, &oOfficeDrawingConverter, m_bIsNoBase64);

		RELEASEOBJECT(pFontPicker);
		return result;
	}
	void CVsdxSerializer::setTempDir(const std::wstring& sTempDir)
	{
		m_sTempDir = sTempDir;
	}
	void CVsdxSerializer::setFontDir(const std::wstring& sFontDir)
	{
		m_sFontDir = sFontDir;
	}
    void CVsdxSerializer::setEmbeddedFontsDir(const std::wstring& sEmbeddedFontsDir)
	{
		m_sEmbeddedFontsDir = sEmbeddedFontsDir;
	}
	void CVsdxSerializer::setDrawingConverter(NSBinPptxRW::CDrawingConverter* pDrawingConverter)
	{
		m_pExternalDrawingConverter = pDrawingConverter;
	}
	void CVsdxSerializer::setIsNoBase64(bool val)
	{
		m_bIsNoBase64 = val;
	}
	void CVsdxSerializer::setMacroEnabled(bool val)
	{
		m_bIsMacro = val;
	}
	bool CVsdxSerializer::getMacroEnabled()
	{
		return m_bIsMacro;
	}
};
