#ifndef _CPDOCCORE_ODF_PERCENT_H_
#define _CPDOCCORE_ODF_PERCENT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include "odfattributes.h"

namespace cpdoccore { 
	namespace odf_types { 

		class percent
		{
		public:
			percent(double val = 100.0) : value_(val)
			{}

			double get_value() const { return value_; }
			static percent parse(const std::wstring & Str);

		private:
			double value_;
		};

		std::wostream & operator << (std::wostream & _Wostream, const percent & _Percent);

		class percent_rel
		{
		public:
			percent_rel(int val) : value_(val)
			{}

			int get_value() const { return value_; }
			static percent_rel parse(const std::wstring & Str);

		private:
			int value_;
		};

		std::wostream & operator << (std::wostream & _Wostream, const percent_rel & _Percent);

	}

	APPLY_PARSE_XML_ATTRIBUTES(odf_types::percent);

	APPLY_PARSE_XML_ATTRIBUTES(odf_types::percent_rel);

}
#endif
