
#include "XFS.h"
#include <Logic/Biff_records/XF.h>
#include <Logic/Biff_records/XFCRC.h>
#include <Logic/Biff_records/XFExt.h>

#include <Logic/Biff_structures/ExtProp.h>
#include <simple_xml_writer.h>

namespace XLS
{

int cellStyleXfs_count = 0;

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
	
	cellStyleXfs_count = 0;
	
    XF xf(cell_xf_current_id, style_xf_current_id);
    int count = proc.repeated(xf ,16, 0);

	while (count > 0)
	{
		//разделить style & complex
		XF* xfs = dynamic_cast<XF*>(elements_.front().get());

		if ((xfs->fStyle.value()) && (*xfs->fStyle.value()))
		{
			m_cell_styles.push_back(elements_.front());
			cellStyleXfs_count++;
		}
		else
		{
			m_cell_xfs.push_back(elements_.front());
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
			XFExt* ext = dynamic_cast<XFExt*>(elements_.front().get());

			m_xf_ext.push_back(elements_.front());
			elements_.pop_front();
			count--;
		}
	}

	int first_xf_ext = 0;

	for (_UINT16 i = 0 ; i < m_cell_styles.size(); i++)
	{
		XF		*xfs = dynamic_cast<XF*>(m_cell_styles[i].get());

		if (m_xf_ext.size() > 0 && xfs->cell.fHasXFExt)
		{
			XFExt *ext_find = NULL;
			
			for (_UINT16 j = first_xf_ext ; j < m_xf_ext.size(); j++)
			{
				XFExt *ext = dynamic_cast<XFExt*>(m_xf_ext[j].get());
				if (ext->ixfe > i)break;

				if (ext->ixfe == i)
				{
					ext_find = ext;
					first_xf_ext = j + 1;
					break;
				}
			}
			if (ext_find)
			{
				xfs->style.ext_props = ext_find->rgExt;
			}
		}
		xfs->style.RegisterFillBorder();
		
		//if (xfs->cell.font_id < 0xFFFF)
		//{
		//	xfs->ifnt.setValue((unsigned short) xfs->style.font_id);
		//}
		

/*		if (xfs->style.font_color.enabled)
		{
			int font_id = xfs->ifnt;
			global_info->RegisterFontColorId(font_id, xfs->style.font_color);
		}*/		
	}
	
	for (long i = 0 ; i < m_cell_xfs.size(); i++)
	{
		XF		*xfs = dynamic_cast<XF*>(m_cell_xfs[i].get());

		if (m_xf_ext.size() > cellStyleXfs_count)
		{
			XFExt*ext = dynamic_cast<XFExt*>(m_xf_ext[i + cellStyleXfs_count].get());
			if (ext)
			{
				xfs->cell.ext_props = ext->rgExt;
			}
		}
		
		xfs->cell.RegisterFillBorder();

		//if (xfs->cell.font_id < 0xFFFF)
		//{
		//	xfs->ifnt.setValue( (unsigned short) xfs->cell.font_id);
		//}
		
		//if (xfs->cell.font_color.enabled)
		//{
		//	int font_id = xfs->ifnt;
		//	global_info->RegisterFontColorId(font_id, xfs->cell.font_color);
		//}
	}

	return true;
}
int XFS::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cellStyleXfs")
		{
			CP_XML_ATTR(L"count", m_cell_styles.size());
			for (long i = 0; i < m_cell_styles.size(); i++)
			{
				m_cell_styles[i]->serialize(CP_XML_STREAM());
			}
		}
		CP_XML_NODE(L"cellXfs")
		{
			CP_XML_ATTR(L"count", m_cell_xfs.size());
			for (long i = 0; i < m_cell_xfs.size(); i++)
			{
				m_cell_xfs[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}

} // namespace XLS

