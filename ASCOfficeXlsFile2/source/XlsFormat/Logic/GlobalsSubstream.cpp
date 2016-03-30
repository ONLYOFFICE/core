
#include "GlobalsSubstream.h"
#include <Logic/AnyObject.h>
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/WriteProtect.h>
#include <Logic/Biff_records/FilePass.h>
#include <Logic/Biff_records/Template.h>
#include <Logic/Biff_unions/INTERFACE.h>
#include <Logic/Biff_records/WriteAccess.h>
#include <Logic/Biff_records/FileSharing.h>
#include <Logic/Biff_records/CodePage.h>
#include <Logic/Biff_records/Lel.h>
#include <Logic/Biff_records/DSF.h>
#include <Logic/Biff_records/Excel9File.h>
#include <Logic/Biff_records/RRTabId.h>
#include <Logic/Biff_records/ObProj.h>
#include <Logic/Biff_records/ObNoMacros.h>
#include <Logic/Biff_records/CodeName.h>
#include <Logic/Biff_unions/FNGROUPS.h>
#include <Logic/Biff_records/Lbl.h>
#include <Logic/Biff_records/OleObjectSize.h>
#include <Logic/Biff_unions/PROTECTION.h>
#include <Logic/Biff_records/Window1.h>
#include <Logic/Biff_records/Backup.h>
#include <Logic/Biff_records/HideObj.h>
#include <Logic/Biff_records/Date1904.h>
#include <Logic/Biff_records/CalcPrecision.h>
#include <Logic/Biff_records/RefreshAll.h>
#include <Logic/Biff_records/BookBool.h>
#include <Logic/Biff_unions/FORMATTING.h>
#include <Logic/Biff_unions/PIVOTCACHEDEFINITION.h>
#include <Logic/Biff_unions/DOCROUTE.h>
#include <Logic/Biff_records/UserBView.h>
#include <Logic/Biff_records/UsesELFs.h>
#include <Logic/Biff_unions/BUNDLESHEET.h>
#include <Logic/Biff_unions/METADATA.h>
#include <Logic/Biff_records/MTRSettings.h>
#include <Logic/Biff_records/ForceFullCalculation.h>
#include <Logic/Biff_records/Country.h>
#include <Logic/Biff_unions/SUPBOOK.h>
#include <Logic/Biff_unions/LBL.h>
#include <Logic/Biff_unions/RTD.h>
#include <Logic/Biff_records/RecalcId.h>
#include <Logic/Biff_records/HFPicture.h>
#include <Logic/Biff_unions/MSODRAWINGGROUP.h>
#include <Logic/Biff_unions/SHAREDSTRINGS.h>
#include <Logic/Biff_records/ExtSST.h>
#include <Logic/Biff_records/WebPub.h>
#include <Logic/Biff_records/WOpt.h>
#include <Logic/Biff_records/CrErr.h>
#include <Logic/Biff_records/BookExt.h>
#include <Logic/Biff_records/FeatHdr.h>
#include <Logic/Biff_records/DConn.h>
#include <Logic/Biff_unions/THEME.h>
#include <Logic/Biff_records/CompressPictures.h>
#include <Logic/Biff_records/Compat12.h>
#include <Logic/Biff_records/GUIDTypeLib.h>
#include <Logic/Biff_records/EOF.h>
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/MDTInfo.h>
#include <Logic/Biff_records/ExternSheet.h>
#include <Logic/Biff_records/XFExt.h>
#include <Logic/Biff_records/XFCRC.h>
#include <Logic/Biff_records/DXF.h>
#include <Logic/Biff_unions/STYLES.h>

