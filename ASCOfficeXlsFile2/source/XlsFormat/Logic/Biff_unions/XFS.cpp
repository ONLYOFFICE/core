
#include "XFS.h"
#include <Logic/Biff_records/XF.h>
#include <Logic/Biff_records/XFCRC.h>
#include <Logic/Biff_records/XFExt.h>

#include <Logic/Biff_structures/ExtProp.h>

namespace XLS
{


XFS::XFS()
:	cell_xf_current_id(0), style_xf_current_id(0)
{
}


XFS::~XFS()
{
}


BaseObjectPtr XFS::clone()
{
	return BaseObjectPtr(new XFS(*this));
}


// XFS = 16*XF [XFCRC 16*4050XFExt]
const bool XFS::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();
	
	global_info->cellStyleXfs_count = 0;
	
    XF xf(cell_xf_current_id, style_xf_current_id);
    int count = proc.repeated(xf ,16, 0);

	int ind = 0;
	while (count > 0 && elements_.size() > 0)
	{
		//разделить style & complex
		XF* xfs = dynamic_cast<XF*>(elements_.front().get());

		xfs->ind_xf = ind++;

		if (xfs->fStyle)
		{
			m_arCellStyles.push_back(elements_.front());
			global_info->cellStyleXfs_count++;
		}
		else
		{
			m_arCellXFs.push_back(elements_.front());
		}
		
		elements_.pop_front();
		count--;
	}
	
	if(proc.optional<XFCRC>())
	{
		elements_.pop_back(); // Crc не нужен

		count = proc.repeated<XFExt>(16, 4050);
		while (count > 0)
		{
			if (elements_.empty()) break;

			XFExt* ext = dynamic_cast<XFExt*>(elements_.front().get());

			m_arXFext.push_back(elements_.front());
			elements_.pop_front();
			count--;
		}
	}
//------------------------------------------------------------------------------------
	int first_xf_ext = 0;

	for (_UINT16 i = 0 ; i < m_arCellStyles.size(); i++)
	{
		XF		*xfs = dynamic_cast<XF*>(m_arCellStyles[i].get());

		xfs->style.RegisterFillBorder();

	}
	first_xf_ext = 0;
	
    for (int i = 0 ; i < m_arCellXFs.size(); i++)
	{
		XF		*xfs = dynamic_cast<XF*>(m_arCellXFs[i].get());

		if (m_arXFext.size() > 0 && xfs->cell.fHasXFExt)
		{
			for (_UINT16 j = first_xf_ext ; j < m_arXFext.size(); j++)
			{
				XFExt *ext = dynamic_cast<XFExt*>(m_arXFext[j].get());
				if (ext->ixfe > i)break;

				if (ext->ixfe == xfs->ind_xf)
				{
					xfs->cell.ext_props = ext->rgExt;
					first_xf_ext	= j + 1;
					break;
				}
			}
		}		
		xfs->cell.RegisterFillBorder();
	}

	return true;
}
int XFS::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cellStyleXfs")
		{
			CP_XML_ATTR(L"count", m_arCellStyles.size());
            for (int i = 0; i < m_arCellStyles.size(); i++)
			{
				m_arCellStyles[i]->serialize(CP_XML_STREAM());
			}
		}
		CP_XML_NODE(L"cellXfs")
		{
			CP_XML_ATTR(L"count", m_arCellXFs.size());
            for (int i = 0; i < m_arCellXFs.size(); i++)
			{
				m_arCellXFs[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}

} // namespace XLS

