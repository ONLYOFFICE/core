#ifndef CPDOCCORE_MEDIAITEMS_UTILS_H_
#define CPDOCCORE_MEDIAITEMS_UTILS_H_

#include <string>

#include "mediaitems.h"

namespace cpdoccore {
namespace oox {
namespace utils {
namespace media {

	bool is_internal(const std::wstring & uri, const std::wstring & packetRoot);
	std::wstring get_rel_type(mediaitems::Type type);

}
}
}
}

#endif
