#pragma once
#ifndef ODP_STYLES_MARKERS_INCLUDE_H_
#define ODP_STYLES_MARKERS_INCLUDE_H_

#include "OdtFormat/WritingVector.h"
#include "Marker.h"

namespace Odp
{
	namespace Styles
	{
		class Markers : public Odt::WritingVector<Marker>
		{
		public:
			Markers();
			virtual ~Markers();
			explicit Markers(const XML::XNode& node);
			const Markers& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const Marker& operator[](const std::string& name) const;
			Marker& operator[](const std::string& name);
			const Marker& find(const std::string& name) const;
			Marker& find(const std::string& name);
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_MARKERS_INCLUDE_H_