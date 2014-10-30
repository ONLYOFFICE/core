#pragma once
#include "Converter.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"


typedef void (*load_from_resource)(void*, int, CString&);

namespace NSBinPptxRW
{
	class CPPTXWriter
	{
	private:
		CBinaryFileReader	m_oReader;
		CImageManager2		m_oImageManager;
		CString				m_strDstFolder;

		std::map<BYTE, LONG> m_mainTables;

		std::vector<PPTX::Theme>			m_arThemes;
		
		std::vector<PPTX::SlideMaster>		m_arSlideMasters;
		std::vector<CSlideMasterInfo>		m_arSlideMasters_Theme;
		
		std::vector<PPTX::SlideLayout>		m_arSlideLayouts;
		std::vector<LONG>					m_arSlideLayouts_Master;

		std::vector<PPTX::Slide>			m_arSlides;
		std::vector<LONG>					m_arSlides_Layout;

		std::vector<PPTX::NotesMaster>		m_arNotesMasters;
		std::vector<PPTX::NotesSlide>		m_arNotesSlides;
		std::vector<LONG>					m_arNotesSlides_Master;
		
		PPTX::Presentation				m_oPresentation;
		PPTX::TableStyles				m_oTableStyles;
		PPTX::VmlDrawing				m_oVmlDrawing;
		PPTX::App						m_oApp;
		PPTX::Core						m_oCore;
		PPTX::ViewProps					m_oViewProps;
		PPTX::PresProps					m_oPresProps;

		BOOL							m_bIsDefaultNoteMaster;
		PPTX::NotesSlide				m_oDefaultNote;

		load_from_resource m_fCallback;
		void* m_pCallbackArg;
	public:

		CPPTXWriter(load_from_resource fCallback, void* pCallbackArg)
		{
			m_strDstFolder = _T("");
			m_bIsDefaultNoteMaster = TRUE;
			m_fCallback = fCallback;
			m_pCallbackArg = pCallbackArg;
		}
		~CPPTXWriter()
		{
		}

		void Init(CString strFolder)
		{
			m_strDstFolder = strFolder;
            CString strPPT = m_strDstFolder + _T("\\ppt");


            FileSystem::Directory::CreateDirectory(string2std_string(m_strDstFolder));
            FileSystem::Directory::CreateDirectory(string2std_string(m_strDstFolder), _T("docProps"));
            FileSystem::Directory::CreateDirectory(strPPT);
            FileSystem::Directory::CreateDirectory(string2std_string(strPPT), _T("media"));

			m_oImageManager.Clear();
			m_oImageManager.SetDstMedia(m_strDstFolder + _T("\\ppt\\media"));

			m_oReader.m_pRels->m_pManager = &m_oImageManager;
		}

