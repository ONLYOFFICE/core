#pragma once
#ifndef PPTX_LOGIC_ROT_INCLUDE_H_
#define PPTX_LOGIC_ROT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Rot : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Rot)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				lat = node.ReadAttributeInt(L"lat");
				lon = node.ReadAttributeInt(L"lon");
				rev = node.ReadAttributeInt(L"rev");

				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("lat"), lat);
				oAttr.Write(_T("lon"), lon);
				oAttr.Write(_T("rev"), rev);

				return XmlUtils::CreateNode(_T("a:rot"), oAttr);
			}
		public:
			int lat;
			int lon;
			int rev;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				normalize_value(lat, 0, 21600000);
				normalize_value(lon, 0, 21600000);
				normalize_value(rev, 0, 21600000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ROT_INCLUDE_H_