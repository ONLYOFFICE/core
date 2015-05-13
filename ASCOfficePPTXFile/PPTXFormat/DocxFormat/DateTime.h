#pragma once
#ifndef OOX_DATE_TIME_INCLUDE_H_
#define OOX_DATE_TIME_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/Utility/Utility.h"


namespace PPTX
{
	class DateTime
	{
	public:
		DateTime()
		{
		}
		explicit DateTime(const CString& value) : m_datetime(value, s_pattern)
		{
		}
		explicit DateTime(const ::DateTime& dt) : m_datetime(dt)
		{
		}

	public:
		CString ToString() const
		{
			return m_datetime.ToString(s_pattern);
		}
		static DateTime Parse(const CString& value)
		{
			return DateTime(value);
		}

	public:
		::DateTime& datetime()
		{
			return m_datetime;
		}
		const ::DateTime& datetime() const
		{
			return m_datetime;
		}

	private:
		static const CString s_pattern;
		::DateTime m_datetime;
	};

    const CString DateTime::s_pattern = _T("%YYYY-%MM-%DDT%hh:%mm:%ssZ");
} // namespace PPTX

#endif // OOX_DATE_TIME_INCLUDE_H_