		void OpenPPTY(BYTE* pBuffer, int len, CString srcFolder, CString strThemesFolder)
		{
			int start_pos = 0;

			int cur_pos = start_pos;
			while (cur_pos < len && pBuffer[cur_pos] != (BYTE)(';'))
				++cur_pos;

			if (cur_pos == len || cur_pos == start_pos)
				return;

            CStringA __str_ppty((LPSTR)(pBuffer + start_pos), cur_pos - start_pos);
			start_pos = cur_pos + 1;

			cur_pos = start_pos;
			while (cur_pos < len && pBuffer[cur_pos] != (BYTE)(';'))
				++cur_pos;

			if (cur_pos == len || cur_pos == start_pos)
				return;

            CStringA __str_version((LPSTR)(pBuffer + start_pos), cur_pos - start_pos);
			start_pos = cur_pos + 1;

			cur_pos = start_pos;
			while (cur_pos < len && pBuffer[cur_pos] != (BYTE)(';'))
				++cur_pos;

			if (cur_pos == len || cur_pos == start_pos)
				return;

            CStringA __str_decode_len_ansi((LPSTR)(pBuffer + start_pos), cur_pos - start_pos);
			CString  __str_decode_len = __str_decode_len_ansi;
			start_pos = cur_pos + 1;

			pBuffer += start_pos;
			len -= start_pos;
			int dstLenTemp = XmlUtils::GetInteger(__str_decode_len);
			//int dstLenTemp = Base64DecodeGetRequiredLength(len);

			BYTE* pDstBuffer = new BYTE[dstLenTemp];
			int dstLen = dstLenTemp;
            Base64::Base64Decode((LPCSTR)pBuffer, len, pDstBuffer, &dstLen);

			m_oReader.m_strContentTypes = _T("");
			m_oReader.Init(pDstBuffer, 0, dstLen);
			m_oReader.m_strFolder = srcFolder;
			m_oReader.m_strFolderThemes = strThemesFolder;
			
			for (LONG i = 0; i < 30/*main tables max*/; ++i)
			{
				BYTE _type = m_oReader.GetUChar();
				if (0 == _type)
					break;

				m_mainTables.insert(std::pair<BYTE, LONG>(_type, m_oReader.GetLong()));				
			}

			std::map<BYTE, LONG>::iterator pPair;

			// writer
			CXmlWriter oXmlWriter;

			// первым делом определим количество необходимого. если хоть одно из этих чисел - ноль, то ппту не корректный
			LONG nCountThemes = 0;
			LONG nCountMasters = 0;
			LONG nCountLayouts = 0;
			LONG nCountSlides = 0;

			pPair = m_mainTables.find(NSMainTables::Themes);
			if (m_mainTables.end()  != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountThemes = m_oReader.GetLong();
			}
			pPair = m_mainTables.find(NSMainTables::SlideMasters);
			if (m_mainTables.end()  != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountMasters = m_oReader.GetLong();
			}
			pPair = m_mainTables.find(NSMainTables::SlideLayouts);
			if (m_mainTables.end()  != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountLayouts = m_oReader.GetLong();
			}
			pPair = m_mainTables.find(NSMainTables::Slides);
			if (m_mainTables.end()  != pPair)
			{
				m_oReader.Seek(pPair->second);
				nCountSlides = m_oReader.GetLong();
			}

			if (0 == nCountThemes || 0 == nCountMasters || 0 == nCountLayouts || 0 == nCountSlides)
			{
				return;
			}

			// теперь создадим массивы для рельсов
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				CSlideMasterInfo elm;
				m_arSlideMasters_Theme.push_back(elm);
			}
			for (LONG i = 0; i < nCountLayouts; ++i)
			{
				m_arSlideLayouts_Master.push_back(0);
			}
			for (LONG i = 0; i < nCountSlides; ++i)
				m_arSlides_Layout.push_back(0);

