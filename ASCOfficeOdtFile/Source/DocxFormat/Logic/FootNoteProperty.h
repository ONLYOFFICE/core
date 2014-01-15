#ifndef OOX_LOGIC_FOOTNOTE_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_FOOTNOTE_PROPERTY_INLCUDE_H_

#include "NoteProperty.h"


namespace OOX
{
	namespace Logic
	{
		class FootNoteProperty : public NoteProperty, public WritingElement
		{
		public:
			FootNoteProperty();
			virtual ~FootNoteProperty();
			explicit FootNoteProperty(const XML::XNode& node);
			const FootNoteProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FOOTNOTE_PROPERTY_INCLUDE_H_