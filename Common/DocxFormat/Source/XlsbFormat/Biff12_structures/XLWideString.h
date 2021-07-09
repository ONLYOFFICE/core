#ifndef XLWIDESTRING_H
#define XLWIDESTRING_H

#include <Logic/Biff_structures/BiffString.h>
using namespace XLS;
namespace XLSB
{

   typedef XLUnicodeString_T<unsigned int,	aw_WIDE,					cch_READ_FROM_RECORD>		XLWideString;

}   // namespace XLS

#endif // XLWIDESTRING_H
