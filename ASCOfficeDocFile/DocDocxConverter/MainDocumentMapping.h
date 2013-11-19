#pragma once

#include "DocumentMapping.h"
#include "SectionPropertiesMapping.h"
#include "TableInfo.h"
#include "TableMapping.h"

namespace DocFileFormat
{
	class MainDocumentMapping : public DocumentMapping
	{
	public:
		MainDocumentMapping (ConversionContext* ctx, const ProgressCallback* ffCallBack);
		virtual ~MainDocumentMapping();
		virtual void Apply (IVisitable* visited);

	private:

		const ProgressCallback* m_ffCallBack;
	};
}