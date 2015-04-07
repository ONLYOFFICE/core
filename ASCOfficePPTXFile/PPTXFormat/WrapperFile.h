#pragma once
#ifndef PPTX_WRAPPER_FILE_INCLUDE_H_
#define PPTX_WRAPPER_FILE_INCLUDE_H_

#include "DocxFormat/File.h"
#include "DocxFormat/NamespaceOwn.h"
#include "FileMap.h"

#include "../Editor/BinWriters.h"

namespace PPTX
{
	class WrapperFile : public PPTX::File
	{
	public:
		WrapperFile()
		{
			m_written = false;
			m_WrittenFileName = _T("");
		}
		virtual ~WrapperFile()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename)
		{
			return;
		}
		virtual void read(const OOX::CPath&, FileMap& map) = 0;

	protected:
		mutable bool			m_written;
		mutable OOX::CPath		m_WrittenFileName;
	public:
		bool GetWrittenStatus()const
		{
			return m_written;
		}
		void WrittenSetFalse()
		{
			m_written = false;
			m_WrittenFileName = _T("");
		}
		const OOX::CPath GetWrittenFileName()const
		{
			return m_WrittenFileName;
		}

		// PPTY format serialise
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->SkipRecord();
		}
        virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
		}
	};
} // namespace PPTX

#endif // PPTX_WRAPPER_FILE_INCLUDE_H_