			// ThemeRels
			pPair = m_mainTables.find(NSMainTables::ThemeRels);
			if (m_mainTables.end()  != pPair)
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
					m_arSlideLayouts_Master[m_arSlideMasters_Theme[i].m_arLayouts[j]] = (LONG)i;
				}
			}

			// готово, теперь нужно слайдам проставить шаблоны
			pPair = m_mainTables.find(NSMainTables::SlideRels);
			if (m_mainTables.end()  != pPair)
			{
				m_oReader.Seek(pPair->second);
				m_oReader.Skip(6); // type + len + start attr

				while (true)
				{
					BYTE _at = m_oReader.GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					m_arSlides_Layout[_at] = m_oReader.GetULong();
				}
			}

			// теперь нужно удалить все themes, которые не ведут на мастерслайд
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
				arThemesSave[m_arSlideMasters_Theme[i].m_lThemeIndex] = true;
			}
			LONG lCurrectTheme = 0;
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				if (!arThemesSave[i])
					continue;
				arThemesDst[i] = lCurrectTheme;
				++lCurrectTheme;
			}
			// теперь нужно перебить ссылки
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				m_arSlideMasters_Theme[i].m_lThemeIndex = arThemesDst[i];
			}
			
			// themes
			pPair = m_mainTables.find(NSMainTables::Themes);
			if (m_mainTables.end()  != pPair)
			{
				CString strFolder = m_strDstFolder + _T("\\ppt\\theme");
				CString strFolderRels = strFolder + _T("\\_rels");

                FileSystem::Directory::CreateDirectory(strFolder);
                FileSystem::Directory::CreateDirectory(strFolderRels);

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);
								
				for (LONG i = 0; i < nCountThemes; ++i)
				{
					if (!arThemesSave[i])
					{
						// это ненужная тема
						continue;
					}

					PPTX::Theme elm;
					m_arThemes.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartTheme();
					m_arThemes[i].fromPPTY(&m_oReader);

					CString strMasterXml = _T("");
					strMasterXml.Format(_T("\\theme%d.xml"), i + 1);
					oXmlWriter.ClearNoAttack();

					m_oReader.m_pRels->CloseRels();

					m_arThemes[i].toXmlWriter(&oXmlWriter);
					oXmlWriter.SaveToFile(strFolder + strMasterXml);
					m_oReader.m_pRels->SaveRels(strFolderRels + strMasterXml + _T(".rels"));
				}
			}

			// slideMasters
			pPair = m_mainTables.find(NSMainTables::SlideMasters);
			if (m_mainTables.end()  != pPair)
			{
				CString strFolder = m_strDstFolder + _T("\\ppt\\slideMasters");
				CString strFolderRels = strFolder + _T("\\_rels");

                FileSystem::Directory::CreateDirectory(strFolder);
                FileSystem::Directory::CreateDirectory(strFolderRels);

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);

				LONG __nCountLayouts = 0;
				
				for (LONG i = 0; i < nCountMasters; ++i)
				{
					PPTX::SlideMaster elm;
					m_arSlideMasters.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartMaster(i, m_arSlideMasters_Theme[i]);
					m_arSlideMasters[i].fromPPTY(&m_oReader);
					
					std::vector<PPTX::Logic::XmlId>& arrLays = m_arSlideMasters[i].sldLayoutIdLst;
					LONG lLayouts = (LONG)m_arSlideMasters_Theme[i].m_arLayouts.size();
					for (LONG j = 0; j < lLayouts; ++j)
					{
						arrLays.push_back(PPTX::Logic::XmlId());
						
						CString sId = _T("");
						sId.Format(_T("%u"), 0x80000000 + __nCountLayouts + j + 1);

						arrLays[j].m_name = _T("sldLayoutId");
						arrLays[j].id = sId;
						arrLays[j].rid = (size_t)(j + 1);
					}
					__nCountLayouts += (LONG)(lLayouts + 1);

					m_oReader.m_pRels->CloseRels();

					CString strMasterXml = _T("");
					strMasterXml.Format(_T("\\slideMaster%d.xml"), i + 1);
					oXmlWriter.ClearNoAttack();

					m_arSlideMasters[i].toXmlWriter(&oXmlWriter);

					oXmlWriter.SaveToFile(strFolder + strMasterXml);
					m_oReader.m_pRels->SaveRels(strFolderRels + strMasterXml + _T(".rels"));
				}
			}

			// slideLayouts
			pPair = m_mainTables.find(NSMainTables::SlideLayouts);
			if (m_mainTables.end()  != pPair)
			{
				CString strFolder = m_strDstFolder + _T("\\ppt\\slideLayouts");
				CString strFolderRels = strFolder + _T("\\_rels");

                FileSystem::Directory::CreateDirectory(strFolder);
                FileSystem::Directory::CreateDirectory(strFolderRels);

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);
				
				for (LONG i = 0; i < nCountLayouts; ++i)
				{
					PPTX::SlideLayout elm;
					m_arSlideLayouts.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartLayout(m_arSlideLayouts_Master[i]);
					m_arSlideLayouts[i].fromPPTY(&m_oReader);
					m_oReader.m_pRels->CloseRels();

					CString strMasterXml = _T("");
					strMasterXml.Format(_T("\\slideLayout%d.xml"), i + 1);
					oXmlWriter.ClearNoAttack();

					m_arSlideLayouts[i].toXmlWriter(&oXmlWriter);

					oXmlWriter.SaveToFile(strFolder + strMasterXml);
					m_oReader.m_pRels->SaveRels(strFolderRels + strMasterXml + _T(".rels"));	
				}
			}

			// slides
			int nComment = 1;
			pPair = m_mainTables.find(NSMainTables::Slides);
			if (m_mainTables.end()  != pPair)
			{
				CString strFolder = m_strDstFolder + _T("\\ppt\\slides");
				CString strFolderRels = strFolder + _T("\\_rels");

               FileSystem::Directory::CreateDirectory (strFolder);
               FileSystem::Directory::CreateDirectory (strFolderRels);

				m_oReader.Seek(pPair->second);
				m_oReader.Skip(4);
				
				for (LONG i = 0; i < nCountSlides; ++i)
				{
					PPTX::Slide elm;
					m_arSlides.push_back(elm);

					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartSlide(i, m_arSlides_Layout[i]);
					m_arSlides[i].fromPPTY(&m_oReader);

					if (m_arSlides[i].comments.is_init())
					{
						m_oReader.m_pRels->WriteSlideComments(nComment);
						if (1 == nComment)
						{
                            FileSystem::Directory::CreateDirectory (m_strDstFolder + _T("\\ppt\\comments"));
						}
						CString strCommentFile = _T("");
						strCommentFile.Format(_T("\\ppt\\comments\\comment%d.xml"), nComment);

						oXmlWriter.ClearNoAttack();
						m_arSlides[i].comments->toXmlWriter(&oXmlWriter);
						oXmlWriter.SaveToFile(m_strDstFolder + strCommentFile);

						++nComment;
					}

					m_oReader.m_pRels->CloseRels();

					CString strMasterXml = _T("");
					strMasterXml.Format(_T("\\slide%d.xml"), i + 1);
					oXmlWriter.ClearNoAttack();

					m_arSlides[i].toXmlWriter(&oXmlWriter);

					oXmlWriter.SaveToFile(strFolder + strMasterXml);
					m_oReader.m_pRels->SaveRels(strFolderRels + strMasterXml + _T(".rels"));	
				}
			}

			if (FALSE)
			{
				// noteMasters
				pPair = m_mainTables.find(NSMainTables::NotesMasters);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					LONG lCount = m_oReader.GetLong();
					
					for (LONG i = 0; i < lCount; ++i)
					{
						PPTX::NotesMaster elm;
						m_arNotesMasters.push_back(elm);
						m_arNotesMasters[i].fromPPTY(&m_oReader);
					}
				}

				// notes
				pPair = m_mainTables.find(NSMainTables::NotesSlides);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					LONG lCount = m_oReader.GetLong();
					
					for (LONG i = 0; i < lCount; ++i)
					{
						PPTX::NotesSlide elm;
						m_arNotesSlides.push_back(elm);
						m_arNotesSlides[i].fromPPTY(&m_oReader);
					}
				}
			}
			else
			{
				// create default
				CreateDefaultNotesMasters((int)m_arThemes.size() + 1);
				CreateDefaultNote();

				CString strFolder = m_strDstFolder + _T("\\ppt\\notesSlides");
				CString strFolderRels = strFolder + _T("\\_rels");

                FileSystem::Directory::CreateDirectory (strFolder);
                FileSystem::Directory::CreateDirectory (strFolderRels);

				LONG lCount = (LONG)m_arSlides.size();				
				for (LONG i = 0; i < lCount; ++i)
				{
					m_oReader.m_pRels->Clear();
					m_oReader.m_pRels->StartNote(i);
					m_oReader.m_pRels->CloseRels();

					CString strMasterXml = _T("");
					strMasterXml.Format(_T("\\notesSlide%d.xml"), i + 1);
					oXmlWriter.ClearNoAttack();

					m_oDefaultNote.toXmlWriter(&oXmlWriter);

					oXmlWriter.SaveToFile(strFolder + strMasterXml);
					m_oReader.m_pRels->SaveRels(strFolderRels + strMasterXml + _T(".rels"));	
				}
			}

			if (FALSE)
			{
				// app
				pPair = m_mainTables.find(NSMainTables::App);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					m_oApp.fromPPTY(&m_oReader);
				}

				// core
				pPair = m_mainTables.find(NSMainTables::Core);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					m_oCore.fromPPTY(&m_oReader);
				}

				// tableStyles
				pPair = m_mainTables.find(NSMainTables::TableStyles);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					m_oTableStyles.fromPPTY(&m_oReader);
				}

				// viewProps
				pPair = m_mainTables.find(NSMainTables::ViewProps);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					m_oViewProps.fromPPTY(&m_oReader);
				}

				CreateDefaultPresProps();
			}
			else
			{
				// create default
				CreateDefaultApp();
				CreateDefaultCore();
				CreateDefaultPresProps();
				//CreateDefaultTableStyles();
				CreateDefaultViewProps();

				pPair = m_mainTables.find(NSMainTables::TableStyles);
				if (m_mainTables.end()  != pPair)
				{
					m_oReader.Seek(pPair->second);
					m_oTableStyles.fromPPTY(&m_oReader);
				}

				if (m_oTableStyles.Styles.empty())
				{
					CreateDefaultTableStyles();
				}
			}

			// app 
			oXmlWriter.ClearNoAttack();
			m_oApp.toXmlWriter(&oXmlWriter);
			oXmlWriter.SaveToFile(m_strDstFolder + _T("\\docProps\\app.xml"));

			// core
			oXmlWriter.ClearNoAttack();
			m_oCore.toXmlWriter(&oXmlWriter);
			oXmlWriter.SaveToFile(m_strDstFolder + _T("\\docProps\\core.xml"));

			// presProps
			oXmlWriter.ClearNoAttack();
			m_oPresProps.toXmlWriter(&oXmlWriter);
			oXmlWriter.SaveToFile(m_strDstFolder + _T("\\ppt\\presProps.xml"));

			// viewProps
			oXmlWriter.ClearNoAttack();
			m_oViewProps.toXmlWriter(&oXmlWriter);
			oXmlWriter.SaveToFile(m_strDstFolder + _T("\\ppt\\viewProps.xml"));

			m_oReader.m_pRels->Clear();
			m_oReader.m_pRels->StartRels();

			// tablestyles
			oXmlWriter.ClearNoAttack();
			m_oTableStyles.toXmlWriter(&oXmlWriter);
			oXmlWriter.SaveToFile(m_strDstFolder + _T("\\ppt\\tableStyles.xml"));
			
			// presentation
			bool bIsAuthors = false;
			pPair = m_mainTables.find(NSMainTables::Presentation);
			if (m_mainTables.end()  != pPair)
			{
				CString strFolder = m_strDstFolder + _T("\\ppt");
				CString strFolderRels = strFolder + _T("\\_rels");

                FileSystem::Directory::CreateDirectory (strFolderRels);

				m_oReader.Seek(pPair->second);
				m_oPresentation.fromPPTY(&m_oReader);

				m_oPresentation.sldMasterIdLst.clear();
				LONG nCountLayouts = 0;
				for (LONG i = 0; i < nCountMasters; ++i)
				{
					m_oPresentation.sldMasterIdLst.push_back(PPTX::Logic::XmlId());

					CString sId = _T("");
					sId.Format(_T("%u"), 0x80000000 + nCountLayouts);
					m_oPresentation.sldMasterIdLst[i].m_name = _T("sldMasterId");
					m_oPresentation.sldMasterIdLst[i].id = sId;
					m_oPresentation.sldMasterIdLst[i].rid = (size_t)(i + 1);
					nCountLayouts += (LONG)(m_arSlideMasters_Theme[i].m_arLayouts.size() + 1);
				}

				m_oReader.m_pRels->WriteMasters(nCountMasters);
				m_oReader.m_pRels->WriteThemes(nCountThemes);

				int nCurrentRels = m_oReader.m_pRels->GetNextId();

				m_oPresentation.sldIdLst.clear();
				for (LONG i = 0; i < nCountSlides; ++i)
				{
					m_oPresentation.sldIdLst.push_back(PPTX::Logic::XmlId());

					CString sId = _T("");
					sId.Format(_T("%u"), 256 + i);
					m_oPresentation.sldIdLst[i].m_name = _T("sldId");
					m_oPresentation.sldIdLst[i].id = sId;
					m_oPresentation.sldIdLst[i].rid = (size_t)nCurrentRels;
					++nCurrentRels;
				}

				m_oReader.m_pRels->WriteSlides(nCountSlides);
				m_oReader.m_pRels->EndPresentationRels(m_oPresentation.commentAuthors.is_init());

				m_oPresentation.notesMasterIdLst.clear();
				m_oPresentation.notesMasterIdLst.push_back(PPTX::Logic::XmlId());
				m_oPresentation.notesMasterIdLst[0].m_name = _T("notesMasterId");
				m_oPresentation.notesMasterIdLst[0].rid = (size_t)nCurrentRels;

				m_oReader.m_pRels->CloseRels();

				oXmlWriter.ClearNoAttack();
				m_oPresentation.toXmlWriter(&oXmlWriter);

				oXmlWriter.SaveToFile(strFolder + _T("\\presentation.xml"));
				m_oReader.m_pRels->SaveRels(strFolderRels + _T("\\presentation.xml.rels"));	

				if (m_oPresentation.commentAuthors.is_init())
				{
					oXmlWriter.ClearNoAttack();
					m_oPresentation.commentAuthors->toXmlWriter(&oXmlWriter);
					
					oXmlWriter.SaveToFile(strFolder + _T("\\commentAuthors.xml"));
					bIsAuthors = true;
				}
			}

			RELEASEARRAYOBJECTS(pDstBuffer);

			// content types
			CStringWriter oContentTypes;
			oContentTypes.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\
