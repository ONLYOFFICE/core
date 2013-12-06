#pragma once
#ifndef OOX_THEME_PATH_INCLUDE_H_
#define OOX_THEME_PATH_INCLUDE_H_

#include "./../WritingElement.h"
#include "FillToRect.h"
#include "nullable.h"
#include <string>


namespace OOX
{
	namespace Theme
	{
		class Path : public WritingElement
		{
		public:
			Path();
			virtual ~Path();
			explicit Path(const XML::XNode& node);
			const Path& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_path;
			nullable__<FillToRect>	m_fillToRect;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_PATH_INCLUDE_H_