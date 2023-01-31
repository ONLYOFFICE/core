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
#include "../../../../Common/MS-LCID.h"

#include "GlobalsSubstream.h"
#include "AnyObject.h"

#include "Biff_unions/INTERFACE.h"
#include "Biff_unions/PROTECTION.h"
#include "Biff_unions/FNGROUPS.h"
#include "Biff_unions/FORMATTING.h"
#include "Biff_unions/PIVOTCACHEDEFINITION.h"
#include "Biff_unions/DOCROUTE.h"
#include "Biff_unions/BUNDLESHEET.h"
#include "Biff_unions/METADATA.h"
#include "Biff_unions/SUPBOOK.h"
#include "Biff_unions/LBL.h"
#include "Biff_unions/RTD.h"
#include "Biff_unions/MSODRAWINGGROUP.h"
#include "Biff_unions/SHAREDSTRINGS.h"
#include "Biff_unions/THEME.h"
#include "Biff_unions/STYLES.h"
#include "Biff_unions/XFS.h"

#include "Biff_records/BOF.h"
#include "Biff_records/WriteProtect.h"
#include "Biff_records/FilePass.h"
#include "Biff_records/Template.h"
#include "Biff_records/WriteAccess.h"
#include "Biff_records/FileSharing.h"
#include "Biff_records/CodePage.h"
#include "Biff_records/Lel.h"
#include "Biff_records/DSF.h"
#include "Biff_records/Excel9File.h"
#include "Biff_records/RRTabId.h"
#include "Biff_records/ObProj.h"
#include "Biff_records/ObNoMacros.h"
#include "Biff_records/CodeName.h"
#include "Biff_records/Lbl.h"
#include "Biff_records/OleObjectSize.h"
#include "Biff_records/Window1.h"
#include "Biff_records/Backup.h"
#include "Biff_records/HideObj.h"
#include "Biff_records/Date1904.h"
#include "Biff_records/CalcPrecision.h"
#include "Biff_records/RefreshAll.h"
#include "Biff_records/BookBool.h"
#include "Biff_records/UserBView.h"
#include "Biff_records/UsesELFs.h"
#include "Biff_records/MTRSettings.h"
#include "Biff_records/ForceFullCalculation.h"
#include "Biff_records/Country.h"
#include "Biff_records/RecalcId.h"
#include "Biff_records/HFPicture.h"
#include "Biff_records/ExtSST.h"
#include "Biff_records/WebPub.h"
#include "Biff_records/WOpt.h"
#include "Biff_records/CrErr.h"
#include "Biff_records/BookExt.h"
#include "Biff_records/FeatHdr.h"
#include "Biff_records/DConn.h"
#include "Biff_records/CompressPictures.h"
#include "Biff_records/Compat12.h"
#include "Biff_records/GUIDTypeLib.h"
#include "Biff_records/EOF.h"
#include "Biff_records/BOF.h"
#include "Biff_records/MDTInfo.h"
#include "Biff_records/ExternSheet.h"
#include "Biff_records/XFExt.h"
#include "Biff_records/XFCRC.h"
#include "Biff_records/DXF.h"
#include "Biff_records/SupBook.h"
#include "Biff_records/NameCmt.h"
#include "Biff_records/SupBook.h"
#include "Biff_records/ExternName.h"
#include "Biff_records/ExternSheet.h"
#include "Biff_records/Continue.h"
//#include "Biff_records/XCT.h"
//#include "Biff_records/CRN.h"

#include "Biff_structures/ODRAW/OfficeArtDgContainer.h"

