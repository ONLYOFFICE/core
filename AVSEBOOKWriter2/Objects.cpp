#include "stdafx.h"
#include "Logic/LogicPagePDF.h"
#include "Logic/EbookPage.h"
#include <iostream>
#include <fstream>
						Object::Object(){}
						Object::~Object(){}
Object::ObjectType		Object::getType()
				{
					return type;
				}
void					Object::coord(Coordinates &t_coord)
{
	x(t_coord.x());
	y(t_coord.y());
}

Coordinates*			Object::coord()
{
	Coordinates *t_coord =  new Coordinates(x(), y());
	return t_coord;
}

void					Object::size(Size &t_size)
{
	setSize(&t_size);
}

Size*					Object::size() const
{
	Size *t_size =  new Size(m_width, m_height);
	return t_size;
}

							SymbolObject::SymbolObject()
{
	type = Object::unknown;
}

void						SymbolObject::setType(ElementTypes t_type)
{
	elementType = t_type;
}

SymbolObject::ElementTypes	SymbolObject::getElementType()
{
	return elementType;
}

void						SymbolObject::setText(wchar_t *t_symbol)
{
	symbol = *t_symbol;
}

void						SymbolObject::code(wchar_t *code)
{
	m_code = *code;
}

wchar_t						SymbolObject::code()
{
	return code;
}

wchar_t						SymbolObject::getText() const
{
	return symbol;
}

void			SymbolLine::baseLine(double t_baseLine)
{
	m_baseLine = t_baseLine;
}

double			SymbolLine::baseLine()
{
	return m_baseLine;
}

				SymbolLine::SymbolLine()
{
	type = Object::unknown;
}

				SymbolLine::SymbolLine(SymbolLine &line)
{
	type = Object::unknown;
	copy(&line);
}

SymbolLine*		SymbolLine::split(double t_x)
{
	bool flag = true;
	Iterator<SymbolObject*> *iter= getIterator();
	while(flag && iter->getID() < count)
	{
		if(t_x <= iter->item()->x())
		{
			flag = false;
		}
		iter->next();
	}
	long pos = iter->getID();
	delete iter;
	return split(pos);
}

SymbolLine*		SymbolLine::split(long position)
{
	SymbolLine *p_line = new SymbolLine;
	while(position > 0)
	{
		p_line->pushBack(pop());
		position--;
	}
	resize();
	p_line->resize();
	return p_line;
}

void			SymbolLine::resize()
{
	if(count > 0)
	{
		x(head->item->x());
		y(head->item->y());
		double temp = 0.0;
		Iterator<SymbolObject*> *iter = getIterator();
		//while(iter->getID() < count)
		//{
		//	temp += iter->item()->width();
		//	iter->next();
		//}
		//delete iter;
		iter->last();
		width(iter->item()->x() + iter->item()->width() - m_x);
	}
}

			LineGroup::LineGroup()
{
	type = Object::group;
	flag = true;
	m_align = unknown;
}

			LineGroup::~LineGroup()
{
}

int			LineGroup::align()
{
	return m_align;
}

void		LineGroup::align(LineGroup::Align t_a)
{
	m_align = t_a;
}

double		LineGroup::space()
{
	return commonVerticalSpace;
}

void		LineGroup::space(double t_s)
{
	commonVerticalSpace = t_s;
}

LineGroup*	LineGroup::split(bool orient, double x, double y)
{
	LineGroup *p_group = new LineGroup();
	Iterator<SymbolLine*> *iter = getIterator();
	if(orient) //vertical
	{
		while(iter->getID() < count)
		{
			p_group->pushBack(iter->item()->split(x));
			iter->next();
		}
	}
	else
	{
		//while(iter->getID() < count)
		//{
			while(y > iter->item()->y() + iter->item()->height())
			{
				//while(iter->getID() < count)
				//{
					p_group->pushBack(iter->popItem());
				//}
			//}
			//iter->next();
		}
	}
	resize();
	p_group->resize();
	delete iter;
	return p_group;
}

void		LineGroup::merge(LineGroup* p_group)
{
	if(p_group)
	{
		LineGroup *up, *down;
		if(p_group->first()->y() > first()->y())
		{
			up = this;
			down = p_group;
		}
		else
		{
			up = p_group;
			down = this;
		}
		if(up && down)
		{
			while(down->getCount() > 0)
			{
				up->pushBack(down->pop());
			}
			up->resize();
		}
	}
}

void		LineGroup::resize()
{
	Iterator<SymbolLine*> *iter = getIterator();
	double t_width = 0.0, t_height = 0.0, t_x = 10000.0;
	while(iter->getID() < count)
	{
		iter->item()->resize();
		if(t_x > iter->item()->x())
			t_x = iter->item()->x();
		t_height += iter->item()->height();
		if(t_width < iter->item()->width())
			t_width = iter->item()->width();
		iter->next();
	}
	x(t_x);
	y(head->item->y());
	setSize(t_width, t_height);
}

		Cell::Cell(long t_rowID, long t_columnID, Object *p_content)
{
	merged = false;
	borders[0] = true;
	borders[1] = true;
	borders[2] = true;
	borders[3] = true;
	m_rowID = t_rowID;
	m_columnID = t_columnID;
	m_content = p_content;
}

		Cell::Cell()
{
	merged = false;
	m_rowID = 0;
	m_columnID = 0;
	mergeXcount = 0;
	mergeYcount = 0;
	borders[0] = true;
	borders[1] = true;
	borders[2] = true;
	borders[3] = true;
	m_content = NULL;
}

void	Cell::addCell(int rid, int cid)
{
	if(rid == m_rowID && abs(m_columnID + mergeXcount - cid) == 1)
		mergeXcount++;
	else
		if(abs(mergeYcount + m_rowID - rid) == 1 && cid == m_columnID)
			mergeYcount++;
}

int		Cell::cellCountX()
{
	return mergeXcount;
}

int		Cell::cellCountY()
{
	return mergeYcount;
}

bool	Cell::setBordersVisability(int number, bool visible)
{
	if(number < 4)
	{
		borders[number] = visible;
		return true;
	}
	return false;
}

void	Cell::content(Object *t_object)
{
	m_content = t_object;
	//setSize(t_object->width(), t_object->height());
}

Object* Cell::content() const
{
	return m_content;
}

void	Cell::resize()
{
	if(content != NULL)
		setSize(m_content->size());
}

void	Cell::rowID(long t_id)
{
	m_rowID = t_id;
}

long	Cell::rowID()
{
	return m_rowID;
}

void	Cell::columnID(long t_id)
{
	m_columnID = t_id;
}

long	Cell::columnID()
{
	return m_columnID;
}

Cell*	Cell::split(bool orient, double &t_x, double &t_y)
{
	Cell *p_cell = new Cell;
	p_cell->content(m_content->split(orient,t_x,t_y));
	if(orient)//vertical
	{
		p_cell->x(x());
		p_cell->y(y());
		p_cell->resize();
		x(x() + p_cell->width());
		width(width()-p_cell->width());
		p_cell->height(height());
	}
	else
	{
		p_cell->width(width());
		resize();
		p_cell->x(x());
		p_cell->y(y() + height());
		width(p_cell->width());
	}
	return p_cell;
}

						Table2::Table2()
{
	type = Object::table;
	rows = new List<List<Cell*>*>;
	m_columnCount = 0;
}

						Table2::Table2(long rowCount, long columnCount)
{
	type = Object::table;
	m_columnCount = columnCount;
	rows = new List<List<Cell*>*>;
	while(rowCount-- > 0)
	{
		List<Cell*>* cellList = new List<Cell*>;
		long temp = columnCount;
		while(temp-- > 0)
		{
			Cell *cell = new Cell;
			cell->rowID(rowCount);
			cell->columnID(temp);
			cellList->push(cell);
		}
		rows->push(cellList);
	}
}

						Table2::~Table2()
{
	rows->clear();
	delete rows;
}

List<Cell*>*			Table2::getRow(long id)
{
	return rows->getAt(id);
}

Iterator<List<Cell*>*>* Table2::getRowIterator()
{
	return rows->getIterator();
}

void					Table2::addRow()
{
	rows->pushBack(new List<Cell*>);
}

void					Table2::pushRow(List<Cell*>* row)
{
	rows->pushBack(row);
}

bool					Table2::empty()
{
	if(rows->getCount() > 0)
		if(rows->first()->getCount() > 0)
			return false;
	return true;
}

long					Table2::getCount()
{
	return rows->getCount();
}

long					Table2::columnCount()
{
	return m_columnCount;
}

void					Table2::columnCount(long t_count)
{
	if(t_count > m_columnCount)
		m_columnCount = t_count;
}

void					Table2::resize()
{
	m_x = rows->first()->first()->x();
	m_y = rows->first()->first()->y();
	m_width = m_x + rows->last()->last()->x() + rows->last()->last()->width();
	m_height = m_y + rows->last()->last()->y() + rows->last()->last()->height();
}

void		Image::setImageInfo(NSCommon::CImageInfo p_info)
{
		info = p_info;
}

CImageInfo	Image::getImageInfo() const
{
	return info;
}

void		GraphicLine::endPoint(Coordinates t_endPoint)
{
	m_endPoint = t_endPoint;
}

Coordinates GraphicLine::endPoint()
{
	return m_endPoint;
}

void		GraphicLine::setOrientationHint(int t_orient)
{
	m_orient = t_orient;
}

