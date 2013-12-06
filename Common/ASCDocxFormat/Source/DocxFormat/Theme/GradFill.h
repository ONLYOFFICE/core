#pragma once
#ifndef OOX_STYLES_GRAD_FILL_INCLUDE_H_
#define OOX_STYLES_GRAD_FILL_INCLUDE_H_

#include "./../WritingElement.h"
#include "GsLst.h"
#include "Lin.h"
#include "Path.h"
#include "property.h"


namespace OOX
{
	namespace Theme
	{
		class GradFill : public WritingElement
		{
		public:
			GradFill();
			virtual ~GradFill();
			explicit GradFill(const XML::XNode& node);
			const GradFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int		m_rotWithShape;
			GsLst	m_gsLst;
			nullable__<Lin>		m_lin;
			nullable__<Path>	m_path;
		};
	} // namespace Theme
} // OOX

#endif // OOX_STYLES_GRAD_FILL_INCLUDE_H_