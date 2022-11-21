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

#include "PIVOTVIEW.h"
#include "PIVOTVIEWEX.h"
#include "PIVOTCORE.h"
#include "PIVOTFRT.h"
#include "PIVOTVD.h"
#include "PIVOTIVD.h"
#include "PIVOTPI.h"
#include "PIVOTLI.h"
#include "PIVOTEX.h"
#include "PIVOTADDL.h"
#include "PIVOTFORMAT.h"
#include "PIVOTFRT9.h"
#include "PIVOTCACHEDEFINITION.h"
#include "DBQUERY.h"
#include "SXSRC.h"

#include "../Biff_records/SXDI.h"
#include "../Biff_records/SxView.h"
#include "../Biff_records/SXAddl.h"
#include "../Biff_records/SXEx.h"
#include "../Biff_records/SxFormat.h"
#include "../Biff_records/SxDXF.h"
#include "../Biff_records/SXViewEx9.h"
#include "../Biff_records/SXStreamID.h"
#include "../Biff_records/QsiSXTag.h"

namespace XLS
{

PIVOTVIEW::PIVOTVIEW()
{
	indexStream = -1;
	indexCache	= -1;
}

PIVOTVIEW::~PIVOTVIEW()
{
}

BaseObjectPtr PIVOTVIEW::clone()
{
	return BaseObjectPtr(new PIVOTVIEW(*this));
}

// PIVOTVIEW = PIVOTCORE [PIVOTFRT]
const bool PIVOTVIEW::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<PIVOTCORE>())
	{
		return false;
	}
	m_PIVOTCORE = elements_.back();
	elements_.pop_back();

	PIVOTCORE *core = dynamic_cast<PIVOTCORE*>(m_PIVOTCORE.get());
	
	SxView* view = dynamic_cast<SxView*>(core->m_SxView.get());
	if (view)
	{
		name = view->stTable.value();
	}
	
	if (proc.optional<PIVOTFRT>())
	{
		m_PIVOTFRT = elements_.back();
		elements_.pop_back();
	}
//-------------------------------------------------------------------------------------------------
	PIVOTCACHEDEFINITION* pivot_cache = dynamic_cast<PIVOTCACHEDEFINITION*>(global_info_->arPIVOTCACHEDEFINITION[view->iCache].get());
	if (pivot_cache)
	{
		SXSRC*			src			= dynamic_cast<SXSRC*>		(pivot_cache->m_SXSRC.get());
		SXStreamID *	stream_id	= dynamic_cast<SXStreamID*>	(pivot_cache->m_SXStreamID.get());
		PIVOTFRT*		frt			= dynamic_cast<PIVOTFRT*>	(m_PIVOTFRT.get());

		indexStream = stream_id ? stream_id->idStm : -1;

		if (frt && src)
		{
			pivot_cache->m_PIVOTADDL = frt->m_PIVOTADDL;

			PIVOTFRT9* frt9		= dynamic_cast<PIVOTFRT9*>(frt->m_PIVOTFRT9.get());
			if (frt9)
			{
				pivot_cache->m_PIVOTVIEWEX = frt9->m_PIVOTVIEWEX;

				DBQUERY * db_query	= dynamic_cast<DBQUERY*>(src->m_source.get());
				if (db_query)
				{
					QsiSXTag *qsiTag =dynamic_cast<QsiSXTag*>(frt9->m_QsiSXTag.get());
					if (qsiTag->fTensorEx)
					{
		//OLAP !!!
						src->bOLAP = true;
						core->bOLAP = true;
					}
					db_query->m_DBQUERYEXT = frt9->m_DBQUERYEXT;
				}
			}
		}
	}
	return true;
}