void Page::createCommonList()
{
	items = new List<Object*>;
	Iterator<LineGroup*> *groupIterator = groups->getIterator();
	//items->push(groupIterator->popItem());
	Iterator<Object*> *itemIterator = items->getIterator();
	while(groups->getCount() > 0)
	{
		if(items->getCount())
		{
			if(groupIterator->item()->y() >= items->last()->y())
			{
				items->pushBack(groupIterator->popItem());
			}
			else
			{
				itemIterator->first();
				bool stop = false;
				while(!stop && itemIterator->getID() < items->getCount())
				{
					if(groupIterator->item()->y() < itemIterator->item()->y())
					{
						stop = true;
						items->insert(itemIterator->getID() - 1, groupIterator->popItem());
					}
					itemIterator->next();
				}
			}
		}
		else
			items->push(groupIterator->popItem());
	}
	delete groupIterator;
	Iterator<GroupList*> *listIterator = lists->getIterator();
	while(lists->getCount() > 0)
	{
		if(items->getCount())
		{
			if(listIterator->item()->y() > items->last()->y())
			{
				items->pushBack(listIterator->popItem());
			}
			else
			{
				itemIterator->first();
				bool stop = false;
				while(!stop && itemIterator->getID() < items->getCount())
				{
					if(listIterator->item()->y() < itemIterator->item()->y())
					{
						stop = true;
						items->insert(itemIterator->getID() - 1, listIterator->popItem());
					}
					itemIterator->next();
				}
			}
		}
		else
			items->push(listIterator->popItem());
	}
	delete listIterator;
	Iterator<Table2*> *tableIterator = tables->getIterator();
	while(tables->getCount() > 0)
	{
		if(items->getCount())
		{
			if(tableIterator->item()->y() > items->last()->y())
			{
				items->pushBack(tableIterator->popItem());
			}
			else
			{
				itemIterator->first();
				bool stop = false;
				while(!stop && itemIterator->getID() < items->getCount())
				{
					if(tableIterator->item()->y() < itemIterator->item()->y())
					{
						stop = true;
						items->insert(itemIterator->getID(), tableIterator->popItem());
					}
					itemIterator->next();
				}
			}
		}
		else
			items->push(tableIterator->popItem());
	}
	delete tableIterator;
	Iterator<Image*> *imageIterator = images->getIterator();
	while(images->getCount() > 0)
	{
		if(items->getCount())
		{
			if(imageIterator->item()->y() > items->last()->y())
			{
				items->pushBack(imageIterator->popItem());
			}
			else
			{
				itemIterator->first();
				bool stop = false;
				while(!stop && itemIterator->getID() < items->getCount())
				{
					if(imageIterator->item()->y() < itemIterator->item()->y())
					{
						stop = true;
						items->insert(itemIterator->getID() - 1, imageIterator->popItem());
					}
					itemIterator->next();
				}
			}
		}
		else
			items->push(imageIterator->popItem());
	}
	delete imageIterator;
}

void							Converter::convert(CFormat *format, CTextPDF *pTextPDF, Page *page)
{
	textPDF = pTextPDF;
	Iterator<Object*> *itemIterator = page->items->getIterator();
	format->m_listSections.AddTail();
	CSection* pSection = &format->m_listSections.GetTail();
	while(itemIterator->getID() < page->items->getCount())
	{
		switch(itemIterator->item()->getType())
		{
			case Object::group:
				pSection->m_arItems.AddTail(convertParagraph(itemIterator->item()));
				break;
			case Object::image:
				pSection->m_arItems.AddTail(convertImage(itemIterator->item()));
				break;
			case Object::list:
				pSection->m_arItems.AddTail(convertList(itemIterator->item()));
				break;
			case Object::table:
				pSection->m_arItems.AddTail(convertTable(itemIterator->item()));
				break;
			case Object::unknown:
				break;
		}
		itemIterator->next();
	}
	delete itemIterator;
}

NSEBook::CEbookNumbering*		Converter::convertList(Object *p_list)
{
	GroupList *list = dynamic_cast<GroupList*>(p_list);
	CEbookNumbering *numbering = new CEbookNumbering;
	Iterator<LineGroup*> *groupIterator = list->getIterator();
	while(groupIterator->getID() < list->getCount())
	{
		numbering->m_arParagraphs.Add(*convertParagraph(groupIterator->item()));
		groupIterator->next();
	}
	delete groupIterator;
	return numbering;
}

NSEBook::CEbookTable*			Converter::convertTable(Object *p_table)
{
	Table2 *table = dynamic_cast<Table2*>(p_table);
	CEbookTable* eTable = new CEbookTable;
	Iterator<List<Cell*>*> *rowIterator = table->getRowIterator();
	eTable->m_strTableXml += _T("<style type=\"text/css\">table{border-collapse:collapse;} td, th{border:1px solid #000000;}</style><table>");
	while(rowIterator->getID() < table->getCount())
	{
		eTable->m_strTableXml += _T("<tr>");
		Iterator<Cell*> *cellIterator = rowIterator->item()->getIterator();
		while(cellIterator->getID() < rowIterator->item()->getCount())
		{
			Cell* cell = cellIterator->item();
			if(!cell->merged)
			{
				eTable->m_strTableXml += _T("<td");
				if(cell->cellCountX())
				{
					CString str = _T("");
					str.Format(_T("%i"),cell->cellCountX() + 1);
					eTable->m_strTableXml += _T(" colspan=\"") + str + _T("\"");
				}
				if(cell->cellCountY())
				{
					CString str= _T("");
					str.Format(_T("%i"),cell->cellCountY() + 1);
					eTable->m_strTableXml += _T(" rowspan=\"") + str + _T("\"");
				}
				eTable->m_strTableXml += _T(">");
				if(cell->content())
				{
					Object* obj= cell->content();
					switch(obj->getType())
					{
						case Object::group:
						{
							LineGroup* group = (LineGroup*)(obj);
							if(group)
							{
								Iterator<SymbolLine*> *lineIterator = group->getIterator();
								while(lineIterator->getID() < group->getCount())
								{
									Iterator<SymbolObject*> *symbolIterator = lineIterator->item()->getIterator();
									while(symbolIterator->getID() < lineIterator->item()->getCount())
									{
										if(symbolIterator->item()->getElementType() == SymbolObject::word)
										{
											Word *word = dynamic_cast<Word*>(symbolIterator->item());
											wchar_t *temp = new wchar_t[word->getCount() + 1];
											int i = 0;
											Iterator<SymbolObject*> *wordSymbolIterator = word->getIterator();
											while(wordSymbolIterator->getID() < word->getCount())
											{
												temp[i++] = wordSymbolIterator->item()->getText();
												wordSymbolIterator->next();
											}
											temp[i] = L'\0';
											BSTR test = SysAllocString(temp);
											eTable->m_strTableXml += test;
											delete temp;
											delete wordSymbolIterator;
										}
										else
										{
											eTable->m_strTableXml += symbolIterator->item()->getText();
										}
										symbolIterator->next();
									}
									delete symbolIterator;
									lineIterator->next();
								}
								delete lineIterator;
							}
							break;
						}
						case Object::image:
						{
							//(convertImage(obj));
							break;
						}
						case Object::list:
						{
							//(convertList(obj);
							break;
						}
						case Object::table:
						{
							//(convertTable(obj);
							break;
						}
						case Object::unknown:
							break;
					}
				}
				eTable->m_strTableXml += _T("</td>");
			}
			cellIterator->next();
		}
		eTable->m_strTableXml += _T("</tr>");
		delete cellIterator;
		rowIterator->next();
	}
	delete rowIterator;
	eTable->m_strTableXml += _T("</table>");
	return eTable;
}

NSEBook::CEbookParagraph*		Converter::convertParagraph(Object* group)
{// I was really sorry for this
	LineGroup *lGroup = dynamic_cast<LineGroup*>(group);
	CEbookParagraph *paragraph = new CEbookParagraph;
	CAtlList<CEbookParagraph::CSpan> *spans = new CAtlList<CEbookParagraph::CSpan>;
	Iterator<SymbolLine*> *lineIterator = lGroup->getIterator();
	while(lineIterator->getID() < lGroup->getCount())
	{
		CEbookParagraph::CLine *line = new CEbookParagraph::CLine;
		Iterator<SymbolObject*> *objectIterator = lineIterator->item()->getIterator();
		while(objectIterator->getID() < lineIterator->item()->getCount())
		{
			BSTR text;
			BSTR code;
			double base = lineIterator->item()->baseLine();
			switch(objectIterator->item()->getElementType())
			{
				case SymbolObject::sign:
					{
						SymbolObject *obj = objectIterator->item();
						wchar_t *temp = new wchar_t[2];
						temp[0] = obj->getText();
						temp[1] = L'\0';
						text = SysAllocString(temp);
						temp[0] = obj->code();
						temp[1] = L'\0';
						code = SysAllocString(temp);
						delete temp;
						double x = obj->x();
						double y = obj->y();
						double width = obj->width();
						double height = obj->height();
						CLogicTextLine *logicLine = textPDF->CommandText(text, code, x, y, width, height, base);
						if(logicLine != NULL)
						{
							while(logicLine->m_arSpans.GetCount() > 0)
							{
								spans->AddTail(*(logicLine->m_arSpans.RemoveHead()));
							}
						}
						break;
					}
				case SymbolObject::space:
					{
						SymbolObject *obj = objectIterator->item();
						wchar_t *temp = new wchar_t[2];
						temp[0] = obj->getText();
						temp[1] = L'\0';
						text = SysAllocString(temp);
						temp[0] = obj->code();
						temp[1] = L'\0';
						code = SysAllocString(temp);
						delete temp;
						double x = obj->x();
						double y = obj->y();
						double width = obj->width();
						double height = obj->height();
						CLogicTextLine *logicLine = textPDF->CommandText(text, code, x, y, width, height, base);
						if(logicLine != NULL)
						{
							while(logicLine->m_arSpans.GetCount() > 0)
							{
								spans->AddTail(*(logicLine->m_arSpans.RemoveHead()));
							}
						}
						break;
					}
				case SymbolObject::word:
					{
						Word *word = dynamic_cast<Word*>(objectIterator->item());
						Iterator<SymbolObject*> *symbolIterator = word->getIterator();
						while(symbolIterator->getID() < word->getCount())
						{
							SymbolObject *obj = symbolIterator->item();
							wchar_t *temp = new wchar_t[2];
							temp[0] = obj->getText();
							temp[1] = L'\0';
							text = SysAllocString(temp);
							temp[0] = obj->code();
							temp[1] = L'\0';
							code = SysAllocString(temp);
							delete temp;
							double x = obj->x();
							double y = obj->y();
							double width = obj->width();
							double height = obj->height();
							CLogicTextLine *logicLine = textPDF->CommandText(text, code, x, y, width, height, base);
							if(logicLine != NULL)
							{
								while(logicLine->m_arSpans.GetCount() > 0)
								{
									spans->AddTail(*(logicLine->m_arSpans.RemoveHead()));
								}
							}
							symbolIterator->next();
						}
					}
			}
			objectIterator->next();
		}
		delete objectIterator;
		line->m_listSpans.AddTailList(spans);
		spans = new CAtlList<CEbookParagraph::CSpan>;
		line->m_dBaselinePos = lineIterator->item()->baseLine();
		paragraph->m_arLines.AddTail(*line);
		line = new CEbookParagraph::CLine;
		lineIterator->next();
	}
	delete lineIterator;
	return paragraph;
}

