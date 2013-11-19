#include "stdafx.h"
#include "./../Converter.h"
#include "nullable_property.h"
#include "OdtFormat/Content/CoveredTableCell.h"

namespace Odp2DrawingXML
{
	void Converter::TableConvert(const Odt::Content::Table& pTable, CElementsContainer& pEContainer, const Odt::Logic::Properties& ParentProps, long x, long y, bool master)const
	{
		long xt = x;
		long yt = y;

		nullable_property<Odp::Styles::TableTemplate> TableStyle;
		if(pTable.TemplateName.is_init())
			TableStyle = m_Folder->GetTableTemplate(pTable.TemplateName.get());

		size_t size = pTable.Rows->size() * pTable.Columns->size();
		Odt::Logic::Properties* PropsTable = new Odt::Logic::Properties [size];
		for(size_t i = 0; i < pTable.Rows->size(); i++)
		{
			bool UseStyle = false;
			Odt::Logic::Properties RowProps = m_Folder->GetPropertiesFromStyle(pTable.Rows->at(i).StyleName.get_value_or(""), master);
			RowProps = Odt::Logic::Properties::merge(ParentProps, RowProps);
			if(TableStyle.is_init())
			{
				if((i == 0) && (TableStyle->FirstRow.is_init()) && (pTable.UseFirstRowStyles.get_value_or(false)))
				{
					Odt::Logic::Properties FirstRow = m_Folder->GetPropertiesFromStyle(TableStyle->FirstRow.get(), true);
					RowProps = Odt::Logic::Properties::merge(RowProps, FirstRow);
					UseStyle = true;
				}
				else if((i == pTable.Rows->size()-1) && (TableStyle->LastRow.is_init()) && (pTable.UseLastRowStyles.get_value_or(false)))
				{
					Odt::Logic::Properties LastRow = m_Folder->GetPropertiesFromStyle(TableStyle->LastRow.get(), true);
					RowProps = Odt::Logic::Properties::merge(RowProps, LastRow);
					UseStyle = true;
				}
				else if(((i % 2) == 1) && (TableStyle->OddRows.is_init()) && (pTable.UseBandingRowsStyles.get_value_or(false)))
				{
					Odt::Logic::Properties OddRow = m_Folder->GetPropertiesFromStyle(TableStyle->OddRows.get(), true);
					RowProps = Odt::Logic::Properties::merge(RowProps, OddRow);
					UseStyle = true;
				}
				//else if(TableStyle->Body.is_init())
				//{
				//}
			}

			for(size_t j = 0; j < pTable.Columns->size(); j++)
			{
				Odt::Logic::Properties ColProps = m_Folder->GetPropertiesFromStyle(pTable.Columns->at(j).Style.get(), master);
				ColProps = Odt::Logic::Properties::merge(RowProps, ColProps);
				if((TableStyle.is_init()) && (!UseStyle))
				{
					if((j == 0) && (TableStyle->FirstColumn.is_init()) && (pTable.UseFirstColumnStyles.get_value_or(false)))
					{
						Odt::Logic::Properties FirstColumn = m_Folder->GetPropertiesFromStyle(TableStyle->FirstColumn.get(), true);
						ColProps = Odt::Logic::Properties::merge(ColProps, FirstColumn);
					}
					else if((j == pTable.Columns->size()-1) && (TableStyle->LastColumn.is_init()) &&  (pTable.UseLastColumnStyles.get_value_or(false)))
					{
						Odt::Logic::Properties LastColumn = m_Folder->GetPropertiesFromStyle(TableStyle->LastColumn.get(), true);
						ColProps = Odt::Logic::Properties::merge(ColProps, LastColumn);
					}
					else if(((j % 2) == 1) && (TableStyle->OddColumns.is_init()) && (pTable.UseBandingColumnsStyles.get_value_or(false)))
					{
						Odt::Logic::Properties OddColumn = m_Folder->GetPropertiesFromStyle(TableStyle->OddColumns.get(), true);
						ColProps = Odt::Logic::Properties::merge(ColProps, OddColumn);
					}
					else if(TableStyle->Body.is_init())
					{
						Odt::Logic::Properties Body = m_Folder->GetPropertiesFromStyle(TableStyle->Body.get(), true);
						ColProps = Odt::Logic::Properties::merge(ColProps, Body);
					}
				}

				PropsTable[i * pTable.Columns->size() + j] = ColProps;
			}
		}

		//Odt::Logic::Properties properties = ParentProps;
		for(size_t i = 0; i < pTable.Rows->size(); i++)
		{
			for(size_t j = 0; j < pTable.Rows->at(i).Cells->size(); j++)
			{
				if(pTable.Rows->at(i).Cells->at(j).is<Odt::Content::TableCell>())
				{
					long numCol = pTable.Rows->at(i).Cells->at(j).getNumCol();
					Odt::Logic::Properties DefCellProps = m_Folder->GetPropertiesFromStyle(pTable.Rows->at(i).DefaultCellStyleName.get_value_or(""), master);
					DefCellProps = Odt::Logic::Properties::merge(PropsTable[i * pTable.Columns->size() + numCol] , DefCellProps);
					if(pTable.Rows->at(i).Cells->at(j).as<Odt::Content::TableCell>().Style.is_init())
					{
						Odt::Logic::Properties CellProps = m_Folder->GetPropertiesFromStyle(pTable.Rows->at(i).Cells->at(j).as<Odt::Content::TableCell>().Style.get(), master);
						DefCellProps = Odt::Logic::Properties::merge(DefCellProps, CellProps);
					}

					long w = (long)(PropsTable[i * pTable.Columns->size() + numCol].TableColumn->Width->value() * 10);
					long h = (long)(PropsTable[i * pTable.Columns->size() + numCol].TableRow->Height->value() * 10);
					for(size_t k = 1; k < pTable.Rows->at(i).Cells->at(j).as<Odt::Content::TableCell>().ColumnsSpanned.get_value_or(0); k++)
					{
						w += (long)(PropsTable[i * pTable.Columns->size() + numCol + k].TableColumn->Width->value() * 10);
					}
					for(size_t k = 1; k < pTable.Rows->at(i).Cells->at(j).as<Odt::Content::TableCell>().RowsSpanned.get_value_or(0); k++)
					{
						h += (long)(PropsTable[(i + k) * pTable.Columns->size() + numCol].TableRow->Height->value() * 10);
					}

					TableCellConvert(pTable.Rows->at(i).Cells->at(j).as<Odt::Content::TableCell>(), pEContainer, DefCellProps, xt, yt, w, h, master);
					xt += w;
				}
				else if(pTable.Rows->at(i).Cells->at(j).is<Odt::Content::CoveredTableCell>())
				{
					long numCol = pTable.Rows->at(i).Cells->at(j).getNumCol();

					long w = (long)(PropsTable[i * pTable.Columns->size() + numCol].TableColumn->Width->value() * 10);
					for(size_t k = 1; k < pTable.Rows->at(i).Cells->at(j).as<Odt::Content::CoveredTableCell>().CountHMerge.get(); k++)
					{
						w += (long)(PropsTable[i * pTable.Columns->size() + numCol - k + 1].TableColumn->Width->value() * 10);
					}

					xt += w;
				}
			}
			xt = x;
			yt += (long)(PropsTable[i * pTable.Columns->size()].TableRow->Height->value() * 10);
		}
	}

