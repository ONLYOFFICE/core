#ifndef OOX_LOGIC_NOTE_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_NOTE_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "NumFormat.h"


namespace OOX
{
	namespace Logic
	{
		class NoteProperty
		{
		public:
			NoteProperty();
			~NoteProperty();

		public:
			property<Logic::NumFormat> NumFormat;
			property<int>			   NumStart;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_NOTE_PROPERTY_INCLUDE_H_