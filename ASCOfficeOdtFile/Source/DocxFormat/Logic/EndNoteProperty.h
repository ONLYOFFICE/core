#ifndef OOX_LOGIC_ENDNOTE_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_ENDNOTE_PROPERTY_INCLUDE_H_

#include "NoteProperty.h"


namespace OOX
{
	namespace Logic
	{
		class EndNoteProperty : public NoteProperty, public WritingElement
		{
		public:
			EndNoteProperty();
			virtual ~EndNoteProperty();
			explicit EndNoteProperty(const XML::XNode& node);
			const EndNoteProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_ENDNOTE_PROPERTY_INCLUDE_H_