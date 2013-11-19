#pragma once
#ifndef PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_
#define PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "DocxFormat/RId.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class WavAudioFile : public WrapperWritingElement
		{
		public:
			WavAudioFile();
			virtual ~WavAudioFile();			
			explicit WavAudioFile(const XML::XNode& node);
			const WavAudioFile& operator =(const XML::XNode& node);
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> name;
			property<OOX::RId> embed;
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_