#include "precompiled_xls.h"
#include "XFS.h"
#include <Logic/Biff_records/XF.h>
#include <Logic/Biff_records/XFCRC.h>
#include <Logic/Biff_records/XFExt.h>

#include <simple_xml_writer.h>

namespace XLS
{;


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
	int count = proc.repeated(XF(cell_xf_current_id, style_xf_current_id) ,16, 0);

	while (count > 0)
	{
		//разделить style & complex
		XF* xfs = dynamic_cast<XF*>(elements_.back().get());

		if ((xfs->fStyle.value()) && (*xfs->fStyle.value()))
		{
			m_cell_styles.insert(m_cell_styles.begin(), elements_.back());
		}
		else
		{
			m_cell_xfs.insert(m_cell_xfs.begin(), elements_.back());
		}
		
		elements_.pop_back();
		count--;
	}
	
	if(proc.optional<XFCRC>())
	{
		count = proc.repeated<XFExt>(16, 4050);
		while (count > 0)
		{
			m_xf_ext.insert(m_xf_ext.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}
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
		CP_XML_NODE(L"cellStyles")
		{
			CP_XML_ATTR(L"count", 0);
		}
	}
	return 0;
}

} // namespace XLS

