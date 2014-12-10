#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
	class AnnotationReferenceDescriptor: public ByteStructure
	{
	public:
		static const int STRUCTURE_SIZE = 30;

		inline std::wstring GetUserInitials() const
		{
			return m_UserInitials;
		}

		inline unsigned short GetAuthorIndex() const
		{
			return m_AuthorIndex;
		}

		inline int GetBookmarkId() const
		{
			return m_BookmarkId;
		}

		AnnotationReferenceDescriptor() : m_UserInitials( _T( "" ) ), m_AuthorIndex(0), m_BookmarkId(0)
		{
		}

		virtual ~AnnotationReferenceDescriptor() 
		{
		}

		virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length );

	private:

		/// The initials of the user who left the annotation.
		std::wstring	m_UserInitials;
		/// An index into the string table of comment author names.
		unsigned short	m_AuthorIndex;
		/// Identifies a bookmark.
		int				m_BookmarkId;
	};
}
