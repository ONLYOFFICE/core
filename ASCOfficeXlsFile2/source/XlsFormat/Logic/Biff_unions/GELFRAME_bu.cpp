
#include "GELFRAME.h"
#include <Logic/Biff_records/GelFrame.h>
#include <Logic/Biff_records/Continue.h>

#include <Logic/Biff_unions/PICF.h>

#include <simple_xml_writer.h>

#include "../../../XlsXlsxConverter/XlsConverter.h"
#include "../../../XlsXlsxConverter/xlsx_conversion_context.h"

namespace XLS
{


GELFRAME::GELFRAME()
{
}


GELFRAME::~GELFRAME()
{
}


BaseObjectPtr GELFRAME::clone()
{
	return BaseObjectPtr(new GELFRAME(*this));
}


// GELFRAME = 1*2GelFrame *Continue [PICF]
const bool GELFRAME::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<GelFrame>())
	{
		return false;
	}
	m_GelFrame = elements_.back();
	elements_.pop_back();
	
	if (proc.optional<GelFrame>())
	{
		GelFrame * base		= dynamic_cast<GelFrame*>(m_GelFrame.get());
		GelFrame * addit	= dynamic_cast<GelFrame*>(elements_.back().get());

		if (base && addit)
		{
			base->concatinate(addit);
		}
		elements_.pop_back();
	}
	//
	int count = proc.repeated<Continue>(0, 0);

	if (proc.optional<PICF>())
	{
		m_PICF = elements_.back();
		elements_.pop_back();
	}

	return true;
}

int GELFRAME::serialize(std::wostream & _stream)
{
	GelFrame * frame = dynamic_cast<GelFrame*>(m_GelFrame.get());
	if (frame == NULL) return 0;

	global_info->xls_converter->xlsx_context->get_drawing_context().start_drawing(2); //rect
		global_info->xls_converter->convert(&frame->OPT1);
		global_info->xls_converter->convert(&frame->OPT2);

		global_info->xls_converter->xlsx_context->get_drawing_context().serialize_fill(_stream);
		global_info->xls_converter->xlsx_context->get_drawing_context().serialize_line(_stream);

	global_info->xls_converter->xlsx_context->get_drawing_context().end_drawing();
	return 0;
}

} // namespace XLS