NSEBook::CEbookGraphicObject*	Converter::convertImage(Object *image)
{
	CEbookGraphicObject *obj = new CEbookGraphicObject;
	obj->m_oInfo = dynamic_cast<Image*>(image)->getImageInfo();
	obj->m_rcBounds.left = image->x();
	obj->m_rcBounds.top = image->y();
	obj->m_rcBounds.right = image->x() + image->width();
	obj->m_rcBounds.bottom = image->y() + image->height();
	return obj;
}

void	Parser::clear()
{
		//pages->clear();
		graphicLines->clear();
		lists->clear();
		groups->clear();
		symbolLines->clear();
		pointMap->rowList.clear();
		delete currentLine;
		delete currentWord;
		thereAreSpaces = false;
		currentYcoord = 0.0;
		minSymbolWidth = 100.0;
		maxSymbolWidth = 0.0;
		verticalLineSpace = 0.0;
		currentLine = new SymbolLine;
		currentWord = new Word;
		currentWord->setType(SymbolObject::word);
}

void	Parser::moveTo(double &x, double &y)
{
	coord.x(x);
	coord.y(y);
}

void	Parser::lineTo(double &x, double &y)
{
	GraphicLine *p_line = new GraphicLine(coord, Coordinates(x, y));
	if(abs(coord.x() - x) < 0.01)
	{
		if(coord.y() < y)
			p_line->setOrientationHint(270);
		else
			if(coord.y() > y)
				p_line->setOrientationHint(90);
	}
	else
	{
		if(abs(coord.y() - y) < 0.01)
		{
			if(coord.x() < x)
				p_line->setOrientationHint(0);
			else
				if(coord.x() > x)
					p_line->setOrientationHint(180);
		}
		else
			p_line->setOrientationHint(-1);
	}
	graphicLines->push(p_line);
}

void	Parser::text(BSTR &bsText, BSTR &bsGid, double &x, double &y, double &width, double &height, double &baselineoffset)
{
	if(minSymbolWidth > width)
		minSymbolWidth = width;
	if(maxSymbolWidth < width)
		maxSymbolWidth = width;
	SymbolObject *p_obj = new SymbolObject(bsText, bsGid);
	BSTR temp = bsText;
	p_obj->x(x);
	p_obj->y(y + baselineoffset);
	p_obj->width(width);
	p_obj->height(height);
	//---------- analyzing symbol object
	switch(p_obj->getText())
	{
		case(_T(' ')):
			if(!thereAreSpaces)
				thereAreSpaces = true;
			p_obj->setType(SymbolObject::space);
			break;
		case(_T('.')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T(',')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T(':')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T(';')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T('!')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T('?')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T('”')):
			p_obj->setType(SymbolObject::sign);
			break;
		case(_T('•')):
			p_obj->setType(SymbolObject::sign);
			break;
		default:
			//currentWord->pushBack(p_obj);
			break;
	}

	if(abs(currentYcoord - y) < 1)// && baselineoffset == currentLine->baseLine())// if we are on the same line
	{
			//-------- continue word or create a new one
			if(p_obj->getElementType() != SymbolObject::sign && p_obj->getElementType() != SymbolObject::space)
			{
				if(currentWord->getCount() > 0)
				{
					Iterator<SymbolObject*> *symbolIterator = currentWord->getIterator();
					symbolIterator->last();
					if(p_obj->x() - symbolIterator->item()->x() - symbolIterator->item()->width() > 0.5)
					{
						currentLine->pushBack(currentWord);
						currentLine->width(currentWord->x() - currentLine->x() + currentWord->width());
						if(currentLine->height() > currentWord->height())// decrement height
							currentLine->height(currentWord->height());
						currentWord = new Word;
						currentWord->setType(SymbolObject::word);
						currentWord->x(x);
						currentWord->y(y + baselineoffset);
					}
				}
				else
				{
					currentWord->x(x);
					currentWord->y(y + baselineoffset);
				}
				currentWord->pushBack(p_obj);
				currentWord->width(x + width - currentWord->x());// increment width
				if(currentWord->height() > height)// decrement height
					currentWord->height(height);
			}
			else
			{
				if(currentWord->getCount() > 0)
				{
					currentLine->pushBack(currentWord);
					currentWord = new Word;
					currentWord->setType(SymbolObject::word);
				}
				currentLine->pushBack(p_obj);
				currentLine->width(x - currentLine->x() + width);
			}
			//----------
	}
	else
	{
		if(currentWord->getCount() > 0)
		{
			currentLine->pushBack(currentWord);
			currentLine->width(currentWord->x() - currentLine->x() + currentWord->width());
			if(currentLine->height() > currentWord->height())// decrement height
				currentLine->height(currentWord->height());
			currentWord = new Word;
			currentWord->setType(SymbolObject::word);
		}
		if(currentLine->getCount() > 0)// it need for first symbol on a new line
		{
			if(maxLineWidth < currentLine->width())
				maxLineWidth = currentLine->width();
			symbolLines->pushBack(currentLine);
			currentLine = new SymbolLine;// new line
		}
		currentYcoord = y;
		currentLine->y(y + baselineoffset);
		currentLine->x(x);
		currentLine->baseLine(baselineoffset);

		if(p_obj->getElementType() != SymbolObject::sign && p_obj->getElementType() != SymbolObject::space)
		{
			currentWord->x(x);
			currentWord->y(y + baselineoffset);
			currentWord->pushBack(p_obj);
			currentWord->width(x + width - currentWord->x());// increment width
			if(currentWord->height() > height)// decrement height
				currentWord->height(height);
		}
		else
		{
			currentLine->pushBack(p_obj);
				currentLine->width(x - currentLine->x() + width);
		}
	}
}

void	Parser::image(CImageInfo p_info, double &x, double &y, double &width, double &height)
{
	Image *image = new Image(p_info);
	image->width(width);
	image->height(height);
	image->x(x);
	image->y(y);
	images->pushBack(image);
	//model->addObject(p_image);
}

void	Parser::beginPage()
{
}

void	Parser::setPixelBuffer(BYTE *p_buffer)
{
	buffer = p_buffer;
}

void	Parser::setPagePixelSize(long width, long height)
{
	pixelWidth = width;
	pixelHeight = height;
}

bool	Parser::analizeGraphic(long x, long y, long right, long bottom)
{
	bool isTable = true;// uber flag
	//Iterator<LineGroup*> *groupIterator = groups->getIterator();// uncomment following lines for parce only tables with text
	/*if(groups->getCount() > 0)
	{	
		int count = 0;
		while(!isTable && groupIterator->getID() < groups->getCount())
		{
			if(groupIterator->item()->x() >= 25.4 * x / 96.0 &&
				groupIterator->item()->width() <= 25.4 * (right - x + 1) / 96.0 &&
				groupIterator->item()->y() >= 25.4 * (y) / 96.0 &&
				groupIterator->item()->height() <= 25.4 * (bottom - y + 1) / 96.0)
				count++;
			if(count >= 3)// MAGIC NUMBER
				isTable = true;
			groupIterator->next();
		}
		delete groupIterator;
	}*/
	if(isTable)
		createGraphicLines(x, y, right, bottom, isTable);
///------------
	if(!isTable)
		return isTable;
///--------------------------------
	if(graphicLines->getCount() > 0)
	{
		findIntersections();
		//-----------------------
		if(points->getCount() > 1)
		{
			createPointMap();
			if(pointMap->rowList.getCount() > 1)
			{
				createTable();
			}
		}
	}
	return isTable;
}

