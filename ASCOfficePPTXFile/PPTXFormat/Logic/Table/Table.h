#pragma once
#ifndef PPTX_LOGIC_TABLE_INCLUDE_H_
#define PPTX_LOGIC_TABLE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "TableCol.h"
#include "TableRow.h"
#include "TableProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class Table : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Table)

			Table& operator=(const Table& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				TableCols.Copy(oSrc.TableCols);
				TableRows.Copy(oSrc.TableRows);

				TableProperties = oSrc.TableProperties;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:tblGrid"), oNode))
					oNode.LoadArray(_T("a:gridCol"), TableCols);

				node.LoadArray(_T("a:tr"), TableRows);

				TableProperties = node.ReadNode(_T("a:tblPr"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(TableProperties);
				oValue.WriteArray(TableRows);
				oValue.WriteArray(_T("a:tblGrid"), TableCols);

				return XmlUtils::CreateNode(_T("a:tbl"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, TableProperties);
				pWriter->WriteRecordArray(1, 0, TableCols);
				pWriter->WriteRecordArray(2, 0, TableRows);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							TableProperties = new Logic::TableProperties();
							TableProperties->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							pReader->Skip(4);
							LONG lCount = pReader->GetLong();
							for (LONG i = 0; i < lCount; ++i)
							{
								pReader->Skip(1);
								TableCols.Add();
								TableCols[i].fromPPTY(pReader);
							}
							break;
						}
						case 2:
						{
							pReader->Skip(4);
							LONG lCount = pReader->GetLong();
							for (LONG i = 0; i < lCount; ++i)
							{
								pReader->Skip(1);
								TableRows.Add();
								TableRows[i].fromPPTY(pReader);
							}
						}
						default:
						{
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tbl"));
				pWriter->EndAttributes();

				pWriter->Write(TableProperties);
				
				pWriter->WriteString(_T("<a:tblGrid>"));
				size_t n1 = TableCols.GetCount();
				for (size_t i = 0; i < n1; ++i)
					TableCols[i].toXmlWriter(pWriter);
				pWriter->WriteString(_T("</a:tblGrid>"));

				size_t n2 = TableRows.GetCount();
				for (size_t i = 0; i < n2; ++i)
					TableRows[i].toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:tbl"));
			}
			
		public:
			CAtlArray<TableCol>			TableCols;
			CAtlArray<TableRow>			TableRows;
			nullable<TableProperties>	TableProperties;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if (TableProperties.IsInit())
					TableProperties->SetParentPointer(this);
				
				size_t count = TableRows.GetCount();
				for (size_t i = 0; i < count; ++i)
					TableRows[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_INCLUDE_H_