<Default Extension=\"png\" ContentType=\"image/png\" />\
<Default Extension=\"jpeg\" ContentType=\"image/jpeg\" />\
<Default Extension=\"wmf\" ContentType=\"image/x-wmf\" />\
<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\" />\
<Default Extension=\"xml\" ContentType=\"application/xml\" />\
<Default Extension=\"gif\" ContentType=\"image/gif\"/>\
<Default Extension=\"emf\" ContentType=\"image/x-emf\"/>\
<Default Extension=\"jpg\" ContentType=\"image/jpeg\"/>\
\
<Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" />\
<Override PartName=\"/ppt/presProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" />\
<Override PartName=\"/ppt/viewProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml\" />\
<Override PartName=\"/ppt/tableStyles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml\"/>\
<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\" />\
<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\" />"));

			// themes
			for (LONG i = 0; i < (LONG)m_arThemes.size(); ++i)
			{
				CString strTheme = _T("");
				strTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>"), i + 1);
				oContentTypes.WriteString(strTheme);
			}
			if (TRUE)
			{
				// notes theme
				CString strTheme = _T("");
				strTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>"), m_arThemes.size() + 1);
				oContentTypes.WriteString(strTheme);

				oContentTypes.WriteString(_T("<Override PartName=\"/ppt/notesMasters/notesMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml\"/>"));
			}

			// masters
			for (LONG i = 0; i < nCountMasters; ++i)
			{
				CString strMaster = _T("");
				strMaster.Format(_T("<Override PartName=\"/ppt/slideMasters/slideMaster%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\" />"), i + 1);
				oContentTypes.WriteString(strMaster);
			}

			// layouts
			for (LONG i = 0; i < nCountLayouts; ++i)
			{
				CString strL = _T("");
				strL.Format(_T("<Override PartName=\"/ppt/slideLayouts/slideLayout%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\" />"), i + 1);
				oContentTypes.WriteString(strL);
			}

			// slides
			for (LONG i = 0; i < nCountSlides; ++i)
			{
				CString strS = _T("");
				strS.Format(_T("<Override PartName=\"/ppt/slides/slide%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slide+xml\" />"), i + 1);
				oContentTypes.WriteString(strS);
			}

			// notes
			for (LONG i = 0; i < nCountSlides; ++i)
			{
				CString strN = _T("");
				strN.Format(_T("<Override PartName=\"/ppt/notesSlides/notesSlide%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml\"/>"), i + 1);
				oContentTypes.WriteString(strN);
			}

			// slideComments
			for (int i = 1; i < nComment; ++i)
			{
				CString strN = _T("");
				strN.Format(_T("<Override PartName=\"/ppt/comments/comment%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.comments+xml\"/>"), i);
				oContentTypes.WriteString(strN);				
			}
			// comment authors
			if (bIsAuthors)
			{
				oContentTypes.WriteString(_T("<Override PartName=\"/ppt/commentAuthors.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.commentAuthors+xml\"/>"));
			}

			oContentTypes.WriteString(m_oReader.m_strContentTypes);

			oContentTypes.WriteString(_T("</Types>"));

			CFile oFile;
            oFile.CreateFile(m_strDstFolder + _T("/[Content_Types].xml"));
			CString strContentTypes = oContentTypes.GetData();
			oFile.WriteStringUTF8(strContentTypes);
			oFile.CloseFile();


			CString strRELS = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/>\