void	Parser::endPage()
{
	if(currentWord->getCount() > 0)// finish word
	{
		currentLine->pushBack(currentWord);
		currentLine->width(currentWord->x() - currentLine->x() + currentWord->width());
		if(currentLine->height() > currentWord->height())
			currentLine->height(currentWord->height());
		currentWord = new Word;
		currentWord->setType(SymbolObject::word);
	}
	if(currentLine->getCount() > 0)// finish line
	{
		if(maxLineWidth < currentLine->width())
			maxLineWidth = currentLine->width();
		symbolLines->pushBack(currentLine);
		currentLine = new SymbolLine;
	}
	///---------------- looking for common vertical empty space 
	Iterator<SymbolLine*> *baseLineIterator = symbolLines->getIterator();
	Iterator<SymbolLine*> *nextLineIterator = symbolLines->getIterator();
	nextLineIterator->next();
	List<Space*> *spaces = new List<Space*>;
	Iterator<Space*> *spaceIterator= spaces->getIterator();
	double currentVerticalSpace = 0.0;

	while(nextLineIterator->getID() < symbolLines->getCount())
	{
		currentVerticalSpace = nextLineIterator->item()->y() - baseLineIterator->item()->y() - baseLineIterator->item()->baseLine();
		bool flag = true;
		while(flag && spaceIterator->getID() < spaces->getCount())
		{
			if(abs(currentVerticalSpace - spaceIterator->item()->space) < 0.5)
			{
				spaceIterator->item()->count++;
				flag = false;
			}
			spaceIterator->next();
		}
		if(flag)
		{
			Space *space = new Space;
			space->space = currentVerticalSpace;
			space->count = 0;
			spaces->pushBack(space);
		}
		spaceIterator->first();
		baseLineIterator->next();
		nextLineIterator->next();
	}

	spaceIterator->first();
	delete baseLineIterator;
	delete nextLineIterator;

	double commonVerticalSpace = 0;
	long maxCount = 0;
	while(spaceIterator->getID() < spaces->getCount())// looking for most common space
	{
		if(maxCount < spaceIterator->item()->count)
		{
			maxCount = spaceIterator->item()->count;
			commonVerticalSpace = spaceIterator->item()->space;
		}
		spaceIterator->next();
	}
	delete spaceIterator;
	verticalLineSpace = commonVerticalSpace;
	///--------------------------- group lines
	if(symbolLines->getCount() > 0)
	{
		Iterator<SymbolLine*> *lineIterator = symbolLines->getIterator();
		while(lineIterator->getID() < symbolLines->getCount())
		{
			if(lineIterator->item()->getCount() == 1 && lineIterator->item()->first()->getType() == SymbolObject::space)
			{
				lineIterator->next();
			}
			else
			{
				Iterator<SymbolObject*> *symbolIterator = lineIterator->item()->getIterator();
				while(symbolIterator->getID() < lineIterator->item()->getCount())
				{
					double x = symbolIterator->item()->x();
					double width = symbolIterator->item()->width();
					SymbolObject::ElementTypes type = symbolIterator->item()->getElementType();
					symbolIterator->next();
					if(symbolIterator->item()->x() - x - width >= 0)
					{
						if(thereAreSpaces)
						{
							if(symbolIterator->item()->x() - x - width >= minSymbolWidth)
							{
								if(symbolIterator->item()->getElementType() == type && (type == SymbolObject::word || type == SymbolObject::sign))
									findGroup(lineIterator->item()->split(x));
							}
						}
						else
						{
							if(symbolIterator->item()->x() - x - width >= maxSymbolWidth)
							{
								if(symbolIterator->item()->getElementType() == type && type == SymbolObject::word)
									findGroup(lineIterator->item()->split(x));
							}
							else
							{
								/*if((symbolIterator->item()->getType() == type && type == SymbolObject::word) || (symbolIterator->item()->getType() == SymbolObject::word && type == SymbolObject::sign))
								{
									SymbolObject *sp = new SymbolObject;
									wchar_t *text = new wchar_t;
									*text = _T(' ');
									sp->setText(text);
									sp->setType(SymbolObject::space);
									sp->x(x + width);
									sp->y(lineIterator->item()->y());
									sp->width(minSymbolWidth);
									lineIterator->item()->insert(symbolIterator->getID(), sp);
									//symbolIterator->next();
									delete text;
								}*/
							}
						}
					}
				}
				findGroup(lineIterator->item());
				lineIterator->next();
				delete symbolIterator;
			}
		}
		delete lineIterator;
		symbolLines->clear();
	}
	///---------------------- working with groups
	if(groups->getCount() > 0)
	{
		//prepare iterators
		Iterator<LineGroup*> *groupIterator = groups->getIterator();
		Iterator<LineGroup*> *aditionalGroupIterator = groups->getIterator();
		aditionalGroupIterator->next();
		///---------------------------------------------- find lists in text
		while(groupIterator->getID() < groups->getCount())
		{
			if(groupIterator->item()->first()->first()->getText() == _T('•'))
			{
				findList(groupIterator->item());
				groupIterator->item()->flag = false;
				while(aditionalGroupIterator->getID() < groups->getCount() && !findList(aditionalGroupIterator->item()))
					aditionalGroupIterator->next();
				if(aditionalGroupIterator->item()->getCount() == 0)
					aditionalGroupIterator->popItem();
				aditionalGroupIterator->goToID(groupIterator->getID() + 1);
			}
			else
			{
				groupIterator->next();
				aditionalGroupIterator->next();
			}
		}
		///------------------- delete list items from group list
		groupIterator->first();
		while(groupIterator->getID() < groups->getCount())
		{
			if(groupIterator->item()->flag)
			{
				if(groupIterator->item()->align() == LineGroup::unknown)
				{
					groupIterator->item()->align(LineGroup::middle);// all "unknown" groups turn to middle
					groupIterator->item()->flag = false;
				}
				groupIterator->next();
			}
			else
				groupIterator->popItem();
		}
		groupIterator->first();
		///--------------------- if there are some list items - we merged it and create real lists
		if(lists->getCount() > 0)
		{
			Iterator<GroupList*> *listIterator  = lists->getIterator();
			Iterator<GroupList*> *aditionalListIterator = lists->getIterator();
			aditionalListIterator->next();
			while(listIterator->getID() < lists->getCount())
			{
				Iterator<LineGroup*> *lastGroup = listIterator->item()->getIterator();
				aditionalListIterator->goToID(listIterator->getID() + 1);
				while(aditionalListIterator->getID() < lists->getCount())
				{
					lastGroup->last();
					if(abs(lastGroup->item()->x() - aditionalListIterator->item()->first()->x()) < minSymbolWidth)
					{
						Iterator<SymbolLine*> *lastLine = lastGroup->item()->getIterator();
						lastLine->last();
						if(aditionalListIterator->item()->first()->y() - lastLine->item()->y() - lastLine->item()->baseLine() - lastGroup->item()->space() <= lastLine->item()->baseLine() + lastGroup->item()->space() * 2)
						{
							while(aditionalListIterator->item()->getCount() > 0)
								listIterator->item()->pushBack(aditionalListIterator->item()->pop());
							aditionalListIterator->popItem();
						}
						else
							aditionalListIterator->next();
					}
					else
						aditionalListIterator->next();
				}
				listIterator->next();
				delete lastGroup;
			}
			delete aditionalListIterator;
			delete listIterator;
		}
		delete groupIterator;
		delete aditionalGroupIterator;
///---------------end working with group
	}
	
}

Page*	Parser::finishPage()
{
	fillTables();

	Iterator<LineGroup*> *groupIterator = groups->getIterator();
	while(groupIterator->getID() < groups->getCount())
	{
		if(groupIterator->item()->flag)
			groupIterator->next();
		else
			groupIterator->popItem();
	}
	delete groupIterator;

	Page *page = new Page;
	page->lines = graphicLines;
	page->groups = groups;
	page->lists = lists;
	page->tables = graphicTables;
	page->images = images;
	lists = new List<GroupList*>;
	graphicLines = new List<GraphicLine*>;
	groups = new List<LineGroup*>;
	graphicTables = new List<Table2*>;
	pages->pushBack(page);
	return page;
}

void	Parser::createGraphicLines(long x, long y, long right, long bottom, bool &isTable)
{
	List<long*> *lineStart = new List<long*>;// start line coordinates 
	long currentPixelY= y - 1;
	long currentPixelX = x - 1;
	List<long*> *pixelLine = new List<long*>;// current pixel line, X coordinate of begin and end pixels of set of not white pixels 
	List<long*> *previousPixelLine = NULL;// previous pixel line
	long *coord;// pointer to 3 long - x, y, flag =(
	int count = 0;// count of horrizontal solid pixel lines
	long ff = 0;// count of horizontal splited pixel lines
	while(++currentPixelY < bottom)
	{
		bool flag = false;
		currentPixelX = x - 1;
		///--------------------------
		while(++currentPixelX < right)
		{
			DWORD *pixelPointer = (DWORD*)(buffer + 4 * currentPixelY * pixelWidth + 4 * currentPixelX);
			if(*pixelPointer == 0xFFFFFFFF)// if pixel is white
			{
				if(flag)//if previous pixels is not white
				{
					flag = false;
					coord[1] = currentPixelX;// end set
					coord[2] = 0;
					pixelLine->pushBack(coord);// put coordinates of set nonwhite pixels in list
				}
			}
			else// if it is not white
			{
				if(!flag)// and previous white
				{
					coord = new long[3];// create new coordinates array for non white pixel set
					coord[0] = currentPixelX;
					flag = true;
				}
			}
		}
		if(currentPixelX == right)// for unfinished solid pixel line
		{
			if(flag)
			{
				flag = false;
				coord[1] = currentPixelX;
				coord[2] = 0;
				pixelLine->pushBack(coord);
			}
		}
		///-------------------------
		if(pixelLine->getCount() > 0)// if pixel line has a non white pixel sets
		{
			if(pixelLine->getCount() == 1)
			{
				if(pixelLine->first()[0] <= x + 2 && pixelLine->first()[1] >= right - 2)// if we have one solid line (magic numbers)
				{
					count++;// incremet count of neighboring solid pixel lines
					pixelLine->clear();
				}
			}
			else// if we have more than one set
			{
				Iterator<long*> *pixelLineIterator = pixelLine->getIterator();
				if(previousPixelLine)
				{
					Iterator<long*> *previousPixelLineIterator = previousPixelLine->getIterator();
					if(count > 10)// magic number
					{
						isTable = false;
						count = 0;
					}
					else// if solid line is not so "fat"
					{
						if(count > 0)
						{
							//if(graphicLines->getAt(graphicLines->getCount() - 1)->orient() == 0)
							//{
							//	isTable = false;
							//}
							//else
							{
								GraphicLine *line = new GraphicLine(Coordinates(x, currentPixelY - count),
									Coordinates(right, currentPixelY - count));// creating horizontal line
								line->setOrientationHint(0);
								graphicLines->pushBack(line);
								count = 0;
							}
						}
						if(previousPixelLine->getCount() == pixelLine->getCount())// if we have equal count of pixel sets 
						{
							while(pixelLineIterator->getID() < pixelLine->getCount())// we compare it with each other to make sure, that they all equal
							{
								if(pixelLineIterator->item()[0] != previousPixelLineIterator->item()[0] || 
									pixelLineIterator->item()[1] != previousPixelLineIterator->item()[1])
								{// if not - it can mean two things, first - it is not a table, second - some borders are changed their locations
									isTable = false;
								}
								//else
								//{// continue vertical lines must be here
									
								//}
								pixelLineIterator->next();
								previousPixelLineIterator->next();
							}
							previousPixelLine->clear();
							previousPixelLine = pixelLine;
						}
						else// if we have not equal count of pixel sets in neighboring pixel lines
						{
							bool flag = true;
							count = 0;
							while(pixelLineIterator->getID() < pixelLine->getCount())// we count of coincidence of pixel sets
							{
								previousPixelLineIterator->first();
								while(flag && previousPixelLineIterator->getID() < previousPixelLine->getCount())
								{
									if(pixelLineIterator->item()[0] == previousPixelLineIterator->item()[0] && 
										pixelLineIterator->item()[1] == previousPixelLineIterator->item()[1])
									{
										pixelLineIterator->item()[2] = 1;// turn on flags
										previousPixelLineIterator->item()[2] = 1;
										flag = false;
									}
									previousPixelLineIterator->next();
								}
								if(!flag)
								{
									count++;
									flag = true;
								}
								pixelLineIterator->next();
							}
							if(count == pixelLine->getCount())// if count of coincidence equals to count of sets in current pixel line,
							{// it means some vertical lines ended in previous pixel line
								count = 0;
								previousPixelLineIterator->first();
								while(previousPixelLineIterator->getID() < previousPixelLine->getCount())// we are looking for not flaged sets
								{
									if(previousPixelLineIterator->item()[2] == 0)// if we will find it
									{
										Iterator<long*> *startCoordIterator = lineStart->getIterator();
										bool flag = true;
										while(flag && startCoordIterator->getID() < lineStart->getCount())// we must find start coordinates of that line
										{
											if(startCoordIterator->item()[0] == previousPixelLineIterator->item()[0] + (previousPixelLineIterator->item()[1] - previousPixelLineIterator->item()[0])/2)
											{
												flag = false;
												startCoordIterator->prev();
											}
											startCoordIterator->next();
										}
										long *coord = startCoordIterator->popItem();
										GraphicLine *line = new GraphicLine(Coordinates(coord[0], coord[1]),
											Coordinates(coord[0], currentPixelY + 1));
										line->setOrientationHint(270);
										graphicLines->pushBack(line);
										delete coord;
									}
									previousPixelLineIterator->next();
								}
								previousPixelLine->clear();
								previousPixelLine = pixelLine;
							}
							else
							{
								if(count == previousPixelLine->getCount())// if count of coincidence equals to count of sets in previous pixel line, 
								{// it means some vertical lines began in current pixel line
									count = 0;
									pixelLineIterator->first();
									while(pixelLineIterator->getID() < pixelLine->getCount())
									{
										if(pixelLineIterator->item()[2] == 0)// we put start coordinates into list
										{
											long *startCoord = new long[2];
											startCoord[0] = pixelLineIterator->item()[0] + (pixelLineIterator->item()[1] - pixelLineIterator->item()[0])/2;
											startCoord[1] = currentPixelY - 2;
											lineStart->pushBack(startCoord);
										}
										pixelLineIterator->next();
									}
									previousPixelLine->clear();
									previousPixelLine = pixelLine;
								}
								else// difficult part: some lines ends, some begins, some are changed locations, and may be it is not a table...
								{
									count = 0;
									if(currentPixelY - ff > 1)
									{
										ff = currentPixelY;
										pixelLineIterator->first();
										while(pixelLineIterator->getID() < pixelLine->getCount())
										{
											if(pixelLineIterator->item()[1] - pixelLineIterator->item()[0] > 10)
											{
												GraphicLine *line = new GraphicLine(Coordinates(pixelLineIterator->item()[0], currentPixelY + 1),
													Coordinates(pixelLineIterator->item()[1], currentPixelY + 1));// creating horizontal line
												line->setOrientationHint(0);
												graphicLines->pushBack(line);
											}
											pixelLineIterator->next();
										}
									}
									else
										ff = currentPixelY;
								}
							}
						}
					}
					pixelLineIterator->first();
					while(pixelLineIterator->getID() < pixelLine->getCount())
					{
						pixelLineIterator->item()[2] = 0;
						pixelLineIterator->next();
					}
					pixelLine = new List<long*>;
					delete previousPixelLineIterator;
				}
				else
				{
					if(count > 0)
					{
						GraphicLine *line = new GraphicLine(Coordinates(x, currentPixelY - count),
							Coordinates(right, currentPixelY - count));// creating horizontal line
						line->setOrientationHint(0);
						graphicLines->pushBack(line);
						count = 0;
					}
					while(pixelLineIterator->getID() < pixelLine->getCount())
					{
						long *startCoord = new long[2];
						startCoord[0] = pixelLineIterator->item()[0] + (pixelLineIterator->item()[1] - pixelLineIterator->item()[0])/2;
						startCoord[1] = currentPixelY - 2;
						lineStart->pushBack(startCoord);
						pixelLineIterator->next();
					}
					previousPixelLine = pixelLine;
					pixelLine = new List<long*>;
				}
				delete pixelLineIterator;
			}
		}
	}
	///-----------------------
	if(currentPixelY == bottom)
		{
			while(lineStart->getCount())
			{
				GraphicLine *line = new GraphicLine(Coordinates(lineStart->first()[0], lineStart->first()[1]),
					Coordinates(lineStart->first()[0], currentPixelY + 1));
				line->setOrientationHint(270);
				graphicLines->pushBack(line);
				lineStart->pop();
			}
			if(count > 0)
			{
				//if(pixelLine->first()[0] <= x + 2 && pixelLine->first()[1] >= right - 2)// if we have one solid line
				{
					GraphicLine *line = new GraphicLine(Coordinates(x, currentPixelY - count),
						Coordinates(right, currentPixelY - count));// creating horizontal line
					line->setOrientationHint(0);
					graphicLines->pushBack(line);
					count = 0;
				}
			}
			pixelLine->clear();
		}
	delete lineStart;
	delete pixelLine;
	if(previousPixelLine)
		delete previousPixelLine;
}

