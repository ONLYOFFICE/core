#pragma once

#include <string>

#include "mediaitems.h"

namespace cpdoccore {
namespace odf {
namespace utils {
namespace media {

	std::wstring create_file_name(const std::wstring & uri, _mediaitems::Type type, size_t Num);
	std::wstring get_rel_type(_mediaitems::Type type);

}
}
}
}