</Relationships>");

            FileSystem::Directory::CreateDirectory (m_strDstFolder + _T("\\_rels"));
			oFile.CreateFile(m_strDstFolder + _T("\\_rels\\.rels"));
			oFile.WriteStringUTF8(strRELS);
			oFile.CloseFile();

		}

	private:

		void ReadMasterInfo(LONG nIndexMaster)
		{
			LONG _rec_start = m_oReader.GetPos();
			LONG _end_rec = _rec_start + m_oReader.GetLong() + 4;

			CSlideMasterInfo& oMaster = m_arSlideMasters_Theme[nIndexMaster];			
			
			m_oReader.Skip(1); // start attributes

			while (true)
			{
				BYTE _at = m_oReader.GetUChar();
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
						CStringA strMaster64 = m_oReader.GetString2A();
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
					BYTE _at = m_oReader.GetUChar();
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
							CStringA strLayout64 = m_oReader.GetString2A();
							break;
						}
						default:
							break;
					}
				}
			}

			m_oReader.Seek(_end_rec);
		}

		void CreateDefaultApp()
		{
			m_oApp.TotalTime = 0;
			m_oApp.Words = 0;
			m_oApp.Application = _T("OnlyOffice");
			m_oApp.PresentationFormat = _T("On-screen Show (4:3)");
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
				CString s = _T("");
				s.Format(_T("Theme %d"), i + 1);
				m_oApp.TitlesOfParts.push_back(PPTX::Logic::PartTitle());
				m_oApp.TitlesOfParts[i].m_title = s;
			}

			for (int i = 0; i < nCountSlides; ++i)
			{
				CString s = _T("");
				s.Format(_T("Slide %d"), i + 1);
				m_oApp.TitlesOfParts.push_back(PPTX::Logic::PartTitle());
				m_oApp.TitlesOfParts[nCountThemes + i].m_title = s;
			}
			
			m_oApp.LinksUpToDate = false;
			m_oApp.SharedDoc = false;
			m_oApp.HyperlinksChanged = false;
			m_oApp.AppVersion = _T("3.0000");			
		}
		void CreateDefaultCore()
		{
			m_oCore.creator  = _T("");
			m_oCore.lastModifiedBy = _T("");
		}
		void CreateDefaultViewProps()
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
		void CreateDefaultTableStyles()
		{
			m_oTableStyles.def = _T("{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}");
		}
		void CreateDefaultPresProps()
		{
			// nothing
		}
		void CreateDefaultNotesMasters(int nIndexTheme)
		{
			m_fCallback(m_pCallbackArg, nIndexTheme, m_strDstFolder);
		}
		void CreateDefaultNote()
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
			m_oDefaultNote.cSld.spTree.m_name = _T("p:spTree");
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

			PPTX::Logic::RunElem elm;
			pTxBody->Paragrs[0].RunElems.push_back(elm);
			pTxBody->Paragrs[0].RunElems[0].InitRun(pTxRun);

			PPTX::Logic::SpTreeElem elm_tree;
			m_oDefaultNote.cSld.spTree.SpTreeElems.push_back(elm_tree);
			m_oDefaultNote.cSld.spTree.SpTreeElems[0].InitElem(pShape);

			m_oDefaultNote.clrMapOvr = new PPTX::Logic::ClrMapOvr();
		}
	};
}