void	Parser::findIntersections()
{
	Iterator<GraphicLine*> *lineIterator = graphicLines->getIterator();
		Iterator<GraphicLine*> *aditionalLineIterator = graphicLines->getIterator();
		while(lineIterator->getID() < graphicLines->getCount() - 1)
		{
			aditionalLineIterator->goToID(lineIterator->getID() + 1);
			while(aditionalLineIterator->getID() < graphicLines->getCount())
			{
				findIntersectionPoint(lineIterator->item(), aditionalLineIterator->item());
				aditionalLineIterator->next();
			}
			lineIterator->next();
		}
		delete lineIterator;
		delete aditionalLineIterator;
}

void	Parser::createPointMap()
{
	if(points->getCount() > 0)
	{
		pointMap->rowList.clear();
		Iterator<IntersectionPoint*> *pointIterator = points->getIterator();
		double minYcoord;
		while(points->getCount() > 0)
		{
			minYcoord = 1000000.0;
			List<IntersectionPoint*> *row = new List<IntersectionPoint*>;
			pointIterator->first();
			while(pointIterator->getID() < points->getCount())
			{
				if(minYcoord > pointIterator->item()->coord.y())
				{
					minYcoord = pointIterator->item()->coord.y();
				}
				pointIterator->next();
			}
			pointIterator->first();
			while(pointIterator->getID() < points->getCount())
			{
				if(minYcoord == pointIterator->item()->coord.y())
				{
					row->pushBack(pointIterator->popItem());
				}
				else
				{
					pointIterator->next();
				}
			}
			pointMap->rowList.pushBack(row);
		}
	}
}

