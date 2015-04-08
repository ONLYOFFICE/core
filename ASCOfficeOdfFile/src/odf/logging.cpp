#include "precompiled_cpodf.h"
#include <logging.h>
#include <iostream>

namespace cpdoccore {

logging< std::wostream > logging_err(std::wcerr);
logging< std::wostream > logging_cout(std::wcout);

}
