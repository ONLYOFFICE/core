#pragma once
#ifndef OOX_WORKBOOK_FILE_INCLUDE_H_
#define OOX_WORKBOOK_FILE_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "FileTypes.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CWorkbook : public OOX::File, public IFileContainer
		{
		public:
			CWorkbook()
			{
			}
			CWorkbook(const CPath& oPath)
			{
			}
			virtual ~CWorkbook()
			{
			}
		public:

			virtual void read(const CPath& oPath)
			{
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Workbook;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
		public:

			void ClearItems()
			{
			}

		private:
			CPath									m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:conformance"), m_oConformance )
					WritingElement_ReadAttributes_End( oReader )
			}

		public:

			CSimpleArray<WritingElement *>         m_arrItems;

		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_WORKBOOK_FILE_INCLUDE_H_