void	Parser::createTable()
{
	//Table2 *table = new Table2;
	pointMap->addEmptyPoints();
	pointMap->sort();
	Iterator<List<IntersectionPoint*>*> *rowIterator = pointMap->rowList.getIterator();
	int count = 0;
	while(rowIterator->getID() < pointMap->rowList.getCount())
	{
		if(count < rowIterator->item()->getCount())
			count = rowIterator->item()->getCount();
		rowIterator->next();
	}
	Table2 *table = new Table2(pointMap->rowList.getCount() - 1, count - 1);
	rowIterator->first();
	Iterator<List<IntersectionPoint*>*> *aditionalRowIterator = pointMap->rowList.getIterator(1);
	Iterator<List<Cell*>*> *tableRowIterator = table->getRowIterator();
	List<Cell*> *unfinishedCells = new List<Cell*>;
	IntersectionPoint *point1, *point2;
	while(aditionalRowIterator->getID() < pointMap->rowList.getCount())
	{
		Iterator<IntersectionPoint*> *pointIterator = rowIterator->item()->getIterator();
		Iterator<IntersectionPoint*> *aditionalPointIterator = aditionalRowIterator->item()->getIterator();
		Iterator<Cell*> *cellIterator = tableRowIterator->item()->getIterator();
		double previousXcoord = rowIterator->item()->first()->coord.x();
		point1 = pointIterator->item();
		point2 = aditionalPointIterator->item();
		pointIterator->next();
		aditionalPointIterator->next();
		while(pointIterator->getID() < rowIterator->item()->getCount())
		{
			Cell *cell = cellIterator->item();
			if(!cell->merged)
			{
				cell->x(25.4 * previousXcoord / 96);
				/*if(aditionalPointIterator->item()->coord.x() < pointIterator->item()->coord.x())
				{
					//cell->merged = true;
					while(  aditionalPointIterator->getID() < aditionalRowIterator->item()->getCount()
							&& aditionalPointIterator->item()->coord.x() < pointIterator->item()->coord.x())
					{
						cellIterator->next();
						while(cellIterator->item()->merged)
							cellIterator->next();
						cell->addCell(cellIterator->item()->rowID(), cellIterator->item()->columnID());
						cellIterator->item()->merged = true;
						cellIterator->prev();
						aditionalPointIterator->next();
					}
				}
				else
				{
					if(aditionalPointIterator->item()->coord.x() > pointIterator->item()->coord.x())
					{
						//cell->merged = true;
						while(  pointIterator->getID() < rowIterator->item()->getCount()
								&& aditionalPointIterator->item()->coord.x() > pointIterator->item()->coord.x())
						{
							cellIterator->next();
							while(cellIterator->item()->merged)
								cellIterator->next();
							cell->addCell(cellIterator->item()->rowID(), cellIterator->item()->columnID());
							cellIterator->item()->merged = true;
							cellIterator->prev();
							pointIterator->next();
						}
					}
				}*/
				//if(point1 != pointIterator->item() && point2 != aditionalPointIterator->item() &&
				//	pointIterator->item()->coord.x() == aditionalPointIterator->item()->coord.x())
				{
					if(pointIterator->item()->vertical == aditionalPointIterator->item()->vertical)
					{
						if(point1->horizontal == pointIterator->item()->horizontal)
						{
							cell->y(25.4 * pointIterator->item()->coord.y() / 96);
							cell->width(25.4 * (pointIterator->item()->coord.x() - previousXcoord) / 96);
						}
						else
						{
							bool flag = true;
							Iterator<Cell*> *unfinishedCellIterator = unfinishedCells->getIterator();
							while(flag && unfinishedCellIterator->getID() < unfinishedCells->getCount())
							{
								if(unfinishedCellIterator->item()->x() == cell->x())
								{
									flag = false;
								}
								unfinishedCellIterator->next();
							}
							if(!flag)
							{
								unfinishedCellIterator->prev();
								unfinishedCellIterator->item()->addCell(cell->rowID(), cell->columnID());
								cell->merged = true;
								cell = unfinishedCellIterator->popItem();
								//if(point2->horizontal == aditionalPointIterator->item()->horizontal)
								//{
								//	unfinishedCellIterator->popItem();
								//}
							}
							delete unfinishedCellIterator;
						}
						if(point2->horizontal == aditionalPointIterator->item()->horizontal)
						{
							cell->height(25.4 * point2->coord.y() / 96 - cell->y());
						}
						else
						{
							unfinishedCells->pushBack(cell);
						}
						point1 = pointIterator->item();
						point2 = aditionalPointIterator->item();
						previousXcoord = point1->coord.x();
						cellIterator->next();
					}
					else
					{
						long id = cellIterator->getID();
						cellIterator->next();
						while(cellIterator->item()->merged)
							cellIterator->next();
						cell->addCell(cellIterator->item()->rowID(), cellIterator->item()->columnID());
						cellIterator->item()->merged = true;
						cellIterator->goToID(id);
					}
				}
				/*else
				{
					if(aditionalPointIterator->item()->coord.x() < pointIterator->item()->coord.x())
					{
						//cell->merged = true;
						unfinishedCells->pushBack(cell);
					}
					else
					{
						if(aditionalPointIterator->item()->coord.x() > pointIterator->item()->coord.x())
						{
							bool flag = true;
							Iterator<Cell*> *unfinishedCellIterator = unfinishedCells->getIterator();
							while(flag && unfinishedCellIterator->getID() < unfinishedCells->getCount())
							{
								if(unfinishedCellIterator->item()->x() == cell->x())
								{
									flag = false;
								}
								unfinishedCellIterator->next();
							}
							if(!flag)
							{
								unfinishedCellIterator->prev();
								unfinishedCellIterator->item()->addCell(cell->rowID(), cell->columnID());
								cell->merged = true;
								cell = unfinishedCellIterator->item();
								if(point2->horizontal == aditionalPointIterator->item()->horizontal)
								{
									unfinishedCellIterator->popItem();
								}
							}
							delete unfinishedCellIterator;
						}
						else
						{
							cellIterator->item(NULL);
						}
					}
				}*/
				if(pointIterator->getID() < rowIterator->item()->getCount())
					pointIterator->next();
				if(aditionalPointIterator->getID() < aditionalRowIterator->item()->getCount())
					aditionalPointIterator->next();
			}
			else
				cellIterator->next();
		}
		delete cellIterator;
		delete pointIterator;
		delete aditionalPointIterator;
		rowIterator->next();
		aditionalRowIterator->next();
		tableRowIterator->next();
	}
	delete rowIterator;
	delete aditionalRowIterator;
	delete tableRowIterator;
	delete unfinishedCells;
	/*//-----------------------------------------------------------------------------------------
	while(aditionalRowIterator->getID() < pointMap->rowList.getCount())
	{
		//if(rowIterator->item()->first()->coord.x() == aditionalRowIterator->item()->first()->coord.x())
		//{
			List<Cell*> *cellList = new List<Cell*>;
			Cell *cell = new Cell;
			Iterator<IntersectionPoint*> *pointIterator = rowIterator->item()->getIterator(1);
			Iterator<IntersectionPoint*> *aditionalPointIterator = aditionalRowIterator->item()->getIterator(1);
			double previousXcoord = rowIterator->item()->first()->coord.x();
			while(pointIterator->getID() < rowIterator->item()->getCount())
			{
				if(aditionalPointIterator->item()->coord.x() < pointIterator->item()->coord.x())
				{
					while(  aditionalPointIterator->getID() < aditionalRowIterator->item()->getCount()
							&& aditionalPointIterator->item()->coord.x() < pointIterator->item()->coord.x())
						aditionalPointIterator->next();
				}
				else
				{
					if(aditionalPointIterator->item()->coord.x() > pointIterator->item()->coord.x())
					{
						while(  pointIterator->getID() < rowIterator->item()->getCount()
								&& aditionalPointIterator->item()->coord.x() > pointIterator->item()->coord.x())
							pointIterator->next();
					}
				}
				if(pointIterator->item()->coord.x() == aditionalPointIterator->item()->coord.x())
				{
					if(pointIterator->item()->vertical == aditionalPointIterator->item()->vertical)
					{
						cell->x(previousXcoord);
						cell->y(pointIterator->item()->coord.y());
						cell->width(pointIterator->item()->coord.x() - previousXcoord);
						cell->height(aditionalPointIterator->item()->coord.y() - pointIterator->item()->coord.y());
						cell->columnID(cellList->getCount());
						cell->rowID(table->getCount());
						cellList->pushBack(cell);
						cell = new Cell;
						previousXcoord = pointIterator->item()->coord.x();
					}
					else
					{
						cell->merged = true;
					}
				}
				pointIterator->next();
				aditionalPointIterator->next();
			}
			delete pointIterator;
			delete aditionalPointIterator;
			table->pushRow(cellList);
		//}
		rowIterator->next();
		aditionalRowIterator->next();
	}
	rowIterator->goToID(1);
	Iterator<List<Cell*>*> *tableRowIterator = table->getRowIterator();
	Iterator<List<Cell*>*> *aditionalTableRowIterator = table->getRowIterator();
	aditionalTableRowIterator->goToID(1);
	while(aditionalTableRowIterator->getID() < table->getCount())
	{
		Iterator<IntersectionPoint*> *pointIterator = rowIterator->item()->getIterator();
		Iterator<IntersectionPoint*> *aditionalPointIterator = rowIterator->item()->getIterator(1);
		Iterator<Cell*> *cellIterator = tableRowIterator->item()->getIterator();
		Iterator<Cell*> *aditionalCellIterator = aditionalTableRowIterator->item()->getIterator();
		while(cellIterator->getID() < tableRowIterator->item()->getCount() && aditionalCellIterator->getID() < aditionalTableRowIterator->item()->getCount())
		{
			if(cellIterator->item()->x() == pointIterator->item()->coord.x())
			{
				if(cellIterator->item()->width() == aditionalPointIterator->item()->coord.x() - pointIterator->item()->coord.x())
				{
					if(aditionalPointIterator->item()->horizontal != pointIterator->item()->horizontal)
					{
						Cell *cell = (Cell*)aditionalCellIterator->item();
						cellIterator->item()->height(cellIterator->item()->height() + cell->height());
						delete cell;
						aditionalCellIterator->item(cellIterator->item());
					}
				}
			}
			cellIterator->next();
			aditionalCellIterator->next();
		}
		rowIterator->next();
		tableRowIterator->next();
		aditionalTableRowIterator->next();
	}		*/		
	graphicTables->pushBack(table);
}

void	Parser::fillTables()
{
	bool stop = false;
	Iterator<LineGroup*> *groupIterator = groups->getIterator();
	while(groupIterator->getID() < groups->getCount())
	{
		if(groupIterator->item()->flag)
			groupIterator->next();
		else
			groupIterator->item()->flag = true;
	}
	groupIterator->first();
	Iterator<GroupList*> *listIterator = lists->getIterator();
	Iterator<Table2*> *tableIterator = graphicTables->getIterator();
	while(tableIterator->getID() < graphicTables->getCount())
	{
		Iterator<List<Cell*>*> *rowIterator = tableIterator->item()->getRowIterator();
		while(rowIterator->getID() < tableIterator->item()->getCount())
		{
			Iterator<Cell*> *cellIterator = rowIterator->item()->getIterator();
			while(cellIterator->getID() < rowIterator->item()->getCount())
			{
				Cell *cell = cellIterator->item();
				if(!cell->merged)
				{
				groupIterator->first();
				listIterator->first();
				stop = false;
				while(!stop && groupIterator->getID() < groups->getCount())
				{
					LineGroup* group = groupIterator->item();
					if( group->x() >= cell->x() &&
						group->y() >= cell->y() &&
						group->x() < cell->x() + cell->width() &&
						group->y() < cell->y() + cell->height())
					{
						bool flag = false;
						if( group->width() > cell->width())
						{
							flag = true;
							cell->content(group->split(true,cell->x() + cell->width(), 0));
						}
						if( group->height() > cell->height())
						{
							if(flag)
							{
								group = (LineGroup*)cell->content()->split(false, 0.0, cell->y() + cell->height());
								group->flag = false;
								groups->push((LineGroup*)cell->content());
								cell->content(group);
							}
							else
							{
								flag = true;
								cell->content(group->split(false, 0, cell->y() + cell->height()));
							}
						}
						stop = true;
						if(!flag)
						{
							cell->content(group);
							group->flag = false;
						}
					}
					groupIterator->next();
				}
				}
				/*stop = false;
				while(listIterator->getID() < lists->getCount())
				{
					GroupList* list = listIterator->item();
					if( list->x() >= 25.4 * cell->x() / 96.0 &&
						list->y() >= 25.4 * cell->y() / 96.0 &&
						list->x() < 25.4 * (cell->x() + cell->width()) / 96.0 &&
						list->y() < 25.4 * (cell->y() + cell->height()) / 96.0)
					{
						bool flag = false;
						if( list->width() > 25.4 * cell->width() / 96.0)
						{
							flag = true;
							cell->content(list->split(true, 25.4 * (cell->x() + cell->width()) / 96.0, 0));
						}
						if( list->height() > 25.4 * cell->height() / 96.0)
						{
							if(flag)
							{
								list = (LineGroup*)cell->content()->split(false, 0.0, 25.4 * (cell->y() + cell->height()) / 96.0);
								list->flag = false;
								lists->push((GroupList*)cell->content());
								cell->content(list);
							}
							else
							{
								flag = true;
								cell->content(list->split(false, 0, 25.4 * (cell->y() + cell->height()) / 96.0));
							}
						}
						stop = true;
						if(!flag)
						{
							cell->content(list);
							list->flag = false;
						}
					}
					listIterator->next();
				}*/
				cellIterator->next();
			}
			delete cellIterator;
			rowIterator->next();
		}
		delete rowIterator;
		tableIterator->next();
	}
	delete tableIterator;
}

