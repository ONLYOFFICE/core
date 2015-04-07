#pragma once

#include "DocumentMapping.h"
#include "TableInfo.h"

namespace DocFileFormat
{
	class TextboxMapping : public DocumentMapping
	{
	public:

		TextboxMapping(ConversionContext* ctx, int textboxIndex, XmlUtils::CXmlWriter* writer, IMapping* caller);
		TextboxMapping(ConversionContext* ctx, XmlUtils::CXmlWriter* writer, IMapping* caller);
		virtual ~TextboxMapping();
		virtual void Apply(IVisitable* visited);
		void SetInset(int nLeft, int nTop, int nRight, int nBottom);

	private:    

		static int TextboxCount;
		
		int m_nTBIndex;
		
		int m_ndxTextLeft;
		int m_ndyTextTop;
		int m_ndxTextRight;
		int m_ndyTextBottom;
	};
}