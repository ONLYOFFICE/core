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

#include "PPTXWriter.h"

namespace NSBinPptxRW
{
		CPPTXWriter::CPPTXWriter() : m_oPresentation(&m_oDocument), m_oTableStyles(&m_oDocument), m_oVmlDrawing(&m_oDocument),
						m_oApp(&m_oDocument), m_oCore(&m_oDocument), m_oViewProps(&m_oDocument), m_oPresProps(&m_oDocument), m_oDefaultNote(&m_oDocument)

		{
			m_oPresentation.m_bMacroEnabled = false;
			m_bIsDefaultNoteMaster = true;
		}
		CPPTXWriter::~CPPTXWriter()
		{
		}
		void CPPTXWriter::Init(std::wstring strFolder, bool bMacro)
		{
			m_strDstFolder = strFolder;

			OOX::CPath pathPPT = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt");
			OOX::CPath pathDocProps = m_strDstFolder + FILE_SEPARATOR_STR + _T("docProps");

			NSDirectory::CreateDirectory(m_strDstFolder);
			NSDirectory::CreateDirectory(pathDocProps.GetPath());
			NSDirectory::CreateDirectory(pathPPT.GetPath());

			m_oImageManager.Clear();

			m_oImageManager.SetDstFolder(pathPPT.GetPath());

			OOX::CPath pathMedia = pathPPT / _T("media");
			m_oImageManager.SetDstMedia(pathMedia.GetPath());
			NSDirectory::CreateDirectory(pathMedia.GetPath());

			OOX::CPath pathCharts = pathPPT / _T("charts");
			m_oImageManager.SetDstCharts(pathCharts.GetPath());

			OOX::CPath pathEmbeddings = pathPPT / _T("embeddings");
			m_oImageManager.SetDstEmbed(pathEmbeddings.GetPath());

			m_oReader.m_pRels->m_pManager = &m_oImageManager;

			OOX::CPath pathTheme = pathPPT  / _T("theme");
			NSDirectory::CreateDirectory(pathTheme.GetPath());

			m_oReader.m_strFolderThemes = pathTheme.GetPath();

			m_oPresentation.m_bMacroEnabled = bMacro;
		}
		void CPPTXWriter::OpenPPTY(BYTE* pBuffer, int len, std::wstring srcFolder, std::wstring strThemesFolder)
		{
			int start_pos = 0;

			int cur_pos = start_pos;
			while (cur_pos < len && pBuffer[cur_pos] != (BYTE)(';'))
				++cur_pos;

			if (cur_pos == len || cur_pos == start_pos)
				return;

			std::string __str_ppty((char*)(pBuffer + start_pos), cur_pos - start_pos);
			start_pos = cur_pos + 1;

			cur_pos = start_pos;
			while (cur_pos < len && pBuffer[cur_pos] != (BYTE)(';'))
				++cur_pos;

			if (cur_pos == len || cur_pos == start_pos)
				return;

			std::string __str_version((char*)(pBuffer + start_pos), cur_pos - start_pos);
			start_pos = cur_pos + 1;

			cur_pos = start_pos;
			while (cur_pos < len && pBuffer[cur_pos] != (BYTE)(';'))
				++cur_pos;

			if (cur_pos == len || cur_pos == start_pos)
				return;

			std::string __str_decode_len_ansi((char*)(pBuffer + start_pos), cur_pos - start_pos);
			std::wstring  __str_decode_len = std::wstring(__str_decode_len_ansi.begin(), __str_decode_len_ansi.end());
			start_pos = cur_pos + 1;

			pBuffer += start_pos;
			len -= start_pos;
			int dstLenTemp = XmlUtils::GetInteger(__str_decode_len);
			//int dstLenTemp = Base64DecodeGetRequiredLength(len);

			int nVersion = 1;
			if (__str_version.length() > 1)
			{
				nVersion = std::stoi(__str_version.substr(1).c_str());
			}
			bool bIsNoBase64 = nVersion == g_nFormatVersionNoBase64;

			BYTE* pDstBuffer = NULL;
			int dstLen = 0;
			if (!bIsNoBase64)
			{
				BYTE* pDstBuffer = new BYTE[dstLenTemp];
				int dstLen = dstLenTemp;
				Base64::Base64Decode((const char*)pBuffer, len, pDstBuffer, &dstLen);
				m_oReader.Init(pDstBuffer, 0, dstLen);
			}
			else
			{
				pDstBuffer = pBuffer - start_pos;
				dstLen = len + start_pos;
				m_oReader.Init(pDstBuffer, 0, dstLen);
				m_oReader.Seek(start_pos);
			}

			m_oReader.m_strFolder = srcFolder;
			m_oReader.m_strFolderExternalThemes = strThemesFolder;

			for (LONG i = 0; i < 30/*main tables max*/; ++i)
			{
				BYTE _type = 0;

				if (false == m_oReader.GetUCharWithResult(&_type))
					break;

				if (0 == _type)
					break;

				m_mainTables.insert(std::pair<BYTE, LONG>(_type, m_oReader.GetLong()));
			}

			std::map<BYTE, LONG>::iterator pPair;

			CXmlWriter oXmlWriter;

			// первым делом определим количество необходимого. если хоть одно из этих чисел - ноль, то ппту не корректный
			LONG nCountThemes = 0;
			LONG nCountMasters = 0;
			LONG nCountLayouts = 0;
			LONG nCountSlides = 0;
			bool bNotesMasterPresent = false;

			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Themes);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountThemes = m_oReader.GetLong();
			}
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::SlideMasters);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountMasters = m_oReader.GetLong();
			}
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::SlideLayouts);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountLayouts = m_oReader.GetLong();
			}
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Slides);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountSlides = m_oReader.GetLong();
			}

			if (/*0 == nCountThemes || */0 == nCountMasters || 0 == nCountLayouts/* || 0 == nCountSlides*/) //rev 60054 - презентация без слайдов
			{
				return;
			}

			// теперь создадим массивы для рельсов
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				_slideMasterInfo elm;
				m_arSlideMasters_Theme.push_back(elm);
			}
			for (LONG i = 0; i < nCountLayouts; ++i)
			{
				m_arSlideLayouts_Master.push_back(0);
			}
			for (LONG i = 0; i < nCountSlides; ++i)
				m_arSlides_Layout.push_back(0);

			for (LONG i = 0; i < nCountSlides; ++i)
				m_arSlides_Notes.push_back(-1);

	// ThemeRels
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::ThemeRels);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				m_oReader.Skip(5); // type + len

				LONG _count = m_oReader.GetLong();
				for (LONG i = 0; i < _count; ++i)
				{
					BYTE _master_type = m_oReader.GetUChar(); // must be 0
					ReadMasterInfo(i);
				}
			}

	// нужно проставить всем шаблонам мастер.
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				size_t _countL = m_arSlideMasters_Theme[i].m_arLayouts.size();
				for (size_t j = 0; j < _countL; ++j)
				{
					int index = m_arSlideMasters_Theme[i].m_arLayouts[j];

					if (index >= 0 && index < (int)m_arSlideLayouts_Master.size())
					{
						m_arSlideLayouts_Master[index] = (LONG)i;
					}
				}
			}

	// готово, теперь нужно слайдам проставить шаблоны
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::SlideRels);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				m_oReader.Skip(6); // type + len + start attr

				size_t index = 0;
				while (true)
				{
					BYTE _at = m_oReader.GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					//m_arSlides_Layout[_at] = m_oReader.GetULong(); тут прописан не индекс, а тип - смотри - oBinaryWriter.WriteInt1(0, oBinaryWriter.m_pCommon->m_oSlide_Layout_Rels[i]);
					if (index < m_arSlides_Layout.size())
						m_arSlides_Layout[index++] = m_oReader.GetULong();
				}
			}
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::SlideNotesRels);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				m_oReader.Skip(6); // type + len + start attr

				size_t index = 0;
				while (true)
				{
					BYTE _at = m_oReader.GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					//m_arSlides_Layout[_at] = m_oReader.GetULong(); тут прописан не индекс, а тип - смотри - oBinaryWriter.WriteInt1(0, oBinaryWriter.m_pCommon->m_oSlide_Layout_Rels[i]);
					if (index < m_arSlides_Notes.size())
						m_arSlides_Notes[index++] = m_oReader.GetULong();
				}
			}

			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::NotesMastersRels);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				m_oReader.Skip(6); // type + len + start attr

				size_t index = 0;
				while (true)
				{
					BYTE _at = m_oReader.GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					m_arNotesMasters_Theme.push_back(m_oReader.GetULong());
				}
			}
	// теперь нужно удалить все themes, которые не ведут на мастерслайды
			std::vector<LONG> arThemes;
			std::vector<LONG> arThemesDst;
			std::vector<bool> arThemesSave;

			for (LONG i = 0; i < nCountThemes; ++i)
			{
				arThemes.push_back(i);
				arThemesDst.push_back(0);
				arThemesSave.push_back(false);
			}
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				int ind = m_arSlideMasters_Theme[i].m_lThemeIndex;
				if (ind < arThemesSave.size())
				{
					arThemesSave[ind] = true;
				}
			}

			for (size_t i = 0; i < m_arNotesMasters_Theme.size(); i++)
			{
				int index = m_arNotesMasters_Theme[i];
				if (index >= 0 && index < (int)arThemesSave.size())
				{
					arThemesSave[index] = true;
				}
			}
			LONG lCurrectTheme = 0;
			for (LONG i = 0; i < nCountMasters && i < arThemesSave.size(); ++i)
			{
				if (!arThemesSave[i])
					continue;
				arThemesDst[i] = lCurrectTheme;
				++lCurrectTheme;
			}
	// теперь нужно перебить ссылки
			for (LONG i = 0; i < nCountMasters && i < arThemesDst.size(); ++i)
			{
				m_arSlideMasters_Theme[i].m_lThemeIndex = arThemesDst[i];
			}
	// themes
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Themes);
			if (m_mainTables.end() != pPair)
			{
				OOX::CPath pathFolder = m_oReader.m_strFolderThemes;
				OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

				NSDirectory::CreateDirectory(pathFolder.GetPath());
				NSDirectory::CreateDirectory(pathFolderRels.GetPath());

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);

				for (LONG i = 0; i < nCountThemes; ++i)
				{
					if (!arThemesSave[i])
					{
						// это ненужная тема
						continue;
					}

					PPTX::Theme elm(&m_oDocument);
					m_arThemes.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartTheme();
					try
					{
						m_arThemes[i].fromPPTY(&m_oReader);
					}
					catch (...)
					{
					}

					std::wstring strMasterXml = L"theme" + std::to_wstring(i + 1) + L".xml";
					oXmlWriter.ClearNoAttack();

					m_oReader.m_pRels->CloseRels();

					m_arThemes[i].toXmlWriter(&oXmlWriter);

					OOX::CPath pathFile = pathFolder + FILE_SEPARATOR_STR + strMasterXml;
					oXmlWriter.SaveToFile(pathFile.GetPath());

					OOX::CPath pathFileRels = pathFolderRels + FILE_SEPARATOR_STR + strMasterXml + _T(".rels");
					m_oReader.m_pRels->SaveRels(pathFileRels.GetPath());
				}
			}
	// slideMasters
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::SlideMasters);
			if (m_mainTables.end() != pPair)
			{
				OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slideMasters");
				OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

				NSDirectory::CreateDirectory(pathFolder.GetPath());
				NSDirectory::CreateDirectory(pathFolderRels.GetPath());

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);

				LONG __nCountLayouts = 0;

				for (LONG i = 0; i < nCountMasters; ++i)
				{
					PPTX::SlideMaster elm(&m_oDocument);
					m_arSlideMasters.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartMaster(i, m_arSlideMasters_Theme[i]);
					try
					{
						m_arSlideMasters[i].fromPPTY(&m_oReader);
					}
					catch (...)
					{
					}

					std::vector<PPTX::Logic::XmlId>& arrLays = m_arSlideMasters[i].sldLayoutIdLst;
					LONG lLayouts = (LONG)m_arSlideMasters_Theme[i].m_arLayouts.size();
					for (LONG j = 0; j < lLayouts; ++j)
					{
						arrLays.push_back(PPTX::Logic::XmlId(L"p:sldLayoutId"));

						std::wstring sId = std::to_wstring((_UINT64)(0x80000000 + __nCountLayouts + j + 1));

						arrLays[j].id = sId;
						arrLays[j].rid = (size_t)(j + 1);
					}
					__nCountLayouts += (LONG)(lLayouts + 1);

					m_oReader.m_pRels->CloseRels();

					std::wstring strMasterXml = L"slideMaster" + std::to_wstring(i + 1) + L".xml";
					oXmlWriter.ClearNoAttack();

					m_arSlideMasters[i].toXmlWriter(&oXmlWriter);

					OOX::CPath pathFile = pathFolder + FILE_SEPARATOR_STR + strMasterXml;
					oXmlWriter.SaveToFile(pathFile.GetPath());

					OOX::CPath pathFileRels = pathFolderRels + FILE_SEPARATOR_STR + strMasterXml + _T(".rels");
					m_oReader.m_pRels->SaveRels(pathFileRels.GetPath());
				}
			}
	// slideLayouts
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::SlideLayouts);
			if (m_mainTables.end() != pPair)
			{
				OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slideLayouts");
				OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

				NSDirectory::CreateDirectory(pathFolder.GetPath());
				NSDirectory::CreateDirectory(pathFolderRels.GetPath());

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);

				for (LONG i = 0; i < nCountLayouts; ++i)
				{
					PPTX::SlideLayout elm(&m_oDocument);
					m_arSlideLayouts.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartLayout(m_arSlideLayouts_Master[i]);

					try
					{
						m_arSlideLayouts[i].fromPPTY(&m_oReader);
					}
					catch (...)
					{
					}
					m_oReader.m_pRels->CloseRels();

					std::wstring strMasterXml = L"slideLayout" + std::to_wstring(i + 1) + L".xml";
					oXmlWriter.ClearNoAttack();

					m_arSlideLayouts[i].toXmlWriter(&oXmlWriter);

					OOX::CPath pathFile = pathFolder + FILE_SEPARATOR_STR + strMasterXml;
					oXmlWriter.SaveToFile(pathFile.GetPath());

					OOX::CPath pathFileRels = pathFolderRels + FILE_SEPARATOR_STR + strMasterXml + _T(".rels");
					m_oReader.m_pRels->SaveRels(pathFileRels.GetPath());
				}
			}
	// notes
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::NotesSlides);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				LONG lCount = m_oReader.GetLong();

				if (lCount > 0)
				{
					OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("notesSlides");
					OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

					NSDirectory::CreateDirectory(pathFolder.GetPath());
					NSDirectory::CreateDirectory(pathFolderRels.GetPath());

					for (LONG i = 0; i < lCount; ++i)
					{
						PPTX::NotesSlide elm(&m_oDocument);
						m_arNotesSlides.push_back(elm);

						m_oReader.m_pRels->Clear();

						size_t indexSlide = 0;
						for (indexSlide = 0; indexSlide < m_arSlides_Notes.size(); indexSlide++)
						{//todooo -> make map
							if (m_arSlides_Notes[indexSlide] == i)
							{
								break;
							}
						}
						m_oReader.m_pRels->StartNotes((int)indexSlide);

						try
						{
							m_arNotesSlides[i].fromPPTY(&m_oReader);
						}
						catch (...)
						{
						}

						m_oReader.m_pRels->CloseRels();

						std::wstring strNotesXml = L"notesSlide" + std::to_wstring(i + 1) + L".xml";
						oXmlWriter.ClearNoAttack();

						m_arNotesSlides[i].toXmlWriter(&oXmlWriter);

						OOX::CPath pathFile = pathFolder + FILE_SEPARATOR_STR + strNotesXml;
						oXmlWriter.SaveToFile(pathFile.GetPath());

						OOX::CPath pathFileRels = pathFolderRels + FILE_SEPARATOR_STR + strNotesXml + _T(".rels");
						m_oReader.m_pRels->SaveRels(pathFileRels.GetPath());
					}
				}
			}
	// noteMasters
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::NotesMasters);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				LONG lCount = m_oReader.GetLong();

				if (lCount > 0 || m_arNotesSlides.size() > 0)//один элемент
				{
					OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("notesMasters");
					OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

					NSDirectory::CreateDirectory(pathFolder.GetPath());
					NSDirectory::CreateDirectory(pathFolderRels.GetPath());

					PPTX::NotesMaster elm(&m_oDocument);
					m_arNotesMasters.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartThemeNotesMaster((int)m_arSlideMasters_Theme.size());

					bNotesMasterPresent = true;
					if (lCount > 0)
					{
						try
						{
							m_arNotesMasters.back().fromPPTY(&m_oReader);
						}
						catch (...)
						{
						}
						m_oReader.m_pRels->CloseRels();

						std::wstring strMasterNotesXml = L"notesMaster1.xml";
						oXmlWriter.ClearNoAttack();

						m_arNotesMasters.back().toXmlWriter(&oXmlWriter);

						OOX::CPath pathFile = pathFolder + FILE_SEPARATOR_STR + strMasterNotesXml;
						oXmlWriter.SaveToFile(pathFile.GetPath());

						OOX::CPath pathFileRels = pathFolderRels + FILE_SEPARATOR_STR + strMasterNotesXml + _T(".rels");
						m_oReader.m_pRels->SaveRels(pathFileRels.GetPath());
					}
					else
					{
						CreateDefaultNotesMasters((int)m_arSlideMasters_Theme.size());
					}
				}
			}
	// slides
			int nComment = 1;
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Slides);
			if (m_mainTables.end() != pPair)
			{
				OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slides");
				OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

				NSDirectory::CreateDirectory(pathFolder.GetPath());
				NSDirectory::CreateDirectory(pathFolderRels.GetPath());

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);

				for (LONG i = 0; i < nCountSlides; ++i)
				{
					PPTX::Slide elm(&m_oDocument);
					m_arSlides.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartSlide(i, m_arSlides_Layout[i], m_arSlides_Notes[i]);

					try
					{
						m_arSlides[i].fromPPTY(&m_oReader);
					}
					catch (...)
					{
					}

					if (m_arSlides[i].comments.is_init())
					{
						m_oReader.m_pRels->WriteSlideComments(nComment);
						OOX::CPath pathFolderCommentDir = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("comments");
						if (1 == nComment)
						{
							NSDirectory::CreateDirectory(pathFolderCommentDir.GetPath());
						}
						std::wstring strCommentFile = L"comment" + std::to_wstring(nComment) + L".xml";

						oXmlWriter.ClearNoAttack();
						m_arSlides[i].comments->toXmlWriter(&oXmlWriter);

						OOX::CPath pathComment = pathFolderCommentDir + FILE_SEPARATOR_STR + strCommentFile;
						oXmlWriter.SaveToFile(pathComment.GetPath());

						++nComment;
					}

					m_oReader.m_pRels->CloseRels();

					std::wstring strMasterXml = L"slide" + std::to_wstring(i + 1) + L".xml";
					oXmlWriter.ClearNoAttack();

					m_arSlides[i].toXmlWriter(&oXmlWriter);

					OOX::CPath pathFile = pathFolder + FILE_SEPARATOR_STR + strMasterXml;
					oXmlWriter.SaveToFile(pathFile.GetPath());

					OOX::CPath pathFileRels = pathFolderRels + FILE_SEPARATOR_STR + strMasterXml + _T(".rels");
					m_oReader.m_pRels->SaveRels(pathFileRels.GetPath());
				}
			}
	// app
			CreateDefaultApp();
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::App);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				try
				{
					m_oApp.fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
				SetRequiredDefaultsApp();
			}
	// core
			CreateDefaultCore();
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Core);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				try
				{
					m_oCore.fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
				SetRequiredDefaultsCore();
			}

			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::CustomProperties);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				try
				{
					m_oCustomProperties = new PPTX::CustomProperties(&m_oDocument);
					m_oCustomProperties->fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
			}
	// customs
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Customs);
			if (m_mainTables.end() != pPair)
			{
				OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("customXml");
				OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

				NSDirectory::CreateDirectory(pathFolder.GetPath());
				NSDirectory::CreateDirectory(pathFolderRels.GetPath());

				m_oReader.Seek(pPair->second);
				int nCountCustoms = m_oReader.GetLong();

				for (int i = 0; i < nCountCustoms; ++i)
				{
					smart_ptr<OOX::CCustomXML> pCustomXml = new OOX::CCustomXML(NULL, false);

					pCustomXml->fromPPTY(&m_oReader);

					//smart_ptr<OOX::File> pCustomXmlFile = pCustomXml.smart_dynamic_cast<OOX::File>();
					//m_oPresentation.Add(pCustomXmlFile);
					OOX::CPath filename(L"item" + std::to_wstring(i + 1) + L".xml");
					pCustomXml->write(pathFolder / filename, OOX::CPath(m_strDstFolder), *m_oImageManager.m_pContentTypes);
				}
				m_oReader.m_pRels->WriteCustoms(nCountCustoms);
			}

	// presProps
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::PresProps);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				try
				{
					m_oPresProps.fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
			}
			else
			{
				CreateDefaultPresProps();
			}
	// viewProps
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::ViewProps);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				try
				{
					m_oViewProps.fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
			}
			else
			{
				CreateDefaultViewProps();
			}
	// tableStyles
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::TableStyles);
			if (m_mainTables.end() != pPair)
			{
				m_oReader.Seek(pPair->second);
				try
				{
					m_oTableStyles.fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
			}
			if (m_oTableStyles.Styles.empty())
			{
				CreateDefaultTableStyles();
			}
//------------------------------------------------------------------------------------------
	// app
			oXmlWriter.ClearNoAttack();
			m_oApp.toXmlWriter(&oXmlWriter);

			OOX::CPath pathApp = m_strDstFolder + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml");
			oXmlWriter.SaveToFile(pathApp.GetPath());

	// core
			oXmlWriter.ClearNoAttack();
			m_oCore.toXmlWriter(&oXmlWriter);

			OOX::CPath pathCore = m_strDstFolder + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml");
			oXmlWriter.SaveToFile(pathCore.GetPath());

			// customProperies
			if (m_oCustomProperties.IsInit())
			{
				oXmlWriter.ClearNoAttack();
				m_oCustomProperties->toXmlWriter(&oXmlWriter);

				OOX::CPath pathCore = m_strDstFolder + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + OOX::FileTypes::CustomProperties.DefaultFileName().GetPath();
				oXmlWriter.SaveToFile(pathCore.GetPath());
			}

	// presProps
			oXmlWriter.ClearNoAttack();
			m_oPresProps.toXmlWriter(&oXmlWriter);

			OOX::CPath pathPresProps = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("presProps.xml");
			oXmlWriter.SaveToFile(pathPresProps.GetPath());

	// viewProps
			oXmlWriter.ClearNoAttack();
			m_oViewProps.toXmlWriter(&oXmlWriter);

			OOX::CPath pathViewProps = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("viewProps.xml");
			oXmlWriter.SaveToFile(pathViewProps.GetPath());

			m_oReader.m_pRels->Clear();
			m_oReader.m_pRels->StartRels();

	// tablestyles
			oXmlWriter.ClearNoAttack();
			m_oTableStyles.toXmlWriter(&oXmlWriter);

			OOX::CPath pathTableStyles = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("tableStyles.xml");
			oXmlWriter.SaveToFile(pathTableStyles.GetPath());

	// presentation
			bool bIsAuthors = false;
			pPair = m_mainTables.find(NSBinPptxRW::NSMainTables::Presentation);
			if (m_mainTables.end() != pPair)
			{
				OOX::CPath pathFolder = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt");
				OOX::CPath pathFolderRels = pathFolder + FILE_SEPARATOR_STR + _T("_rels");

				NSDirectory::CreateDirectory(pathFolderRels.GetPath());

				m_oReader.Seek(pPair->second);
				try
				{
					m_oPresentation.fromPPTY(&m_oReader);
				}
				catch (...)
				{
				}
				m_oPresentation.sldMasterIdLst.clear();
				LONG nCountLayouts = 0;
				for (LONG i = 0; i < nCountMasters; ++i)
				{
					m_oPresentation.sldMasterIdLst.push_back(PPTX::Logic::XmlId(L"p:sldMasterId"));

					std::wstring sId = std::to_wstring((_UINT64)(0x80000000 + (_UINT64)nCountLayouts));

					m_oPresentation.sldMasterIdLst[i].id = sId;
					m_oPresentation.sldMasterIdLst[i].rid = (size_t)(i + 1);
					nCountLayouts += (LONG)(m_arSlideMasters_Theme[i].m_arLayouts.size() + 1);
				}

				m_oReader.m_pRels->WriteMasters(nCountMasters);
				m_oReader.m_pRels->WriteThemes(nCountThemes);

				unsigned int nCurrentRels = m_oReader.m_pRels->m_lNextRelsID;

				m_oPresentation.sldIdLst.clear();
				for (LONG i = 0; i < nCountSlides; ++i)
				{
					m_oPresentation.sldIdLst.push_back(PPTX::Logic::XmlId(L"p:sldId"));

					std::wstring sId = std::to_wstring(256 + i);

					m_oPresentation.sldIdLst[i].id = sId;
					m_oPresentation.sldIdLst[i].rid = nCurrentRels++;
				}
				m_oReader.m_pRels->WriteSlides(nCountSlides);

				m_oPresentation.notesMasterIdLst.clear();
				if (bNotesMasterPresent)
				{
					m_oPresentation.notesMasterIdLst.push_back(PPTX::Logic::XmlId(L"p:notesMasterId"));

					m_oPresentation.notesMasterIdLst[0].rid = m_oReader.m_pRels->m_lNextRelsID;
					m_oReader.m_pRels->WriteNotesMaster();
				}
				if (m_oPresentation.comments.is_init())
				{
					m_oReader.m_pRels->WritePresentationComments(nComment);
					OOX::CPath pathFolderCommentDir = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("comments");
					if (1 == nComment)
					{
						NSDirectory::CreateDirectory(pathFolderCommentDir.GetPath());
					}
					std::wstring strCommentFile = L"comment" + std::to_wstring(nComment) + L".xml";

					oXmlWriter.ClearNoAttack();
					m_oPresentation.comments->toXmlWriter(&oXmlWriter);

					OOX::CPath pathComment = pathFolderCommentDir + FILE_SEPARATOR_STR + strCommentFile;
					oXmlWriter.SaveToFile(pathComment.GetPath());

					++nComment;
				}
				m_oReader.m_pRels->EndPresentationRels(m_oPresentation.commentAuthors.is_init(), m_oPresentation.m_pVbaProject.is_init(), m_oPresentation.m_pJsaProject.is_init());
				m_oReader.m_pRels->CloseRels();

				OOX::CPath pathPresentation = pathFolder / m_oPresentation.DefaultFileName();

				// todooo втащить все как сделано для Custom
				//m_oPresentation.write(pathPresentation, m_oPresentation.DefaultDirectory(), *m_oImageManager.m_pContentTypes);
				oXmlWriter.ClearNoAttack();
				m_oPresentation.toXmlWriter(&oXmlWriter);

				oXmlWriter.SaveToFile(pathPresentation.GetPath());

				OOX::CPath pathPresentationRels = pathFolderRels + FILE_SEPARATOR_STR + _T("presentation.xml.rels");
				m_oReader.m_pRels->SaveRels(pathPresentationRels.GetPath());

				if (m_oPresentation.commentAuthors.is_init())
				{
					oXmlWriter.ClearNoAttack();
					m_oPresentation.commentAuthors->toXmlWriter(&oXmlWriter);

					OOX::CPath pathCommentAuthors = pathFolder + FILE_SEPARATOR_STR + _T("commentAuthors.xml");
					oXmlWriter.SaveToFile(pathCommentAuthors.GetPath());

					bIsAuthors = true;
				}
			}
			if (!bIsNoBase64)
			{
				RELEASEARRAYOBJECTS(pDstBuffer);
			}

	// content types
			OOX::CContentTypes *pContentTypes = m_oImageManager.m_pContentTypes;

			pContentTypes->Registration(m_oPresentation.type().OverrideType(),
				m_oPresentation.type().DefaultDirectory(),
				m_oPresentation.type().DefaultFileName());

			if (m_oPresentation.m_pVbaProject.IsInit())
			{
				pContentTypes->Registration(m_oPresentation.m_pVbaProject->type().OverrideType(),
					m_oPresentation.type().DefaultDirectory() / m_oPresentation.m_pVbaProject->type().DefaultDirectory(),
					m_oPresentation.m_pVbaProject->type().DefaultFileName());
			}
			pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml", L"/ppt", L"presProps.xml");
			pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml", L"/ppt", L"viewProps.xml");
			pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml", L"/ppt", L"tableStyles.xml");


			pContentTypes->Registration(L"application/vnd.openxmlformats-package.core-properties+xml", L"/docProps", L"core.xml");
			pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.extended-properties+xml", L"/docProps", L"app.xml");
			if (m_oCustomProperties.IsInit())
			{
				pContentTypes->Registration(OOX::FileTypes::CustomProperties.OverrideType(), L"/docProps", OOX::FileTypes::CustomProperties.DefaultFileName().GetPath());
			}

	// themes
			for (size_t i = 0; i < m_arThemes.size(); ++i)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.theme+xml", L"/ppt/theme", L"theme" + std::to_wstring((int)i + 1) + L".xml");
			}
	// notes master
			if (!m_arNotesMasters.empty())
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml", L"/ppt/notesMasters", L"notesMaster1.xml");
			}

	// masters
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml", L"/ppt/slideMasters", L"slideMaster" + std::to_wstring(i + 1) + L".xml");
			}

	// layouts
			for (LONG i = 0; i < nCountLayouts; ++i)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml", L"/ppt/slideLayouts", L"slideLayout" + std::to_wstring((int)i + 1) + L".xml");
			}

	// slides
			for (size_t i = 0; i < m_arSlides.size(); ++i)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.slide+xml", L"/ppt/slides", L"slide" + std::to_wstring((int)i + 1) + L".xml");
			}

	// notes
			for (size_t i = 0; i < m_arNotesSlides.size(); ++i)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml", L"/ppt/notesSlides", L"notesSlide" + std::to_wstring((int)i + 1) + L".xml");
			}

	// slideComments
			for (int i = 1; i < nComment; ++i)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.comments+xml", L"/ppt/comments", L"comment" + std::to_wstring(i) + L".xml");
			}
	// comment authors
			if (bIsAuthors)
			{
				pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.presentationml.commentAuthors+xml", L"/ppt", L"commentAuthors.xml");
			}

			pContentTypes->Write(m_strDstFolder);


			std::wstring strRELS = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/>");
			if (m_oCustomProperties.IsInit())
			{
				strRELS += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/custom-properties\" Target=\"docProps/custom.xml\"/>";
			}
			strRELS += L"</Relationships>";

			OOX::CPath filePathRels = m_strDstFolder + FILE_SEPARATOR_STR + _T("_rels");
			NSDirectory::CreateDirectory(filePathRels.GetPath());

			filePathRels = filePathRels + FILE_SEPARATOR_STR + _T(".rels");

			NSFile::CFileBinary oFile;
			oFile.CreateFileW(filePathRels.GetPath());
			oFile.WriteStringUTF8(strRELS);
			oFile.CloseFile();
		}
		void CPPTXWriter::ReadMasterInfo(LONG nIndexMaster)
		{
			LONG _rec_start = m_oReader.GetPos();
			LONG _end_rec = _rec_start + m_oReader.GetRecordSize() + 4;

			_slideMasterInfo& oMaster = m_arSlideMasters_Theme[nIndexMaster];

			m_oReader.Skip(1); // start attributes

			while (true)
			{
				BYTE _at = m_oReader.GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						oMaster.m_lThemeIndex = m_oReader.GetLong();
						break;
					}
					case 1:
					{
						std::string strMaster64 = m_oReader.GetString2A();
						break;
					}
					default:
						break;
				}
			}

			LONG _lay_count = m_oReader.GetULong();
			for (LONG i = 0; i < _lay_count; ++i)
			{
				m_oReader.Skip(6); // type + len

				while (true)
				{
					BYTE _at = m_oReader.GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							oMaster.m_arLayouts.push_back(0);
							oMaster.m_arLayouts.back()= m_oReader.GetLong();
							break;
						}
						case 1:
						{
							std::string strLayout64 = m_oReader.GetString2A();
							break;
						}
						default:
							break;
					}
				}
			}

			m_oReader.Seek(_end_rec);
		}
		void CPPTXWriter::SetRequiredDefaultsApp()
		{
			m_oApp.AppVersion.reset(NULL);
			std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
			if (sApplication.empty())
				sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
			std::string s = VALUE2STR(INTVER);
			sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif
			m_oApp.Application = sApplication;
		}
		void CPPTXWriter::CreateDefaultApp()
		{
			m_oApp.TotalTime = 0;
			m_oApp.Words = 0;
			SetRequiredDefaultsApp();
			m_oApp.PresentationFormat = L"On-screen Show (4:3)";
			m_oApp.Paragraphs = 0;
			m_oApp.Slides = (int)m_arSlides.size();
			m_oApp.Notes = (int)m_arSlides.size();
			m_oApp.HiddenSlides = 0;
			m_oApp.MMClips = 2;
			m_oApp.ScaleCrop = false;

			int nCountThemes = (int)m_arSlideMasters.size();
			int nCountSlides = (int)m_arSlides.size();

			m_oApp.HeadingPairs.push_back(PPTX::Logic::HeadingVariant());
			m_oApp.HeadingPairs[0].m_type = _T("lpstr");
			m_oApp.HeadingPairs[0].m_strContent = _T("Theme");
			m_oApp.HeadingPairs.push_back(PPTX::Logic::HeadingVariant());
			m_oApp.HeadingPairs[1].m_type = _T("i4");
			m_oApp.HeadingPairs[1].m_iContent = nCountThemes;
			m_oApp.HeadingPairs.push_back(PPTX::Logic::HeadingVariant());
			m_oApp.HeadingPairs[2].m_type = _T("lpstr");
			m_oApp.HeadingPairs[2].m_strContent = _T("Slide Titles");
			m_oApp.HeadingPairs.push_back(PPTX::Logic::HeadingVariant());
			m_oApp.HeadingPairs[3].m_type = _T("i4");
			m_oApp.HeadingPairs[3].m_iContent = nCountSlides;

			for (int i = 0; i < nCountThemes; ++i)
			{
				std::wstring s = L"Theme " + std::to_wstring( i + 1);
				m_oApp.TitlesOfParts.push_back(PPTX::Logic::PartTitle());
				m_oApp.TitlesOfParts[i].m_title = s;
			}

			for (int i = 0; i < nCountSlides; ++i)
			{
				std::wstring s = L"Slide " + std::to_wstring( i + 1);
				m_oApp.TitlesOfParts.push_back(PPTX::Logic::PartTitle());
				m_oApp.TitlesOfParts[nCountThemes + i].m_title = s;
			}

			m_oApp.LinksUpToDate = false;
			m_oApp.SharedDoc = false;
			m_oApp.HyperlinksChanged = false;
		}
		void CPPTXWriter::SetRequiredDefaultsCore()
		{
			if (!m_oCore.creator.IsInit())
			{
				std::wstring sCreator = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvCreator);
				if (!sCreator.empty())
					m_oCore.creator = sCreator;
			}
			if (!m_oCore.created.IsInit())
			{
				std::wstring sCreated = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvCreated);
				if (!sCreated.empty())
					m_oCore.created = sCreated;
			}
			std::wstring sLastModifiedBy = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvLastModifiedBy);
			if (!sLastModifiedBy.empty())
				m_oCore.lastModifiedBy = sLastModifiedBy;
			std::wstring sModified = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvModified);
			if (!sModified.empty())
				m_oCore.modified = sModified;
		}
		void CPPTXWriter::CreateDefaultCore()
		{
//			m_oCore.creator  = _T("");
			m_oCore.lastModifiedBy = _T("");
		}
		void CPPTXWriter::CreateDefaultViewProps()
		{
			m_oViewProps.NormalViewPr = new PPTX::nsViewProps::NormalViewPr();
			m_oViewProps.NormalViewPr->restoredLeft.sz = 15620;
			m_oViewProps.NormalViewPr->restoredTop.sz = 94660;

			m_oViewProps.SlideViewPr = new PPTX::nsViewProps::SlideViewPr();
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.attrVarScale = true;
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sx.n = 104;
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sx.d = 100;
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sy.n = 104;
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sy.d = 100;
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Origin.x = -1236;
			m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Origin.y = -90;

			m_oViewProps.SlideViewPr->CSldViewPr.GuideLst.push_back(PPTX::nsViewProps::Guide());
			m_oViewProps.SlideViewPr->CSldViewPr.GuideLst[0].orient = _T("horz");
			m_oViewProps.SlideViewPr->CSldViewPr.GuideLst[0].pos = 2160;
			m_oViewProps.SlideViewPr->CSldViewPr.GuideLst.push_back(PPTX::nsViewProps::Guide());
			m_oViewProps.SlideViewPr->CSldViewPr.GuideLst[1].pos = 2880;

			m_oViewProps.NotesTextViewPr = new PPTX::nsViewProps::NotesTextViewPr();
			m_oViewProps.NotesTextViewPr->CViewPr.Origin.x = 0;
			m_oViewProps.NotesTextViewPr->CViewPr.Origin.y = 0;
			m_oViewProps.NotesTextViewPr->CViewPr.Scale.sx.n = 100;
			m_oViewProps.NotesTextViewPr->CViewPr.Scale.sx.d = 100;
			m_oViewProps.NotesTextViewPr->CViewPr.Scale.sy.n = 100;
			m_oViewProps.NotesTextViewPr->CViewPr.Scale.sy.d = 100;

			m_oViewProps.GridSpacing = new PPTX::nsViewProps::GridSpacing();
			m_oViewProps.GridSpacing->cx = 72008;
			m_oViewProps.GridSpacing->cy = 72008;
		}
		void CPPTXWriter::CreateDefaultTableStyles()
		{
			m_oTableStyles.def = _T("{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}");
		}
		void CPPTXWriter::CreateDefaultPresProps()
		{
			// nothing
		}
		void CPPTXWriter::CreateDefaultNotesMasters(int nIndexTheme)
		{
			std::wstring strThemeNotes = L"theme" + std::to_wstring( nIndexTheme ) + L".xml";

			OOX::CPath pathNotesTheme = m_oReader.m_strFolderThemes + FILE_SEPARATOR_STR + strThemeNotes;
			Writers::DefaultNotesThemeWriter notesTheme;
			notesTheme.Write(pathNotesTheme.GetPath());
/////////////////////
			OOX::CPath pathNotesMasters = m_strDstFolder + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("notesMasters");
			NSDirectory::CreateDirectory(pathNotesMasters.GetPath());

			OOX::CPath pathNotesMaster1 = pathNotesMasters / _T("notesMaster1.xml");
			Writers::DefaultNotesMasterWriter notesMaster;
			notesMaster.Write(pathNotesMaster1.GetPath());

			OOX::CPath pathNotesMasterRels = pathNotesMasters / _T("_rels");
			NSDirectory::CreateDirectory(pathNotesMasterRels.GetPath());

			std::wstring strThemeNotesNum = std::to_wstring(nIndexTheme);

			std::wstring strVal = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
								<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
								<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme") + strThemeNotesNum + _T(".xml\"/></Relationships>");
			NSFile::CFileBinary oFileRels;
			oFileRels.CreateFile(pathNotesMasterRels.GetPath() + FILE_SEPARATOR_STR + _T("notesMaster1.xml.rels"));
			oFileRels.WriteStringUTF8(strVal);
			oFileRels.CloseFile();

		}
		void CPPTXWriter::CreateDefaultNote()
		{
			PPTX::Logic::NvGrpSpPr& nvGrpSpPr = m_oDefaultNote.cSld.spTree.nvGrpSpPr;
			nvGrpSpPr.cNvPr.id = 1;
			nvGrpSpPr.cNvPr.name = _T("");

			PPTX::Logic::Xfrm* xfrm = new PPTX::Logic::Xfrm();
			xfrm->offX = 0;
			xfrm->offY = 0;
			xfrm->extX = 0;
			xfrm->extY = 0;
			xfrm->chOffX = 0;
			xfrm->chOffY = 0;
			xfrm->chExtX = 0;
			xfrm->chExtY = 0;

			m_oDefaultNote.cSld.spTree.grpSpPr.xfrm = xfrm;

			// shape comment !!! (TODO:)
			PPTX::Logic::Shape* pShape = new PPTX::Logic::Shape();
			pShape->nvSpPr.cNvPr.id = 100000;
			pShape->nvSpPr.cNvPr.name = _T("");

			pShape->nvSpPr.cNvSpPr.noGrp = true;
			pShape->nvSpPr.cNvSpPr.noChangeArrowheads = true;

			pShape->nvSpPr.nvPr.ph = new PPTX::Logic::Ph();
			pShape->nvSpPr.nvPr.ph->type = _T("body");
			pShape->nvSpPr.nvPr.ph->idx = _T("1");

			PPTX::Logic::TxBody* pTxBody = new PPTX::Logic::TxBody();
			pTxBody->Paragrs.push_back(PPTX::Logic::Paragraph());

			PPTX::Logic::Run* pTxRun = new PPTX::Logic::Run();
			pTxRun->rPr = new PPTX::Logic::RunProperties();
			pTxRun->rPr->smtClean = false;
			pTxRun->SetText(_T("")); // enter simple comment here

			pShape->txBody = pTxBody;
			if (pShape->txBody.IsInit())
			{
				if (!pShape->txBody->bodyPr.IsInit())
					pShape->txBody->bodyPr = new PPTX::Logic::BodyPr;
			}

			PPTX::Logic::RunElem elm;
			pTxBody->Paragrs[0].RunElems.push_back(elm);
			pTxBody->Paragrs[0].RunElems[0].InitRun(pTxRun);

			PPTX::Logic::SpTreeElem elm_tree;
			m_oDefaultNote.cSld.spTree.SpTreeElems.push_back(elm_tree);
			m_oDefaultNote.cSld.spTree.SpTreeElems[0].InitElem(pShape);

			m_oDefaultNote.clrMapOvr = new PPTX::Logic::ClrMapOvr();
		}
}