void	Parser::findIntersectionPoint(GraphicLine* p_line1, GraphicLine* p_line2)
{
	GraphicLine *vertical = NULL, *horizontal = NULL;
	bool up = false;
	bool right = false;
	switch(p_line1->orient())
	{
		case 0:
			horizontal = p_line1;
			right = true;//changing a default orientation of line
			break;
		case 90:
			vertical = p_line1;
			up = true;// up up up
			break;
		case 180:
			horizontal = p_line1;
			break;
		case 270:
			vertical = p_line1;
			break;
		default:
			return;
	}

	switch(p_line2->orient())
	{
		case 0:
			if(!horizontal)// if it exist, it means that we have (omg) two horizontal lines... 
			{
				horizontal = p_line2;
				right = true;
			}
			break;
		case 90:
			if(!vertical)// the same problem
			{
				vertical = p_line2;
				up = true;
			}
			break;
		case 180:
			if(!horizontal)// another one
				horizontal = p_line2;
			break;
		case 270:
			if(!vertical)// yep
				vertical = p_line2;
			break;
		default:
			return;
	}
	if(vertical && horizontal)
	{
		IntersectionPoint *point = NULL;
		if(right)
		{
			if(horizontal->x() <= vertical->x() && horizontal->endPoint().x() >= vertical->x())
			{
				if(up)
				{
					if(horizontal->y() <= vertical->y() && horizontal->y() >= vertical->endPoint().y())
						point = new IntersectionPoint;
				}
				else
					if(horizontal->y() >= vertical->y() && horizontal->y() <= vertical->endPoint().y())
						point = new IntersectionPoint;
			}
		}
		else
		{
			if(horizontal->x() >= vertical->x() && horizontal->endPoint().x() <= vertical->x())
			{
				if(up)
				{
					if(horizontal->y() <= vertical->y() && horizontal->y() >= vertical->endPoint().y())
						point = new IntersectionPoint;
				}
				else
					if(horizontal->y() >= vertical->y() && horizontal->y() <= vertical->endPoint().y())
						point = new IntersectionPoint;
			}
		}
		if(point)// if intersection point exist, we control matches with other points
		{
			Iterator<IntersectionPoint*> *pointIterator = points->getIterator();
			bool flag = true;
			while(flag && pointIterator->getID() < points->getCount())
			{
				if(abs(vertical->x() - pointIterator->item()->coord.x()) < 0.1)
					if(abs(horizontal->y() - pointIterator->item()->coord.y()) < 0.1)
						flag = false;
				pointIterator->next();
			}
			if(flag)
			{
				point->horizontal = horizontal;
				point->vertical = vertical;
				point->coord.x(vertical->x());
				point->coord.y(horizontal->y());
				points->push(point);
			}
			delete pointIterator;
		}
	}
	/*else //I don't have any ideas what to do in such situation
	{
		if(horizontal)
		{
			if(right)
			{
				if(horizontal->x() == p_line2->endPoint().x() || horizontal->x() == p_line2->x())
			}
			else
				if(horizontal->endPoint().x() == p_line2->endPoint()->x() || horizontal->endPoint().x() == p_line2->x())
		}
		else
		{
			if(up)
			{
				if(vertical->y() == p_line2->endPoint().y() || vertical->y() == p_line2->y())
			}
			else
				if(vertical->endPoint().y() == p_line2->endPoint().y() || vertical->endPoint().y() == p_line2->y())
		}

	}*/
}

/*void Parser::findColumn(LineGroup *group)
{
	Iterator<Column*> *iter = columns->getIterator();
	bool flag = true;
	while(flag && iter->getID() < columns->getCount())
	{
		switch(group->align())
		{
		case LineGroup::middle:
		case LineGroup::justify:
		case LineGroup::left:
			if(iter->item()->x() <= group->x() && iter->item()->x() + iter->item()->width() >= group->x())
			{
				if(iter->item()->x() + iter->item()->width() >= group->x() + group->width())
					flag = false;
			}
			break;
		case LineGroup::right:
			if(iter->item()->x() + iter->item()->width() >= group->x() + group->width())
			{
				if(iter->item()->x() <= group->x())
					flag = false;
			}
			break;
		}
		iter->next();
	}
	if(flag)
	{
		Column *col = new Column;
		col->x(group->x());
		col->y(group->y());
		col->width(group->width());
		col->height(group->height());
		col->push(group);
		columns->pushBack(col);
	}
	else
	{
		iter->prev();
		iter->item()->pushBack(group);
		iter->item()->height(iter->item()->height() + group->height());
		if(iter->item()->width() < group->width())
			iter->item()->width(group->width());
	}
	delete iter;
}*/

void	Parser::findGroup(SymbolLine *line)
{
	Iterator<LineGroup*> *iter = groups->getIterator();
	bool flag = true;
	double k = 0.7;//magic number =(
	if(line->getCount() > 0)
	{
		while(flag && iter->getID() < groups->getCount())// find a suitable group
		{
			bool unknown = false;
			switch(iter->item()->align())
			{
			case LineGroup::unknown:
				unknown = true;
			case LineGroup::justify:
				if(iter->item()->x() < line->x() + minSymbolWidth && iter->item()->x() > line->x() - minSymbolWidth)
				{
					if(iter->item()->x() + iter->item()->width() < line->x() + line->width() + minSymbolWidth && iter->item()->x() + iter ->item()->width() > line->x() + line->width() - minSymbolWidth)
					{
						Iterator<SymbolLine*> *last = iter->item()->getIterator();
						last->last();
						if(last->item()->baseLine() == line->baseLine() && abs(line->y() - last->item()->y() - last->item()->baseLine() - iter->item()->space()) <= k)
						//if(iter->item()->width() < line->width() + maxSymbolWidth && iter->item()->width() > line->width() - maxSymbolWidth)
						{
							flag = false;
							if(unknown)
								iter->item()->align(LineGroup::justify);
						}
						delete last;
					}
				}
			case LineGroup::left:
				if(flag && iter->item()->x() < line->x() + minSymbolWidth && iter->item()->x() > line->x() - minSymbolWidth)
				{					
					Iterator<SymbolLine*> *last = iter->item()->getIterator();
					last->last();
					if(last->item()->baseLine() == line->baseLine() && abs(line->y() - last->item()->y() - last->item()->baseLine() - iter->item()->space()) <= k)
					{
						flag = false;
						if(unknown)
							iter->item()->align(LineGroup::left);
					}
					delete last;
				}
			case LineGroup::right:
				if(flag && iter->item()->x() + iter->item()->width() < line->x() + line->width() + minSymbolWidth && iter->item()->x() + iter ->item()->width() > line->x() + line->width() - minSymbolWidth)
				{
					Iterator<SymbolLine*> *last = iter->item()->getIterator();
					last->last();
					if(last->item()->baseLine() == line->baseLine() && abs(line->y() - last->item()->y() - last->item()->baseLine() - iter->item()->space()) <= k)
					{
						flag = false;
						if(unknown)
							iter->item()->align(LineGroup::right);
					}
					delete last;
				}
			case LineGroup::middle:
				if(flag && abs(iter->item()->x() + iter->item()->width()/2 - line->x() - line->width()/2) <= 0.4)
				{
					Iterator<SymbolLine*> *last = iter->item()->getIterator();
					last->last();
					if(last->item()->baseLine() == line->baseLine() && abs(line->y() - last->item()->y() - last->item()->baseLine() - iter->item()->space()) <= k)
					{
						flag = false;
						if(unknown)
							iter->item()->align(LineGroup::middle);
					}
				}
			}
			iter->next();
		}
		if(flag)// if we couldn't find a suitable group, we create another one
		{
			LineGroup *p_group = new LineGroup;
			p_group->push(line);
			p_group->x(line->x());
			p_group->y(line->y());
			p_group->width(line->width());
			p_group->height(line->height());
			p_group->space(verticalLineSpace);
			p_group->align(LineGroup::unknown);
			groups->pushBack(p_group);
		}
		else
		{
			iter->prev();
			iter->item()->pushBack(line);
			iter->item()->height(iter->item()->height() + line->height() + iter->item()->space());
			if(iter->item()->width() < line->width())
				iter->item()->width(line->width());
			if(iter->item()->x() > line->x())
				iter->item()->x(line->x());
		}
		delete iter;
	}
}

bool	Parser::findList(LineGroup* group, bool stage)
{
	Iterator<GroupList*> *iter = lists->getIterator();
	bool flag = false;//universal flag
	if(group->getCount() > 0)
	{
		if(group->first()->first()->getText() == _T('•'))
		{
			group->first()->pop();
			if(group->first()->first()->getText() == _T(' '))
				group->first()->pop();
			group->resize();
			if(!findList(group, true))
			{
				GroupList *list = new GroupList;
				list->push(group);
				lists->pushBack(list);
			}
		}
		else
		{
			flag = true;
		}
	}
	if(flag)
	{
		while(flag && iter->getID() < lists->getCount())
		{
			if(iter->item()->first()->x() < group->x() + minSymbolWidth && iter->item()->first()->x() > group->x() - minSymbolWidth)
			{
				Iterator<LineGroup*> *lastGroup = iter->item()->getIterator();
				lastGroup->last();
				Iterator<SymbolLine*> *lastLine = lastGroup->item()->getIterator();
				lastLine->last();
				if(abs(group->first()->y() - lastLine->item()->y() - lastLine->item()->baseLine() - lastGroup->item()->space()) < 0.5)
				{
					flag = false;
				}
				delete lastLine;
				delete lastGroup;
			}
			iter->next();
		}
		if(!flag)
		{
			iter->prev();
			if(stage)
			{
				iter->item()->pushBack(group);
			}
			else
			{
				Iterator<LineGroup*> *lastGroup = iter->item()->getIterator();
				lastGroup->last();
				lastGroup->item()->merge(group);
			}				
			flag = true;
		}
		else
		{
			flag = false;
		}
	}
	delete iter;
	return flag;
}

bool	Parser::findCell(Object *obj, Table2 *currentTable)
{
	bool returnFlag = false;
	if(obj)
	{
		if(maxLineWidth - obj->width() < maxLineWidth/4)
		{
			bool mainFlag = true;
				Iterator<List<Cell*>*> *rowIterator = currentTable->getRowIterator();
				while(mainFlag && rowIterator->getID() < currentTable->getCount())
				{
					if(abs(rowIterator->item()->first()->y() - obj->y()) < 0.5 || 
						(rowIterator->item()->first()->y() < obj->y() && rowIterator->item()->first()->y() + rowIterator->item()->first()->height() > obj->y() + obj->height()) ||
						(rowIterator->item()->first()->y() > obj->y() && rowIterator->item()->first()->y() + rowIterator->item()->first()->height() < obj->y() + obj->height()))
					{
						//if((LineGroup*)obj->flag)
						{
							if(currentTable->empty())
							{
								rowIterator->item()->pushBack(new Cell(rowIterator->getID(), rowIterator->item()->getCount(),obj));
								currentTable->columnCount(currentTable->columnCount()+1);
								//(LineGroup*)obj->flag = false;
								mainFlag = false;
								returnFlag = true;
							}
							else
							{
								//rowIter->last();
								Iterator<Cell*> *iter = rowIterator->item()->getIterator();
								bool subFlag = true;
								while(subFlag && iter->getID() < rowIterator->item()->getCount())
								{
									if(abs(iter->item()->x() - obj->x()) < 0.5)
									{
										subFlag = false;
										mainFlag = false;
										returnFlag = true;
										rowIterator->item()->pushBack(new Cell(rowIterator->getID(), rowIterator->item()->getCount(),obj));
										//(LineGroup*)obj->flag = false;
									}
									else
									{
										if( iter->item()->x() < obj->x() )//&& iter->item()->x() + iter->item()->width() <= aditionalGroupIterator->item()->x())
										{
											if(iter->getID() == rowIterator->item()->getCount())
											{
												LineGroup *emptyGroup = new LineGroup;
												emptyGroup->x(iter->item()->x());
												emptyGroup->y(rowIterator->item()->first()->y());
												emptyGroup->width(iter->item()->y());
												emptyGroup->height(rowIterator->item()->first()->height());
												rowIterator->item()->pushBack(new Cell(rowIterator->getID(), rowIterator->item()->getCount(), emptyGroup));
											}
											iter->next();
										}
										else
										{
											//if(iter->item()->x() > aditionalGroupIterator->item()->x())
											{
												subFlag = false;
												mainFlag = false;
												rowIterator->item()->pushBack(new Cell(rowIterator->getID(), rowIterator->item()->getCount(),obj));
												returnFlag = true;
												//(LineGroup*)obj->flag = false;
											}
										}
									}
								}
								delete iter;
							}
						}
					}
					rowIterator->next();
				}
		}
	}
	return returnFlag;
}

