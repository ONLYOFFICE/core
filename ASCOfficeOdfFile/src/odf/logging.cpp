
#include <logging.h>
#include <iostream>

namespace cpdoccore
{
    logging< std::wostream > logging_cout(std::wcout);

}
