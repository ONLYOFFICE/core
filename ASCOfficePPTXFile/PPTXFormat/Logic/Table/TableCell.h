#pragma once
#ifndef PPTX_LOGIC_TABLE_CELL_INCLUDE_H_
#define PPTX_LOGIC_TABLE_CELL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../TxBody.h"
#include "TableCellProperties.h"
#include "../ShapeProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCell : public WrapperWritingElement
		{
		public:
			TableCell();
			virtual ~TableCell();			
			explicit TableCell(XmlUtils::CXmlNode& node);
			const TableCell& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;

			virtual void GetShapeProperties(ShapeProperties& props)const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tc"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("rowSpan"), RowSpan);
				pWriter->WriteAttribute(_T("gridSpan"), GridSpan);
				pWriter->WriteAttribute(_T("hMerge"), HMerge);
				pWriter->WriteAttribute(_T("vMerge"), VMerge);
				pWriter->WriteAttribute(_T("id"), Id);

				pWriter->EndAttributes();

                pWriter->Write(txBody);
				pWriter->Write(CellProperties);				

				pWriter->EndNode(_T("a:tc"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, Id);
				pWriter->WriteInt2(1, RowSpan);
				pWriter->WriteInt2(2, GridSpan);
				pWriter->WriteBool2(3, HMerge);
				pWriter->WriteBool2(4, VMerge);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, CellProperties);
                pWriter->WriteRecord2(1, txBody);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							Id = pReader->GetString2();
							break;
						}
						case 1:
						{
							RowSpan = pReader->GetLong();
							break;
						}
						case 2:
						{
							GridSpan = pReader->GetLong();
							break;
						}
						case 3:
						{
							HMerge = pReader->GetBool();
							break;
						}
						case 4:
						{
							VMerge = pReader->GetBool();
							break;
						}
						default:
							break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							CellProperties = new TableCellProperties();
							CellProperties->fromPPTY(pReader);
							break;
						}
						case 1:
						{
                            txBody = new Logic::TxBody();
                            txBody->fromPPTY(pReader);
                            txBody->m_ns = _T("a");
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
            nullable<TxBody>				txBody;
			nullable<TableCellProperties>	CellProperties;
			nullable_int					RowSpan;
			nullable_int					GridSpan;
			nullable_bool					HMerge;
			nullable_bool					VMerge;
			nullable_string					Id;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_CELL_INCLUDE_H_