void	Parser::toTXT()// for test only
{
	std::ofstream output("D:/file.txt");
	output<<"new test"<<std::endl;
	bool txt = true;
	bool table = false;
	if(table)
	{
		Iterator<Table2*> *tableIterator = textTables->getIterator();
		while(tableIterator->getID() < textTables->getCount())
		{
			for(int i = 0; i < tableIterator->item()->getCount(); i++)
			{
				output<<"new line"<<std::endl;
				List<Cell*> *row = tableIterator->item()->getRow(i);
				Iterator<Cell*> *cellIterator = row->getIterator();
				while(cellIterator->getID() < row->getCount())
				{
					LineGroup *group = (LineGroup*)cellIterator->item();
					output<<"width: "<<group->width()<<"("<<group->x()<<","<<group->y()<<") "<<group->getCount()<<" lines"<<std::endl;
					output<<std::endl;
					Iterator<SymbolLine*> *lineIterator = group->getIterator();
					while(lineIterator->getID() < group->getCount())
					{
						//output<<"Line #"<<lineIterator->getID()<<" ("<<lineIterator->item()->x()<<", "<<lineIterator->item()->y()<<") width: "<<lineIterator->item()->width()<<": "<<lineIterator->item()->getCount()<<" symbols"<<std::endl;
						Iterator<SymbolObject*> *symbolIterator = lineIterator->item()->getIterator();
						while(symbolIterator->getID() < lineIterator->item()->getCount())
						{
							if(symbolIterator->item()->getType() == SymbolObject::word)
							{
								Iterator<SymbolObject*> *wordSymbolIterator = dynamic_cast<List<SymbolObject*>*>(symbolIterator->item())->getIterator();
								while(wordSymbolIterator->getID() < dynamic_cast<List<SymbolObject*>*>(symbolIterator->item())->getCount())
								{
									output<<(char)wordSymbolIterator->item()->getText();
									wordSymbolIterator->next();
								}
							}
							else
							{
								output<<(char)symbolIterator->item()->getText();
							}
							symbolIterator->next();
						}
						delete symbolIterator;
						lineIterator->next();
					}
					delete lineIterator;
					output<<std::endl;
					cellIterator->next();
				}
				delete cellIterator;
			}
			tableIterator->next();
		}
		delete tableIterator;
	}
	if(txt)
	{
		output<<maxLineWidth;
		Iterator<Page*> *pageIterator = pages->getIterator();
		while(pageIterator->getID() < pages->getCount())
		{
			Iterator<LineGroup*> *groupIterator = pageIterator->item()->groups->getIterator();
			while(groupIterator->getID() < pageIterator->item()->groups->getCount())
			{
				output<<"width: "<<groupIterator->item()->width()<<"("<<groupIterator->item()->x()<<","<<groupIterator->item()->y()<<") group #"<<groupIterator->getID()<<": "<<groupIterator->item()->getCount()<<" lines, align: "<<groupIterator->item()->align()<<std::endl;
				output<<std::endl;
				Iterator<SymbolLine*> *lineIterator = groupIterator->item()->getIterator();
				while(lineIterator->getID() < groupIterator->item()->getCount())
				{
					//output<<"Line #"<<lineIterator->getID()<<" ("<<lineIterator->item()->x()<<", "<<lineIterator->item()->y()<<") width: "<<lineIterator->item()->width()<<": "<<lineIterator->item()->getCount()<<" symbols"<<std::endl;
					Iterator<SymbolObject*> *symbolIterator = lineIterator->item()->getIterator();
					while(symbolIterator->getID() < lineIterator->item()->getCount())
					{
						if(symbolIterator->item()->getElementType() == SymbolObject::word)
						{
							Iterator<SymbolObject*> *wordSymbolIterator = dynamic_cast<List<SymbolObject*>*>(symbolIterator->item())->getIterator();
							while(wordSymbolIterator->getID() < dynamic_cast<List<SymbolObject*>*>(symbolIterator->item())->getCount())
							{
								output<<(char)wordSymbolIterator->item()->getText();
								wordSymbolIterator->next();
							}
						}
						else
						{
							output<<(char)symbolIterator->item()->getText();
						}
						symbolIterator->next();
					}
					output<<std::endl;
					lineIterator->next();
				}
				output<<std::endl;
				groupIterator->next();
			}
			pageIterator->next();
		}
	}
}

void	Parser::toHTML()
{
	std::ofstream output("D:/test.html");
	output<<"<html>"<<std::endl;
	output<<"<head>"<<std::endl;
	output<<"<title> Test </title>"<<std::endl;
	output<<"</head>"<<std::endl;
	output<<"<body>"<<std::endl;
	Iterator<Page*> *pageIterator = pages->getIterator();
	while(pageIterator->getID() < pages->getCount())
	{
	if(pageIterator->item()->lists->getCount() > 0)
	{
		Iterator<GroupList*> *listIterator = pageIterator->item()->lists->getIterator();
		while(listIterator->getID() < pageIterator->item()->lists->getCount())
		{
			output<<"<UL>"<<std::endl;
			Iterator<LineGroup*> *groupIterator = listIterator->item()->getIterator();
			while(groupIterator->getID() < listIterator->item()->getCount())
			{
				output<<"<LI>"<<std::endl;
				Iterator<SymbolLine*> *lineIterator = groupIterator->item()->getIterator();
				while(lineIterator->getID() < groupIterator->item()->getCount())
				{
					//output<<"Line #"<<lineIterator->getID()<<" ("<<lineIterator->item()->x()<<", "<<lineIterator->item()->y()<<") width: "<<lineIterator->item()->width()<<": "<<lineIterator->item()->getCount()<<" symbols"<<std::endl;
					Iterator<SymbolObject*> *symbolIterator = lineIterator->item()->getIterator();
					while(symbolIterator->getID() < lineIterator->item()->getCount())
					{
						if(symbolIterator->item()->getElementType() == SymbolObject::word)
						{
							Iterator<SymbolObject*> *wordSymbolIterator = dynamic_cast<List<SymbolObject*>*>(symbolIterator->item())->getIterator();
							while(wordSymbolIterator->getID() < dynamic_cast<List<SymbolObject*>*>(symbolIterator->item())->getCount())
							{
								output<<(char)wordSymbolIterator->item()->getText();
								wordSymbolIterator->next();
							}
						}
						else
						{
							output<<(char)symbolIterator->item()->getText();
						}
						symbolIterator->next();
					}
					output<<std::endl;
					lineIterator->next();
				}
				output<<"</LI>"<<std::endl;
				groupIterator->next();
			}
			output<<"</UL>"<<std::endl;
			output<<"<HR>"<<std::endl;
			listIterator->next();
		}
	}
	//pageIterator->next();
	//}
	if(pageIterator->item()->tables->getCount() > 0)
	{
		Iterator<Table2*> *tableIterator = pageIterator->item()->tables->getIterator();
		while(tableIterator->getID() < pageIterator->item()->tables->getCount())
		{
			Iterator<List<Cell*>*> *rowIterator = tableIterator->item()->getRowIterator();
			output<<"<style type=\"text/css\">table{border-collapse:collapse;} td, th{border:1px solid #000000;}</style><table>";
			while(rowIterator->getID() < tableIterator->item()->getCount())
			{
				output<<"<tr>";
				Iterator<Cell*> *cellIterator = rowIterator->item()->getIterator();
				while(cellIterator->getID() < rowIterator->item()->getCount())
				{
					Cell* cell = cellIterator->item();
					if(!cell->merged)
					{
						output<<"<td";
						if(cell->cellCountX())
						{
							output<<" colspan=\""<< cell->cellCountX() + 1 <<"\"";
						}
						if(cell->cellCountY())
						{
							output<<" rowspan=\""<< cell->cellCountY() + 1 <<"\"";
						}
						output<<"> <br>";
						if(cell->content())
						{
							Object* obj= cell->content();
							switch(obj->getType())
							{
								case Object::group:
								{
									LineGroup* group = (LineGroup*)(obj);
									if(group)
									{
										Iterator<SymbolLine*> *lineIterator = group->getIterator();
										while(lineIterator->getID() < group->getCount())
										{
											Iterator<SymbolObject*> *symbolIterator = lineIterator->item()->getIterator();
											while(symbolIterator->getID() < lineIterator->item()->getCount())
											{
												if(symbolIterator->item()->getElementType() == SymbolObject::word)
												{
													Word *word = dynamic_cast<Word*>(symbolIterator->item());
													wchar_t *temp = new wchar_t[word->getCount() + 1];
													int i = 0;
													Iterator<SymbolObject*> *wordSymbolIterator = word->getIterator();
													while(wordSymbolIterator->getID() < word->getCount())
													{
														temp[i++] = wordSymbolIterator->item()->getText();
														wordSymbolIterator->next();
													}
													temp[i] = L'\0';
													//BSTR test = SysAllocString(temp);
													output<< temp;
													delete temp;
													delete wordSymbolIterator;
												}
												else
												{
													output<<symbolIterator->item()->getText();
												}
												symbolIterator->next();
											}
											delete symbolIterator;
											lineIterator->next();
										}
										delete lineIterator;
									}
									break;
								}
								case Object::image:
								{
									//(convertImage(obj));
									break;
								}
								case Object::list:
								{
									//(convertList(obj);
									break;
								}
								case Object::table:
								{
									//(convertTable(obj);
									break;
								}
								case Object::unknown:
									break;
							}
						}
						output<<"</td>";
					}
					cellIterator->next();
				}
				output<<"</tr>";
				delete cellIterator;
				rowIterator->next();
			}
			delete rowIterator;
			output<<"</table>";
			tableIterator->next();
		}
		delete	tableIterator;
	}
	pageIterator->next();
	}
	output<<"</body>"<<std::endl;
	output<<"</html>"<<std::endl;
}