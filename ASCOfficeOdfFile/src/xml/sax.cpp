
#include "../../include/cpdoccore/CPSharedPtr.h"
#include "../../include/cpdoccore/xml/sax.h"
#include "sax_xmllite.h"
#include <sstream>

namespace cpdoccore {
namespace xml {

	sax_ptr create_sax(const wchar_t * FileName)
	{
		return create_sax_xmllite(FileName);
	}

}
}