	void Converter::TableCellConvert(const Odt::Content::TableCell& pTableCell, CElementsContainer& pEContainer, const Odt::Logic::Properties& ParentProps, long x, long y, long w, long h, bool master)const
	{
		CShapeElement* pShape = new CShapeElement(NSBaseShape::odp, (int)OdpShapes::sptCRect);

		pShape->m_dStartTime = 0;
		pShape->m_dEndTime = pEContainer.m_dDuration;

		pShape->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		pShape->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight;
/*
		pShape->m_oShape.m_oBrush.m_Alpha1 = 255;
		pShape->m_oShape.m_oBrush.m_Alpha2 = 255;
		pShape->m_oShape.m_oPen.m_nAlpha = 255;
*/
		pShape->m_rcBoundsOriginal.left = x;
		pShape->m_rcBoundsOriginal.top = y;
		pShape->m_rcBoundsOriginal.right = x + w;//(long)(ParentProps.TableColumn->Width->value() * 10);
		//pShape->m_rcBoundsOriginal.right += pShape->m_rcBoundsOriginal.left;
		pShape->m_rcBoundsOriginal.bottom = y + h;//(long)(ParentProps.TableRow->Height->value() * 10);
		//pShape->m_rcBoundsOriginal.bottom += pShape->m_rcBoundsOriginal.top;

		FillPenBrush(ParentProps, *pShape);

		TextVectorConvert(pTableCell.Texts.get(), pShape->m_oShape.m_oText, ParentProps, master);
/*
		double AutoFit = pShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
		TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, AutoFit);
*/
		pShape->m_oShape.SetProperties(NULL, &pEContainer);
		pEContainer.m_arElements.Add(pShape);
	}
}