namespace XLS
{;

GlobalsSubstream::GlobalsSubstream(const unsigned short code_page)
:	code_page_(code_page)
{
}

GlobalsSubstream::~GlobalsSubstream()
{
}


BaseObjectPtr GlobalsSubstream::clone()
{
	return BaseObjectPtr(new GlobalsSubstream(*this));
}


// class Parenthesis_Window1_Backup_HideObj_Date1904 : public ABNFParenthesis
// {
// public:
// 	BaseObjectPtr clone()
// 	{
// 		return BaseObjectPtr(new Parenthesis_OO_order_workaround_1(*this));
// 	}
// 
// 	const bool loadContent(BinProcessor& proc)
// 	{
// 		if(!proc.mandatory<SXAddl_SXCCacheField_SXDCaption>())
// 		{
// 			return false;
// 		}
// 		proc.repeated<Continue_SxaddlSxString>(0, 0);
// 		return true;
// 	};
// };
// 

/*
WORKBOOKCONTENT = [WriteProtect] [FilePass] [Template] INTERFACE WriteAccess [FileSharing] CodePage 
					*2047Lel DSF [Excel9File] RRTabId [ObProj] [ObNoMacros] [CodeName [FNGROUPS] *Lbl 
					[OleObjectSize] PROTECTION 1*Window1 Backup HideObj Date1904 CalcPrecision RefreshAll 
					BookBool FORMATTING *(PIVOTCACHEDEFINITION) [DOCROUTE] *UserBView UsesELFs 1*BUNDLESHEET 
					METADATA [MTRSettings] [ForceFullCalculation] Country *SUPBOOK *LBL *RTD [RecalcId] 
					*HFPicture *MSODRAWINGGROUP [SHAREDSTRINGS] ExtSST *WebPub [WOpt] [CrErr] [BookExt] 
					*FeatHdr *DConn [THEME] [CompressPictures] [Compat12] [GUIDTypeLib] EOF
WORKBOOK = BOF WORKBOOKCONTENT
*/
const bool GlobalsSubstream::loadContent(BinProcessor& proc)
{
	int	count = 0;
	
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
			case rt_BOF:
			{
				if (proc.optional<BOF>())
				{
					BOF *bof = dynamic_cast<BOF*>(elements_.back().get());
					proc.getGlobalWorkbookInfo()->Version = bof->vers;
					if (proc.getGlobalWorkbookInfo()->Version < 0x0600)
					{				

						proc.getGlobalWorkbookInfo()->CodePage = 0; //???
					}
				}
			}break;
			case rt_WriteProtect:	proc.optional<WriteProtect>();	break;
			case rt_FilePass:		proc.optional<FilePass>();		break;
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
			case rt_DSF:				proc.optional<DSF>();			break;
			case rt_Excel9File:			proc.optional<Excel9File>();	break;
			case rt_RRTabId:			proc.optional<RRTabId>();		break;
			case rt_ObProj:				proc.optional<ObProj>();		break;
			case rt_ObNoMacros:			proc.optional<ObNoMacros>();	break;
			case rt_CodeName:			proc.optional<CodeName>();		break;
			case rt_BuiltInFnGroupCount:proc.optional<FNGROUPS>();		break;
			case rt_OleObjectSize:		proc.optional<OleObjectSize>();	break;
			case rt_WinProtect:			proc.optional<PROTECTION>();	break;
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

					proc.getGlobalWorkbookInfo()->CodePage = code_page_;
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
			case rt_Date1904:		proc.optional<Date1904>();		break;
			case rt_CalcPrecision:	proc.optional<CalcPrecision>();	break;
			case rt_RefreshAll:		proc.optional<RefreshAll>();	break;
			case rt_BookBool:		proc.optional<BookBool>();		break;
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

						proc.getGlobalWorkbookInfo()->CodePage;
					}
				}	
			}break;
			case rt_UsesELFs:		proc.optional<UsesELFs>();	break;
			case rt_RecalcId:		proc.optional<RecalcId>();	break;
			case rt_Font:	
			{
				if (proc.mandatory<FORMATTING>())
				{
					m_Formating = elements_.back();
					elements_.pop_back();

					FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());
					if (fmts)
					{
						proc.getGlobalWorkbookInfo()->m_arFonts = &fmts->m_arFonts;
					}
				}		
			}break;
			case rt_SXStreamID:			proc.repeated<PIVOTCACHEDEFINITION>(0, 0);	break;
			case rt_DocRoute:			proc.repeated<DOCROUTE>(0, 0);				break;
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
			case rt_BoundSheet8:			proc.repeated<BUNDLESHEET>(1, 0);		break;
			case rt_MDTInfo:				proc.optional<METADATA>();				break;
			case rt_MTRSettings:			proc.optional<MTRSettings>();			break;
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

					proc.getGlobalWorkbookInfo()->startAddedSharedStrings = shared_strings.size_;
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
			case rt_BookExt:			proc.optional<BookExt>();				break;
			case rt_FeatHdr:
			{
				FeatHdr feat_hdr(true);
				proc.repeated(feat_hdr, 0, 0);
			}break;
			case rt_DConn:				proc.repeated<DConn>(0, 0);				break;
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

	return true;
}

} // namespace XLS
