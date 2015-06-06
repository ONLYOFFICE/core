
#include <logging.h>
#include <iostream>
#include <sstream>

namespace cpdoccore
{
    std::wstringstream              log_stringstream;
    logging< std::wostream >        logging_cout    (std::wcout);
    logging< std::wstringstream >   logging_log     (log_stringstream);

}
