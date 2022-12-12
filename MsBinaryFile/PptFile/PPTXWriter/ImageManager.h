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

#include <string>
#ifndef DISABLE_FILE_DOWNLOADER
#include "../../../Common/Network/FileTransporter/include/FileTransporter.h"
#endif
#include <boost/algorithm/string.hpp>
#include "../Drawing/Attributes.h"
#include "../../../DesktopEditor/common/File.h"


class CMediaManager
{
private:
	std::map<std::wstring, std::wstring>	m_mapMedia;

	long									m_lIndexNextAudio;
	long									m_lIndexNextVideo;
	long									m_lIndexNextImage;

	std::wstring							m_strDstMedia;

public:
    CMediaManager();
    ~CMediaManager();
    void Clear();
    std::wstring FindMedia(const std::wstring& strInput);
    void SetDstMedia(const std::wstring& strDst);
    std::wstring GenerateVideo(const std::wstring& strInput);
    std::wstring GenerateAudio(const std::wstring& strInput);
    std::wstring GenerateImage(const std::wstring& strInput);
    std::wstring GenerateImageJPEG(const std::wstring& strInput);
    std::wstring GenerateMedia(const std::wstring& strInput, const std::wstring& Template, long & Indexer, const std::wstring& strDefaultExt);
    void WriteAudioCollection(const std::vector<PPT::CExFilesInfo>& audioCont);
    bool IsNeedDownload(const std::wstring& strFile);
};

std::wstring CorrectXmlString3(const std::wstring & str);
class CRelsGenerator
{
private:
	PPT::CStringWriter               m_oWriter;
	int										m_lNextRelsID;
	std::map<std::wstring, int>				m_mapMediaRelsID;
	CMediaManager*							m_pManager;
	std::map<std::wstring, std::wstring>	m_mapHyperlinks;

public:
    CRelsGenerator(CMediaManager* pManager);
    ~CRelsGenerator();
    void Clear();

    void StartMaster(int nIndexTheme, int nStartLayoutIndex, int nCountLayouts);

    std::wstring WriteFile(const std::wstring& strPath);

    void StartNotes(int nIndexSlide, bool bMaster);
    void StartSlide(int nIndexLayout, int nIndexNotes);
    void CloseRels();
    void SaveRels(const std::wstring &strFile);
    std::wstring WriteHyperlink(const std::wstring &strHyperlink, bool isExternal = false);
    void StartLayout(int nIndexTheme);


    std::wstring WriteHyperlinkMedia(const std::wstring& strMedia, bool bExternal = true, bool newRIdAlways = false, std::wstring strRelsType = L"http://schemas.microsoft.com/office/2007/relationships/media");
    std::wstring WriteHyperlinkImage(const std::wstring& strImage, bool bExternal = true);
    std::wstring WriteHyperlinkAudio(const std::wstring& strImage, bool bExternal = true);
    std::wstring WriteHyperlinkVideo(const std::wstring& strImage, bool bExternal = true);
    std::wstring WriteMedia(const std::wstring& strMediaPath);
    std::wstring WriteImage(const std::wstring& strImagePath);
    std::wstring WriteSlideRef(const std::wstring& strLocation);
    std::wstring WriteAudio(const std::wstring& strAudioPath, bool & bExternal);
    std::wstring WriteVideo(const std::wstring& strVideoPath, bool & bExternal);

    int getRId()const { return m_lNextRelsID; }
};
