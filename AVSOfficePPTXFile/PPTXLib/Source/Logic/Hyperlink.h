#pragma once
#ifndef PPTX_LOGIC_HYPERLINK_INCLUDE_H_
#define PPTX_LOGIC_HYPERLINK_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "DocxFormat/RId.h"
#include "Media/WavAudioFile.h"

namespace PPTX
{
	namespace Logic
	{

		class Hyperlink : public WrapperWritingElement
		{
		public:
			Hyperlink();
			virtual ~Hyperlink();
			explicit Hyperlink(const XML::XNode& node);
			const Hyperlink& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<WavAudioFile> snd;

			nullable_property<std::string> id;//<OOX::RId> id;//  <xsd:attribute ref="r:id" use="optional"/>
			nullable_property<std::string> invalidUrl;//default=""
			nullable_property<std::string> action;//default=""
			nullable_property<std::string> tgtFrame;//default=""
			nullable_property<std::string> tooltip;//default=""
			nullable_property<bool> history;//default="true"
			nullable_property<bool> highlightClick;//default="false"
			nullable_property<bool> endSnd;//default="false"
		//private:
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HYPERLINK_INCLUDE_H_