int PIVOTVIEW::serialize(std::wostream & strm)
{
	PIVOTCORE* core = dynamic_cast<PIVOTCORE*>(m_PIVOTCORE.get());
	if (!core) return 0;

	SxView* view = dynamic_cast<SxView*>(core->m_SxView.get());
	if (!view) return 0;

	PIVOTFRT*	frt			= dynamic_cast<PIVOTFRT*>(m_PIVOTFRT.get());
	PIVOTEX*	pivot_ex	= dynamic_cast<PIVOTEX*>(core->m_PIVOTEX.get());

	PIVOTADDL* addls	= frt ? dynamic_cast<PIVOTADDL*>(frt->m_PIVOTADDL.get()) : NULL;
    PIVOTFRT9* frt9		= frt ? dynamic_cast<PIVOTFRT9*>(frt->m_PIVOTFRT9.get()) : NULL;

	PIVOTVIEWEX* ex = frt9 ? dynamic_cast<PIVOTVIEWEX*>(frt9->m_PIVOTVIEWEX.get()) : NULL;

	SXEx						*view_ex	= pivot_ex ? dynamic_cast<SXEx*>(pivot_ex->m_SXEx.get()) : NULL;
    SXViewEx9					*view_ex9	= frt9 ? dynamic_cast<SXViewEx9*>(frt9->m_SXViewEx9.get()) : NULL;    
	SXAddl_SXCView_SXDVer10Info *view_ex10	= addls ? dynamic_cast<SXAddl_SXCView_SXDVer10Info*>(addls->m_SXAddl_SXCView_SXDVer10Info.get()) : NULL;
    SXAddl_SXCView_SXDVer12Info *view_ex12	= addls ? dynamic_cast<SXAddl_SXCView_SXDVer12Info*>(addls->m_SXAddl_SXCView_SXDVer12Info.get()) : NULL;

	std::map<int, int>::iterator pFindIndex = global_info_->mapPivotCacheIndex.find(indexStream);

	if (pFindIndex == global_info_->mapPivotCacheIndex.end())
		return 0;

	indexCache = pFindIndex->second;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotTableDefinition")
		{ 
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
			
			CP_XML_ATTR(L"name",				view->stTable.value()); 
			CP_XML_ATTR(L"cacheId",				indexCache); 
			if (view->sxaxis4Data.bRw)
			{
				CP_XML_ATTR(L"dataOnRows",		view->sxaxis4Data.bRw); 
			}
			CP_XML_ATTR(L"applyNumberFormats",	view->fAtrNum);
			CP_XML_ATTR(L"applyBorderFormats",	view->fAtrBdr); 
			CP_XML_ATTR(L"applyFontFormats",	view->fAtrFnt);
			CP_XML_ATTR(L"applyPatternFormats",	view->fAtrPat);
			CP_XML_ATTR(L"applyAlignmentFormats",	view->fAtrAlc);
			CP_XML_ATTR(L"applyWidthHeightFormats",	view->fAtrProc);
			if (!view->stData.value().empty())
			{
				CP_XML_ATTR(L"dataCaption",			view->stData.value()); 
			}
			CP_XML_ATTR(L"showMemberPropertyTips",	0);
			CP_XML_ATTR(L"useAutoFormatting",		view->fAutoFormat); 
            CP_XML_ATTR(L"autoFormatId",			view->itblAutoFmt);
			if (view_ex9)
			{
				CP_XML_ATTR(L"itemPrintTitles",		view_ex9->fPrintTitles);
				CP_XML_ATTR(L"outline",				view_ex9->fLineMode);

                CP_XML_ATTR(L"outlineData",			view_ex12 ? view_ex12->fOutlineData : view_ex9->fLineMode);
			}
            CP_XML_ATTR(L"asteriskTotals",          view_ex10 ? view_ex10->fNotVisualTotals : 0);

            if (view_ex12)
            {
                CP_XML_ATTR(L"indent",				view_ex12->cIndentInc );
                CP_XML_ATTR(L"published",			view_ex12->fPublished);
                CP_XML_ATTR(L"compact",				view_ex12->fCompactData);
                CP_XML_ATTR(L"compactData",			view_ex12->fCompactData);

                CP_XML_ATTR(L"gridDropZones",		!view_ex12->fNewDropZones);
                CP_XML_ATTR(L"showDrill",           !view_ex12->fHideDrillIndicators);
                CP_XML_ATTR(L"printDrill",          view_ex12->fPrintDrillIndicators);
            }

			if (view_ex)
			{
                if (!view_ex->fEnableWizard)	CP_XML_ATTR(L"enableWizard", 0);
				if (!view_ex->fEnableDrilldown)	CP_XML_ATTR(L"enableDrill",	0);
				//CP_XML_ATTR(L"disableFieldList",	!view_ex->fEnableFieldDialog);//enableFieldPropert
				
				if (!view_ex->stError.value().empty())
					CP_XML_ATTR(L"errorCaption",	view_ex->stError.value());
			}

			CP_XML_NODE(L"location")
			{
				CP_XML_ATTR(L"ref", view->ref.toString());
				CP_XML_ATTR(L"firstHeaderRow", view->rwFirstHead - view->ref.rowFirst );
				CP_XML_ATTR(L"firstDataRow", view->rwFirstData - view->ref.rowFirst);
				CP_XML_ATTR(L"firstDataCol", view->colFirstData - view->ref.columnFirst); 

				if (view->cDimPg > 0)
				{
					CP_XML_ATTR(L"rowPageCount", view->cDimPg); 
					CP_XML_ATTR(L"colPageCount", 1);
				}
			}
			if (!core->m_arPIVOTVD.empty())
			{
				CP_XML_NODE(L"pivotFields")
				{
					CP_XML_ATTR(L"count", view->cDim);//Sxvd 
					for (size_t i = 0; i <  core->m_arPIVOTVD.size(); i++)
					{
						core->m_arPIVOTVD[i]->serialize(CP_XML_STREAM());
					}
				}
			}
			size_t index_ivd = 0;
			size_t index_tli = 0;

			if (view->cDimRw > 0 && index_ivd < core->m_arPIVOTIVD.size())
			{
				CP_XML_NODE(L"rowFields")
				{
					CP_XML_ATTR(L"count", view->cDimRw);

					PIVOTIVD* ivd = dynamic_cast<PIVOTIVD*>(core->m_arPIVOTIVD[index_ivd].get());
					ivd->serialize(CP_XML_STREAM());
					index_ivd++;
				}
			}
			if (view->cRw > 0 && index_tli < core->m_arPIVOTLI.size())
			{
				CP_XML_NODE(L"rowItems")
				{
					CP_XML_ATTR(L"count", view->cRw);
					
					PIVOTLI* line = dynamic_cast<PIVOTLI*>(core->m_arPIVOTLI[index_tli].get());
					line->serialize(CP_XML_STREAM());
					index_tli++;
				}
			}
			if (view->cDimCol > 0 && index_ivd < core->m_arPIVOTIVD.size())
			{
				CP_XML_NODE(L"colFields")
				{
					CP_XML_ATTR(L"count", view->cDimCol);
					
					PIVOTIVD* ivd = dynamic_cast<PIVOTIVD*>(core->m_arPIVOTIVD[index_ivd].get());
					ivd->serialize(CP_XML_STREAM());
				}
			}
			if (view->cCol > 0 && index_tli < core->m_arPIVOTLI.size())
			{
				CP_XML_NODE(L"colItems")
				{
					CP_XML_ATTR(L"count", view->cCol);
					
					PIVOTLI* line = dynamic_cast<PIVOTLI*>(core->m_arPIVOTLI[index_tli].get());
					line->serialize(CP_XML_STREAM());
				}
			}
			if (core->m_PIVOTPI)
			{
				CP_XML_NODE(L"pageFields")
				{
					CP_XML_ATTR(L"count", view->cDimPg);
					
					core->m_PIVOTPI->serialize(CP_XML_STREAM());
				}
			}
			if (!core->m_arSXDI.empty())
			{
				CP_XML_NODE(L"dataFields")
				{
					CP_XML_ATTR(L"count", view->cDimData);
					for (size_t i = 0; i <  core->m_arSXDI.size(); i++)
					{
						core->m_arSXDI[i]->serialize(CP_XML_STREAM());
					}
				}
			}

			if (ex)
				ex->serialize_table_view(CP_XML_STREAM());
			
			if ((addls) && (addls->m_SXAddl_SXCView_SXDTableStyleClient))
			{
				SXAddl_SXCView_SXDTableStyleClient* table_style = dynamic_cast<SXAddl_SXCView_SXDTableStyleClient*>(addls->m_SXAddl_SXCView_SXDTableStyleClient.get());
				CP_XML_NODE(L"pivotTableStyleInfo")
				{
					if (!table_style->stName.value().empty())
						CP_XML_ATTR(L"name", table_style->stName.value()); 
					
					CP_XML_ATTR(L"showRowHeaders", table_style->fRowHeaders); 
					CP_XML_ATTR(L"showColHeaders", table_style->fColumnHeaders);
					CP_XML_ATTR(L"showRowStripes", table_style->fRowStrips);
					CP_XML_ATTR(L"showColStripes", table_style->fColumnStrips);
					CP_XML_ATTR(L"showLastColumn", table_style->fLastColumn);
				}
			}
		}
	}

	return 0;
}

} // namespace XLS