namespace XLS
{;

static const int aCodePages[][2] = {
    //charset	codepage
    0,	1252, //ANSI
    1,	0,//Default
    2,	42,//Symbol
    77,	10000,//Mac Roman
    78,	10001,//Mac Shift Jis
    79,	10003,//Mac Hangul
    80,	10008,//Mac GB2312
    81,	10002,//Mac Big5
    83,	10005,//Mac Hebrew
    84,	10004,//Mac Arabic
    85,	10006,//Mac Greek
    86,	10081,//Mac Turkish
    87,	10021,//Mac Thai
    88,	10029,//Mac East Europe
    89,	10007,//Mac Russian
    128,	932,//Shift JIS
    129,	949,//Hangul
    130,	1361,//Johab
    134,	936,//GB2312
    136,	950,//Big5
    238,	1250,//Greek
    161,	1253,//Greek
    162,	1254,//Turkish
    163,	1258,//Vietnamese
    177,	1255,//Hebrew
    178,	1256, //Arabic
    186,	1257,//Baltic
    204,	1251,//Russian
    222,	874,//Thai
    238,	1250,//Eastern European
    254,	437,//PC 437
    255,	850//OEM
};

GlobalsSubstream::GlobalsSubstream(const unsigned short code_page) :	code_page_(code_page)
{
}

GlobalsSubstream::~GlobalsSubstream()
{
}


BaseObjectPtr GlobalsSubstream::clone()
{
	return BaseObjectPtr(new GlobalsSubstream(*this));
}

const bool GlobalsSubstream::loadContent(BinProcessor& proc)
{
	int	count = 0;

	global_info_ = proc.getGlobalWorkbookInfo();
	
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;
		if (type == rt_EOF) 
		{
			proc.mandatory<EOF_T>();
			break;
		}

		switch(type)
		{
			case rt_BOF_BIFF8:
			case rt_BOF_BIFF4:
			case rt_BOF_BIFF3:
			case rt_BOF_BIFF2:
			{
				if (proc.optional<BOF>())
				{
					BOF *bof = dynamic_cast<BOF*>(elements_.back().get());
					global_info_->Version = bof->vers;
					if (global_info_->Version < 0x0600)
					{				

						global_info_->CodePage = 0; //???
					}
				}
			}break;
			case rt_WriteProtect:	
			{
				if (proc.optional<WriteProtect>())
				{
					m_WriteProtect = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_FilePass:		
			{
				if (proc.optional<FilePass>())
				{					
					if (( global_info_->decryptor)  &&
						( global_info_->decryptor->IsVerify() == false))
					{
						if (!global_info_->decryptor->SetPassword(L"VelvetSweatshop"))
							return false;
						else
							global_info_->bWorkbookProtectExist = true;
					}
				}
			}break;
			case rt_Template:
			{
				if (proc.optional<Template>())
				{
					m_Template = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_InterfaceHdr:		proc.optional<INTERFACE_T>();	break;
			case rt_WriteAccess:		proc.optional<WriteAccess>();	break;
			case rt_Lel:				proc.repeated<Lel>(0, 2047);	break;
			case rt_DSF:				proc.optional<DSF>();			break; //resered record
			case rt_Excel9File:			proc.optional<Excel9File>();	break;
			case rt_RRTabId:
			{
				if (proc.optional<RRTabId>())
				{
					m_RRTabId = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_ObProj:				
			{
				if (proc.optional<ObProj>())	
				{
					global_info_->bVbaProjectExist	= true;
					global_info_->bMacrosExist		= true;
				}
			}break;
			case rt_ObNoMacros:	
			{
				if (proc.optional<ObNoMacros>())
				{
					global_info_->bMacrosExist = false; //empty VbaProject
				}
			}break;
			case rt_CodeName:
			{
				if (proc.optional<CodeName>())
				{
					m_CodeName = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_BuiltInFnGroupCount:
			{
				if (proc.optional<FNGROUPS>())
				{
					m_FNGROUPS = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_OleObjectSize:		proc.optional<OleObjectSize>();	break;
			case rt_WinProtect:			
			{
				if (proc.optional<PROTECTION>())
				{
					global_info_->bWorkbookProtectExist = true;
					
					m_PROTECTION = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_FileSharing:		proc.optional<FileSharing>();	break;
			case rt_CodePage:
			{
				if (proc.optional<CodePage>())
				{
					m_CodePage = elements_.back();
					elements_.pop_back();

					CodePage *CodePage_ = dynamic_cast<CodePage*>(m_CodePage.get());

					if ((CodePage_) && (CodePage_->cv != 0/* && CodePage_->cv != 1200*/))
						code_page_ = CodePage_->cv;

					global_info_->CodePage = code_page_;
				}
			}break;
			case rt_Window1:
			{
				count = proc.repeated<Window1>(0, 0);
				while(count > 0)
				{
					m_arWindow1.insert(m_arWindow1.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_Backup:			proc.optional<Backup>();		break;
			case rt_HideObj:		proc.optional<HideObj>();		break;
			case rt_ExternSheet:	proc.optional<ExternSheet>();	break;
			case rt_CalcPrecision:
			{
				if (proc.optional<CalcPrecision>())
				{
					m_CalcPrecision = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_RefreshAll:		proc.optional<RefreshAll>();	break;
			case rt_BookBool:		proc.optional<BookBool>();		break;
			case rt_Date1904:
			{
				if (proc.optional<Date1904>())
				{
					m_Date1904 = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_Country:	
			{
				if (proc.optional<Country>())
				{
					m_Country = elements_.back();
					elements_.pop_back();

					Country *Country_ = dynamic_cast<Country*>(m_Country.get());
					if (Country_)
					{
						int countryDef = Country_->iCountryDef;
						int countryWinIni = Country_->iCountryWinIni;

						global_info_->CodePage;
					}
				}	
			}break;
			case rt_UsesELFs:		proc.optional<UsesELFs>();	break;
			case rt_RecalcId:		proc.optional<RecalcId>();	break;
			case rt_Palette:
			case rt_Font:	
			case rt_Format:
			case rt_XFCRC:
			{
				if (proc.mandatory<FORMATTING>())
				{					
					FORMATTING* fmts_new = dynamic_cast<FORMATTING*>(elements_.back().get());
				
					if (m_Formating ) //Zakaz_detalizatcii(08_fevralia_2014-21_fevralia_2014).XLS
					{//check previus
						FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());
						XFS *xfs = fmts ? dynamic_cast<XFS*>(fmts->m_XFS.get()) : NULL;
						
						if (!xfs || (xfs->m_arCellXFs.empty()))
						{
							if (fmts->m_Palette && !fmts_new->m_Palette) // АИС_Налог_Сводная информация по ЗУ_ver2212r555.xls
								fmts_new->m_Palette = fmts->m_Palette;

							m_Formating = NULL;
						}
						else
						{
							if (!fmts->m_Palette && fmts_new->m_Palette)
								fmts->m_Palette = fmts_new->m_Palette;
							
							if (!fmts->m_TABLESTYLES && fmts_new->m_TABLESTYLES)
								fmts->m_TABLESTYLES = fmts_new->m_TABLESTYLES;
							
							if (!fmts->m_THEME && fmts_new->m_THEME)
								fmts->m_THEME = fmts_new->m_THEME;
						}
					}
					if (!m_Formating )
					{
						m_Formating = elements_.back();
						elements_.pop_back();
						FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());

						if (fmts)
						{	
							global_info_->cellStyleDxfs_count = fmts->m_arDXF.size(); 
						}
					}
				}		
			}break;
			case rt_SXStreamID:			
			{
				count = proc.repeated<PIVOTCACHEDEFINITION>(0, 0);	
				while(count > 0)
				{
					global_info_->arPIVOTCACHEDEFINITION.insert(global_info_->arPIVOTCACHEDEFINITION.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_DocRoute:
			{
				count = proc.repeated<DOCROUTE>(0, 0);
			}break;
			case rt_UserBView:
			{
				count = proc.repeated<UserBView>(0, 0);
				while(count > 0)
				{
					m_arUserBView.insert(m_arUserBView.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_BoundSheet8:			
			{
				count = proc.repeated<BUNDLESHEET>(1, 0);	
				while(count > 0)
				{
					m_arBUNDLESHEET.insert(m_arBUNDLESHEET.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_MDTInfo:
			{
				if (proc.optional<METADATA>())
				{
					m_METADATA = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_MTRSettings:
			{
				if (proc.optional<MTRSettings>())
				{
					m_MTRSettings = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_ForceFullCalculation:	proc.optional<ForceFullCalculation>();	break;
			case rt_SupBook:
			{
				count = proc.repeated<SUPBOOK>(0, 0);
				while(count > 0)
				{
					m_arSUPBOOK.insert(m_arSUPBOOK.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_RealTimeData:			proc.repeated<RTD>(0, 0);				break;
			case rt_HFPicture:
			{
				count = proc.repeated<HFPicture>(0, 0);
				while(count > 0)
				{
					m_arHFPicture.insert(m_arHFPicture.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_MsoDrawingGroup:
			{
				MSODRAWINGGROUP mso_drawiing_group(false);
				count = proc.repeated(mso_drawiing_group, 0, 0);
				while(count > 0)
				{
					m_arMSODRAWINGGROUP.insert(m_arMSODRAWINGGROUP.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}			
			}break;
			case rt_Lbl_BIFF34:
			case rt_Lbl:
			{
				count = proc.repeated<LBL>(0, 0);
				int start_pos = m_arLBL.size();
				while(count > 0)
				{
					m_arLBL.insert(m_arLBL.begin() + start_pos, elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_SST:
			{
				SHAREDSTRINGS	shared_strings(code_page_);
				if (proc.optional(shared_strings))
				{
					m_SHAREDSTRINGS = elements_.back();
					elements_.pop_back();

					global_info_->startAddedSharedStrings = shared_strings.size_;
				}
			}break;
			case rt_ExtSST:
			{
				if (proc.optional<ExtSST>())
				{
					m_ExtSST = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_WebPub:				proc.repeated<WebPub>(0, 0);			break;
			case rt_WOpt:				proc.repeated<WOpt>(0, 0);				break;
			case rt_CrErr:				proc.optional<CrErr>();					break;
			case rt_BookExt:
			{
				if (proc.optional<BookExt>())
				{
					m_BookExt = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_FeatHdr:
			{
				FeatHdr feat_hdr(true);
				proc.repeated(feat_hdr, 0, 0);
			}break;
			case rt_DConn:
			{
				count = proc.repeated<DConn>(0, 0);
				while(count > 0)
				{
					DConn *conn = dynamic_cast<DConn*>(elements_.back().get());
					if (conn)
					{
						if (conn->id.bType == 1)
						{
							global_info_->mapStrConnection.insert(std::make_pair(conn->id.string.strTotal, elements_.back()));
						}
						else if (conn->id.bType == 2)
						{
							global_info_->mapIdConnection.insert(std::make_pair(conn->id.sxStreamID.idStm, elements_.back()));
						}
					}
					m_arDConn.insert(m_arDConn.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_Theme:
			{
				if (proc.optional<THEME>())
				{
					m_THEME = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_CompressPictures:	proc.optional<CompressPictures>();		break;
			case rt_Compat12:			proc.optional<Compat12>();				break;
			case rt_GUIDTypeLib:		proc.optional<GUIDTypeLib>();			break;

			//case rt_XFCRC:
			//{//не по спецификации !!! Calculadora.xls
			//	if(proc.optional<XFCRC>())
			//	{
			//		elements_.pop_back(); // Crc не нужен

			//		count = proc.repeated<XFExt>(0, 0);//(16, 4050);
			//		while (count > 0)
			//		{
			//			//перенести в FORMATING/XFS ?? - тогда нужен пересчет там !!
			//			//if (elements_.empty()) break;
			//			//XFExt* ext = dynamic_cast<XFExt*>(elements_.back().get());
			//			//m_arXFext.push_back(elements_.front());
			//			elements_.pop_front();
			//			count--;
			//		}
			//	}
			//	count = proc.repeated<DXF>(0, 0);
			//	while(count > 0)
			//	{
			//		//m_arDXF.insert(m_arDXF.begin(), elements_.back());
			//		elements_.pop_back();
			//		count--;
			//	}
			//	if (proc.optional<STYLES>())
			//	{
			//		//m_Styles = elements_.back();
			//		elements_.pop_back();
			//	}	
			//}break;
			default://skip					
			{
				proc.SkipRecord();	
			}break;
		}
	}

	if (global_info_->CodePage == 0)
	{	//try from charsets ... todooo make local set on each record (aka Label)
		//from ixfe -> ifnt from xf -> arFonts
		for (std::map<int, int>::iterator it = global_info_->fonts_charsets.begin()
			; global_info_->CodePage == 0 && it != global_info_->fonts_charsets.end()
			; ++it)
		{
			for (int i = 0 ; i < sizeof(aCodePages) / 2; i++)
			{
				if (aCodePages[i][0] == it->first)
				{
					global_info_->CodePage = aCodePages[i][1];
					break;
				}
			}
		}
	}
	if (global_info_->CodePage == 0 && global_info_->lcid_user > 0)
	{
		global_info_->CodePage = msLCID2DefCodePage(global_info_->lcid_user);
	}
	
	UpdateXFC();
	LoadHFPicture();	
	UpdateXti();
	UpdateDefineNames();
	UpdateExternalDefineNames();

	return true;
}
void GlobalsSubstream::UpdateXFC()
{
	FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());

	if (fmts)
	{		
		fmts->update_xfs();
	}
}
void GlobalsSubstream::LoadHFPicture()
{
	if (m_arHFPicture.empty()) return;

	size_t current_size_hf = 0, j = 0;
	for ( size_t i = 0; i < m_arHFPicture.size(); i++)
	{
		HFPicture* hf = dynamic_cast<HFPicture*>(m_arHFPicture[i].get());
		if ((hf) && (hf->recordDrawingGroup))
		{
			if (!hf->fContinue && current_size_hf > 0)
			{
				XLS::CFRecord record(CFRecordType::ANY_TYPE, global_info_);
				for (; j < i; j++)
				{
					hf = dynamic_cast<HFPicture*>(m_arHFPicture[j].get());
					record.appendRawData(hf->recordDrawingGroup);
				}
				ODRAW::OfficeArtDgContainerPtr rgDrawing = ODRAW::OfficeArtDgContainerPtr(new ODRAW::OfficeArtDgContainer(ODRAW::OfficeArtRecord::CA_HF));
				rgDrawing->loadFields(record);
				m_arHFPictureDrawing.push_back(rgDrawing);
				current_size_hf = 0;

			}
			current_size_hf += hf->recordDrawingGroup->getDataSize();
		}
	}
	if (current_size_hf > 0)
	{
		XLS::CFRecord record(ODRAW::OfficeArtRecord::DggContainer, global_info_);
		for (; j < m_arHFPicture.size(); j++)
		{
			HFPicture* hf = dynamic_cast<HFPicture*>(m_arHFPicture[j].get());
			record.appendRawData(hf->recordDrawingGroup);
		}
		ODRAW::OfficeArtDgContainerPtr rgDrawing = ODRAW::OfficeArtDgContainerPtr(new ODRAW::OfficeArtDgContainer(ODRAW::OfficeArtRecord::CA_HF));
		rgDrawing->loadFields(record);
		m_arHFPictureDrawing.push_back(rgDrawing);
	}
}

void GlobalsSubstream::UpdateXti()
{
	for (size_t s = 0; s < m_arSUPBOOK.size(); s++)
	{
		SUPBOOK* SUPP = dynamic_cast<SUPBOOK*>(m_arSUPBOOK[s].get());
		if (!SUPP) continue;

		ExternSheet* extern_sheet = dynamic_cast<ExternSheet*>(SUPP->m_ExternSheet.get());

		GlobalWorkbookInfo::_xti val;

		val.iSup	= s;
		val.pNames	= &SUPP->arNames;

		for (size_t i = 0; extern_sheet && i < extern_sheet->rgXTI.size(); i++)
		{
			XTI* xti = dynamic_cast<XTI*>(extern_sheet->rgXTI[i].get());
			if (!xti) continue;

			SUPBOOK* index_book = dynamic_cast<SUPBOOK*>(m_arSUPBOOK[xti->iSupBook].get());
			if (!index_book) continue;

			SupBook *info = dynamic_cast<SupBook*>(index_book->m_SupBook.get());
			if (!info) continue;

			GlobalWorkbookInfo::_xti val_1;
		
			val_1.iSup		= xti->iSupBook;
			val_1.pNames	= &index_book->arNames;

			if( index_book->nExternIndex > 0 )
			{
				val_1.link = L"[" + std::to_wstring(index_book->nExternIndex) + L"]";
			}	
			if (xti->itabFirst >= 0 /*|| itabLast >= 0*/)
			{
				if (info->rgst.empty() && index_book->nExternIndex < 0)
				{
					std::wstring strRange;
					if(-1 == xti->itabFirst)
					{
						strRange = L"#REF";
					}
					else if (xti->itabFirst < global_info_->sheets_info.size())
					{
						strRange = XMLSTUFF::name2sheet_name(global_info_->sheets_info[xti->itabFirst].name, L"");
						if (xti->itabFirst != xti->itabLast)
						{
							strRange += std::wstring(L":") + XMLSTUFF::name2sheet_name(global_info_->sheets_info[xti->itabLast].name, L"");
						}
					}
					val_1.link = strRange;
				}
				else
				{
					val_1.link = XMLSTUFF::xti_indexes2sheet_name(xti->itabFirst, xti->itabLast, info->rgst, val_1.link); 
				}
			}
			else if (xti->itabFirst == -1)
			{
				//sheet not found
			}
			else if (xti->itabFirst == -2)
			{
				//Workbook-level
			}
			global_info_->arXti_External.push_back(val_1);
		}

		global_info_->arXti.push_back(val);
	}
}

void GlobalsSubstream::UpdateDefineNames()
{
	for (size_t s = 0; s < m_arLBL.size(); s++)
	{
		LBL* LBL_ = dynamic_cast<LBL*>(m_arLBL[s].get());
		if (!LBL_) continue;

		Lbl *lbl = dynamic_cast<Lbl*>(LBL_->m_Lbl.get());
		if (!lbl) continue;

		std::wstring name;
		std::wstring comment;

		if (lbl->fBuiltin)	name = lbl->Name;	
		if (name.empty())	name = lbl->Name_bin.value();
		
		NameCmt *namecmt = dynamic_cast<NameCmt*>(LBL_->m_NameCmt.get());
		if (namecmt)
		{
			if (name.empty())
				name = namecmt->name.value();
			comment = namecmt->comment.value();
		}
		
		std::wstring value = lbl->rgce.getAssembledFormula(lbl->fWorkbookParam/*lbl->itab == 0 ? true : false*/);

		if (!value.empty() && !name.empty())
		{
			int ind_sheet = lbl->itab;

			std::map<std::wstring, std::vector<std::wstring>>::iterator it = global_info_->mapDefineNames.find(name);
			
			if (it != global_info_->mapDefineNames.end())
			{
				while ( it->second.size() <= ind_sheet)
				{
					it->second.push_back(L"");
				}
				it->second[ind_sheet] = value;
				//it->second.push_back(value);
			}
			else
			{
				std::vector<std::wstring> ar(ind_sheet + 1);
			
				ar[ind_sheet] = value;
				//ar.push_back(value);

				global_info_->mapDefineNames.insert(std::make_pair(name, ar));
			}
			LBL_->isSerialize = true;
		}
		else
		{
			if (lbl->fFunc)
			{
				if (name == L"FORMULA") //"general_formulas.xls"
						name = L"_xludf." + name;
			}
		}
		global_info_->arDefineNames.push_back(name);// для имен функций - todooo ... не все функции корректны !! БДИ !!
	}
}
void GlobalsSubstream::UpdateExternalDefineNames()
{
	for (size_t s = 0; s < m_arSUPBOOK.size(); s++)
	{
		SUPBOOK* SUPBOOK_ = dynamic_cast<SUPBOOK*>(m_arSUPBOOK[s].get());
		if (!SUPBOOK_) continue;

		SupBook *supbook = dynamic_cast<SupBook*>(SUPBOOK_->m_SupBook.get());
		if (!supbook) continue;

		global_info_->external_sheets_info.clear();

		for (size_t i = 0; i < supbook->rgst.size(); i++)
		{
			global_info_->external_sheets_info.push_back(supbook->rgst[i]);
		}

		for (size_t i = 0; i < SUPBOOK_->m_arExternName.size(); i++)
		{
			ExternName *externName = dynamic_cast<ExternName*>(SUPBOOK_->m_arExternName[i].get());
			if (!externName) continue;
			
			ExternDocName* docName = dynamic_cast<ExternDocName*>(externName->body.get());
			if(docName)
			{
				if (docName->ixals > 0 && docName->ixals < supbook->rgst.size())
				{
				}
				else
				{
					SUPBOOK::_def_name def_name;
					def_name.fmla		= docName->nameDefinition.getAssembledFormula();
					
					if (false == def_name.fmla.empty())
					{
						SUPBOOK_->mapNamesExt.insert(std::make_pair(docName->extName.value(), def_name));
					}
				}
			}
		}
	}
}
int GlobalsSubstream::serialize_protection(std::wostream & _stream)
{
	if (!m_PROTECTION) return 0;

	return m_PROTECTION->serialize(_stream);
}

int GlobalsSubstream::serialize_format(std::wostream & _stream)
{
	BookExt *book_ext = dynamic_cast<BookExt*>(m_BookExt.get());
	CodeName *code_name = dynamic_cast<CodeName*>(m_CodeName.get());

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"workbookPr")
		{
			if (code_name)
			{
				CP_XML_ATTR(L"codeName", code_name->value);
			}
			if (book_ext)
			{
				CP_XML_ATTR(L"hidePivotFieldList",	book_ext->fHidePivotList);
				CP_XML_ATTR(L"filterPrivacy",		book_ext->fFilterPrivacy);
			}
		}
	}
	return 0;
}

} // namespace XLS
