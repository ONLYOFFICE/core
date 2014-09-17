#pragma once
#include "../stdafx.h"
#include "../PPTXFormat/PPTX.h"

#include "BinWriters.h"

namespace PPTX2EditorAdvanced
{
	using namespace NSBinPptxRW;

	DWORD Convert(NSBinPptxRW::CBinaryFileWriter& oBinaryWriter, PPTX::Folder& oFolder, const CString& strSourceDirectory, const CString& strDstFile)
	{	
		// сначала соберем все объекты для конвертации и сформируем main-таблицы
		NSBinPptxRW::CCommonWriter* pCommon = &oBinaryWriter.m_oCommon;

		CAtlArray<smart_ptr<PPTX::Theme>>			_themes;
		CAtlArray<smart_ptr<PPTX::SlideMaster>>		_slideMasters;
		CAtlArray<smart_ptr<PPTX::Slide>>			_slides;
		CAtlArray<smart_ptr<PPTX::SlideLayout>>		_layouts;
		CAtlArray<smart_ptr<PPTX::NotesSlide>>		_notes;
		CAtlArray<smart_ptr<PPTX::NotesMaster>>		_notesMasters;

		smart_ptr<PPTX::Presentation> presentation = oFolder.get(PPTX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
		int cx = presentation->sldSz->cx;
		int cy = presentation->sldSz->cy;

		// записываем все темы
		size_t nCountMasters = presentation->sldMasterIdLst.GetCount();
		for (size_t nMaster = 0; nMaster < nCountMasters; ++nMaster)
		{
			smart_ptr<PPTX::SlideMaster> slideMaster = ((*presentation)[presentation->sldMasterIdLst[nMaster].rid.get()]).smart_dynamic_cast<PPTX::SlideMaster>();
			size_t pPointerSM = (size_t)(slideMaster.operator ->()); 

			CAtlMap<size_t, LONG>::CPair* pSearchSM = pCommon->slideMasters.Lookup(pPointerSM);
			if (NULL != pSearchSM)
			{
				// такого быть не должно
				continue;
			}

			oBinaryWriter.m_oCommon.m_oRels.Add();
			CMasterSlideInfo& oMasterInfo = oBinaryWriter.m_oCommon.m_oRels[oBinaryWriter.m_oCommon.m_oRels.GetCount() - 1];

			// записываем mainMaster
			LONG lCountSM = (LONG)_slideMasters.GetCount();
			pCommon->slideMasters.SetAt(pPointerSM, lCountSM);
			_slideMasters.Add(slideMaster);

			// проверяем theme
			size_t pPointerTh = (size_t)(slideMaster->Theme.operator ->()); 
			CAtlMap<size_t, LONG>::CPair* pSearchTh = pCommon->themes.Lookup(pPointerTh);
			if (NULL == pSearchTh)
			{
				LONG lCountTh = (LONG)_themes.GetCount();
				pCommon->themes.SetAt(pPointerTh, lCountTh);
				_themes.Add(slideMaster->Theme);
				oMasterInfo.m_lThemeIndex = lCountTh;
			}
			else
			{
				oMasterInfo.m_lThemeIndex = pSearchTh->m_value;
			}

			size_t nCountLayouts = slideMaster->sldLayoutIdLst.GetCount();
			for (size_t iLayout = 0; iLayout < nCountLayouts; ++iLayout)
			{
				smart_ptr<PPTX::SlideLayout> slideLayout = ((*slideMaster)[slideMaster->sldLayoutIdLst[iLayout].rid.get()]).smart_dynamic_cast<PPTX::SlideLayout>();

				// проверяем layout
				size_t pPointerL = (size_t)(slideLayout.operator ->()); 
				CAtlMap<size_t, LONG>::CPair* pSearchL = pCommon->layouts.Lookup(pPointerL);
				if (NULL == pSearchL)
				{
					LONG lCountL = (LONG)_layouts.GetCount();
					pCommon->layouts.SetAt(pPointerL, lCountL);
					_layouts.Add(slideLayout);

					oMasterInfo.m_arLayoutIndexes.Add(lCountL);
					oMasterInfo.m_arLayoutImagesBase64.Add("");
				}
				else
				{
					oMasterInfo.m_arLayoutIndexes.Add(pSearchL->m_value);
					oMasterInfo.m_arLayoutImagesBase64.Add("");
				}
			}
		}

		// записываем все notesMasters
		size_t nCountNoteMasters = presentation->notesMasterIdLst.GetCount();
		for (size_t nNote = 0; nNote < nCountNoteMasters; ++nNote)
		{
			smart_ptr<PPTX::NotesMaster> noteMaster = ((*presentation)[presentation->notesMasterIdLst[nNote].rid.get()]).smart_dynamic_cast<PPTX::NotesMaster>();
			size_t pPointerNM = (size_t)(noteMaster.operator ->()); 

			CAtlMap<size_t, LONG>::CPair* pSearchNM = pCommon->notesMasters.Lookup(pPointerNM);
			if (NULL != pSearchNM)
			{
				// такого быть не должно
				continue;
			}

			// записываем mainMaster
			LONG lCountNM = (LONG)_notesMasters.GetCount();
			pCommon->notesMasters.SetAt(pPointerNM, lCountNM);
			_notesMasters.Add(noteMaster);

			// проверяем theme
			size_t pPointerTh = (size_t)(noteMaster->Theme.operator ->()); 
			CAtlMap<size_t, LONG>::CPair* pSearchTh = pCommon->themes.Lookup(pPointerTh);
			if (NULL == pSearchTh)
			{
				LONG lCountTh = (LONG)_themes.GetCount();
				pCommon->themes.SetAt(pPointerTh, lCountTh);
				_themes.Add(noteMaster->Theme);
			}
		}

		// записываем все слайды
		size_t nCount = presentation->sldIdLst.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			smart_ptr<PPTX::Slide> slide = ((*presentation)[presentation->sldIdLst[i].rid.get()]).smart_dynamic_cast<PPTX::Slide>();
			
			size_t pPointerS = (size_t)(slide.operator ->()); 

			CAtlMap<size_t, LONG>::CPair* pSearchS = pCommon->slides.Lookup(pPointerS);
			if (NULL != pSearchS)
			{
				// такого быть не должно
				continue;
			}

			size_t pPointerL = (size_t)(slide->Layout.operator ->()); 
			CAtlMap<size_t, LONG>::CPair* pSearchL = pCommon->layouts.Lookup(pPointerL);
			if (NULL == pSearchL)
			{
				// такого быть не должно
				oBinaryWriter.m_oCommon.m_oSlide_Layout_Rels.Add(0);				
			}
			else
			{
				oBinaryWriter.m_oCommon.m_oSlide_Layout_Rels.Add(pSearchL->m_value);
			}

			LONG lCountS = (LONG)_slides.GetCount();
			pCommon->slides.SetAt(pPointerS, lCountS);
			_slides.Add(slide);

			// проверяем note
			size_t pPointerN = (size_t)(slide->Note.operator ->()); 

			if (NULL != pPointerN)
			{
				CAtlMap<size_t, LONG>::CPair* pSearchN = pCommon->notes.Lookup(pPointerN);
				if (NULL == pSearchN)
				{
					LONG lCountN = (LONG)_notes.GetCount();
					pCommon->notes.SetAt(pPointerN, lCountN);
					_notes.Add(slide->Note);
				}
			}
		}

		// нужно записать все в maintables. А кроме главных таблиц ничего и нету. Все остальное лежит в них
		// на каждую таблицу - 5 байт (тип и сдвиг)
		// число таблиц - заранее известно (сделаем 30. если потом не будет хватать - новая версия формата)
		oBinaryWriter.WriteReserved(5 * 30);

		// Main
		oBinaryWriter.StartMainRecord(NSMainTables::Main);
		oBinaryWriter.WriteULONG(NSSerFormat::Signature);
		oBinaryWriter.WriteULONG(0);
		
		// App
		smart_ptr<PPTX::App> app = oFolder.get(PPTX::FileTypes::App).smart_dynamic_cast<PPTX::App>();
		if (app.is_init())
		{
			oBinaryWriter.StartMainRecord(NSMainTables::App);
			app->toPPTY(&oBinaryWriter);
		}

		// Core
		smart_ptr<PPTX::Core> core = oFolder.get(PPTX::FileTypes::Core).smart_dynamic_cast<PPTX::Core>();
		if (core.is_init())
		{
			oBinaryWriter.StartMainRecord(NSMainTables::Core);
			core->toPPTY(&oBinaryWriter);
		}

		// ViewProps
		smart_ptr<PPTX::ViewProps> viewProps = presentation->get(PPTX::FileTypes::ViewProps).smart_dynamic_cast<PPTX::ViewProps>();
		if (viewProps.is_init())
		{
			oBinaryWriter.StartMainRecord(NSMainTables::ViewProps);
			viewProps->toPPTY(&oBinaryWriter);
		}

		// TableStyles
		smart_ptr<PPTX::TableStyles> tablestyles = presentation->get(PPTX::FileTypes::TableStyles).smart_dynamic_cast<PPTX::TableStyles>();
		if (tablestyles.is_init())
		{
			oBinaryWriter.StartMainRecord(NSMainTables::TableStyles);
			tablestyles->toPPTY(&oBinaryWriter);
		}

		// Presentation
		oBinaryWriter.StartMainRecord(NSMainTables::Presentation);
		presentation->toPPTY(&oBinaryWriter);

		// themes
		oBinaryWriter.StartMainRecord(NSMainTables::Themes);
		ULONG nCountThemes = (ULONG)_themes.GetCount();
		oBinaryWriter.WriteULONG(nCountThemes);
		for (ULONG i = 0; i < nCountThemes; ++i)
		{
			_themes[i]->toPPTY(&oBinaryWriter);
		}

		// slidemasters
		oBinaryWriter.StartMainRecord(NSMainTables::SlideMasters);
		ULONG nCountSM = (ULONG)_slideMasters.GetCount();
		oBinaryWriter.WriteULONG(nCountSM);
		for (ULONG i = 0; i < nCountSM; ++i)
		{
			_slideMasters[i]->toPPTY(&oBinaryWriter);
		}

		// slidelayouts
		oBinaryWriter.StartMainRecord(NSMainTables::SlideLayouts);
		ULONG nCountL = (ULONG)_layouts.GetCount();
		oBinaryWriter.WriteULONG(nCountL);
		for (ULONG i = 0; i < nCountL; ++i)
		{
			_layouts[i]->toPPTY(&oBinaryWriter);
		}

		// slides
		oBinaryWriter.StartMainRecord(NSMainTables::Slides);
		ULONG nCountS = (ULONG)_slides.GetCount();
		oBinaryWriter.WriteULONG(nCountS);
		for (ULONG i = 0; i < nCountS; ++i)
		{
			_slides[i]->toPPTY(&oBinaryWriter);
		}

		if (FALSE)
		{
			// ПОКА нету NOTES

			// notes
			oBinaryWriter.StartMainRecord(NSMainTables::NotesSlides);
			ULONG nCountN = (ULONG)_notes.GetCount();
			oBinaryWriter.WriteULONG(nCountN);
			for (ULONG i = 0; i < nCountN; ++i)
			{
				_notes[i]->toPPTY(&oBinaryWriter);
			}

			// notesmasters
			oBinaryWriter.StartMainRecord(NSMainTables::NotesMasters);
			ULONG nCountNM = (ULONG)_notesMasters.GetCount();
			oBinaryWriter.WriteULONG(nCountNM);
			for (ULONG i = 0; i < nCountNM; ++i)
			{
				_notesMasters[i]->toPPTY(&oBinaryWriter);
			}
		}

		// ImageMap ---------------------------------------
		oBinaryWriter.StartMainRecord(NSMainTables::ImageMap);
		oBinaryWriter.StartRecord(NSMainTables::ImageMap);
		oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		CAtlMap<CString, NSShapeImageGen::CImageInfo>* pIMaps = &oBinaryWriter.m_oCommon.m_oImageManager.m_mapImagesFile;
		POSITION pos1 = pIMaps->GetStartPosition();

		LONG lIndexI = 0;
		while (NULL != pos1)
		{
			NSShapeImageGen::CImageInfo& oRec = pIMaps->GetNextValue(pos1);
			oBinaryWriter.WriteString1(lIndexI++, oRec.GetPath2());
		}

		oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		oBinaryWriter.EndRecord();

		// ------------------------------------------------

		// FontMap ----------------------------------------
		oBinaryWriter.StartMainRecord(NSMainTables::FontMap);
		oBinaryWriter.StartRecord(NSMainTables::FontMap);
		oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		CAtlMap<CString, CString>* pFMaps = &oBinaryWriter.m_oCommon.m_pNativePicker->m_mapPicks;
		POSITION pos2 = pFMaps->GetStartPosition();

		LONG lIndexF = 0;
		while (NULL != pos2)
		{
			CString& oRec = pFMaps->GetNextValue(pos2);
			oBinaryWriter.WriteString1(lIndexF++, oRec);
		}

		oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		oBinaryWriter.EndRecord();

		// ------------------------------------------------

		if (TRUE)
		{
			// SlideRels --------------------------------------
			oBinaryWriter.StartMainRecord(NSMainTables::SlideRels);
			oBinaryWriter.StartRecord(NSMainTables::SlideRels);
			oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			size_t _s_rels = oBinaryWriter.m_oCommon.m_oSlide_Layout_Rels.GetCount();
			for (size_t i = 0; i < _s_rels; ++i)
			{
				oBinaryWriter.WriteInt1(0, oBinaryWriter.m_oCommon.m_oSlide_Layout_Rels[i]);
			}
			
			oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			oBinaryWriter.EndRecord();

			// ------------------------------------------------

			// ThemeRels --------------------------------------
			oBinaryWriter.StartMainRecord(NSMainTables::ThemeRels);
			oBinaryWriter.StartRecord(NSMainTables::ThemeRels);
			
			CAtlArray<NSBinPptxRW::CMasterSlideInfo>* th_rels = &oBinaryWriter.m_oCommon.m_oRels;
			oBinaryWriter.WriteULONG((ULONG)th_rels->GetCount());

			for (size_t i = 0; i < th_rels->GetCount(); i++)
			{
				NSBinPptxRW::CMasterSlideInfo& oTh = th_rels->GetAt(i);

				oBinaryWriter.StartRecord(0);

				oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				oBinaryWriter.WriteInt1(0, oTh.m_lThemeIndex);
				//oBinaryWriter.WriteBYTE(1);
				//oBinaryWriter.WriteStringA(oTh.m_strImageBase64);
				oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				
				ULONG lay_count = (ULONG)oTh.m_arLayoutIndexes.GetCount();
				oBinaryWriter.WriteULONG(lay_count);

				for (ULONG j = 0; j < lay_count; ++j)
				{
					oBinaryWriter.StartRecord(0);

					oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					oBinaryWriter.WriteInt1(0, oTh.m_arLayoutIndexes[j]);
					//oBinaryWriter.WriteBYTE(1);
					//oBinaryWriter.WriteStringA(oTh.m_arLayoutImagesBase64[j]);
					oBinaryWriter.WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
					
					oBinaryWriter.EndRecord();
				}

				oBinaryWriter.EndRecord();
			}		
			
			oBinaryWriter.EndRecord();
		}

		// ------------------------------------------------

		oBinaryWriter.WriteEmbeddedFonts();
		oBinaryWriter.WriteMainPart();

		// все записалось нормально. осталось скинуть на диск
		BYTE* pbBinBuffer = oBinaryWriter.GetBuffer();
		int nBinBufferLen = (int)oBinaryWriter.GetPosition();
		int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
		BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen];
		if (TRUE == Base64::Base64Encode(pbBinBuffer, nBinBufferLen, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
		{
			CFile oFile;
			oFile.CreateFileW(strDstFile);

			CString strPrefix = _T("");
			strPrefix.Format(_T("PPTY;v1;%d;"), nBinBufferLen);
			CStringA sW = (CStringA)strPrefix;
			oFile.WriteFile(sW.GetBuffer(), (DWORD)sW.GetLength());
			oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
			oFile.CloseFile();
		}

		RELEASEARRAYOBJECTS(pbBase64Buffer);
		return 0;
	}
}
