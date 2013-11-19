#pragma once
#ifndef ODT_CONTENT_AUTOMATIC_STYLES_INCLUDE_H_
#define ODT_CONTENT_AUTOMATIC_STYLES_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include <string>
//#include "./../Logic/Styles.h"
//#include "./../Logic/Style.h"
#include <boost/shared_ptr.hpp>


// TODO устаревший
namespace Odt
{
	namespace Content
	{
		class AutomaticStyles : public WritingElement
		{
		public:
			AutomaticStyles();
			virtual ~AutomaticStyles();
			const AutomaticStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
//			void setStyles(const boost::shared_ptr<Logic::Styles>& styles);

		private:
//			boost::shared_ptr<Logic::Styles> m_styles;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_AUTOMATIC_STYLES_INCLUDE_H_