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
		
		void SetInset		(int nLeft, int nTop, int nRight, int nBottom);
		void SetTextboxStyle(const std::wstring & style);

	private:    

		static int TextboxCount;
		
		int				m_nTBIndex;
		
		double			m_dxTextLeft;
		double			m_dyTextTop;
		double			m_dxTextRight;
		double			m_dyTextBottom;

		std::wstring	m_sTextBoxStyle;
	};
}