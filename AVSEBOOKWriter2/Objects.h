#include "common/common.h"
#include "writers/format.h"

using namespace NSEBook;
class Page;

class Coordinates
{
protected:
	double m_x, m_y;
public:
	Coordinates(): m_x(0), m_y(0) {}
	Coordinates(double x, double y): m_x(x), m_y(y) {}
	Coordinates(Coordinates &t_coord): m_x(t_coord.x()), m_y(t_coord.y()) {}
	~Coordinates() {}
	
	virtual Coordinates operator=(Coordinates t_coord)
	{
		x(t_coord.x());
		y(t_coord.y());
		return *this;
	}
	virtual double x()
	{
		return m_x;
	}

	virtual bool x(double t_x)
	{
		if(t_x > 0)
		{
			m_x = t_x;
			return true;
		}
		return false;
	}

	virtual double y()
	{
		return m_y;
	}

	virtual bool y(double t_y)
	{
		if(t_y > 0)
		{
			m_y = t_y;
			return true;
		}
		return false;
	}	
};

class Size
{
protected:
	double m_width, m_height;
public:
	Size():m_width(0),m_height(0){}
	Size(double width, double height):m_width(width),m_height(height){}
	Size(Size &t_size):m_width(t_size.width()),m_height(t_size.height()){}
	~Size(){}
	
	virtual double width()
	{
		return m_width;
	}

	virtual bool width(double t_width)
	{
		if(t_width > 0)
		{
			m_width = t_width;
			return true;
		}
		return false;
	}

	virtual double height()
	{
		return m_height;
	}

	virtual bool height(double t_height)
	{
		if(t_height > 0)
		{
			m_height = t_height;
			return true;
		}
		return false;
	}
	
	virtual bool setSize(Size *t_size)
	{
		if(width(t_size->width()))
			if(height(t_size->height()))
				return true;
		return false;
	}
	virtual bool setSize(double t_width, double t_height)
	{
		if(width(t_width))
			if(height(t_height))
				return true;
		return false;
	}
	virtual Size& operator=(Size &t_size)
	{
		setSize(&t_size);
		return *this;
	}
};

template<typename E>
class Iterator
{
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual E item() = 0;
	virtual void item(E item) = 0;
	virtual void first() = 0;
	virtual void last() = 0;
	virtual void goToID(long id) = 0;
	virtual long getID() = 0;
	virtual E popItem() = 0;
};

template<typename E>
class List
{
protected:
	class Node
	{
	public:
		Node(){}
		Node(E p_obj): item(p_obj){}
		~Node(){}

		Node *next;
		Node *prev;
		E item;
	};
	Node *head;
	//Node *current;
	long count;
	E pop(Node *p_node)
	{
		//E t_item;
		Node* tmp;
		if(p_node)
		{
			//current = p_node;
			p_node->next->prev = p_node->prev;
			p_node->prev->next = p_node->next;
			tmp = p_node;
			p_node = p_node->next;
			count--;
		}
		else
			return NULL;
		return tmp->item;

	}
	void sort(long position, long n)
	{
		E temp;
		Iterator<E> *p, *iter_forward = getIterator(position), *iter_back = getIterator(n);
		p = getIterator(n>>1);

		do 
		{
			while(iter_forward->item()->x() < p->item()->x() ) iter_forward->next();
			while(iter_back->item()->x() > p->item()->x() ) iter_back->prev();
			if (iter_forward->getID() <= iter_back->getID()) 
			{
				temp = iter_forward->item();
				iter_forward->item(iter_back->item());
				iter_back->item(temp);
				iter_forward->next();
				iter_back->prev();
			}
		} while (iter_forward->getID() <= iter_back->getID());
		if ( iter_back->getID() > 0 ) sort(0, iter_back->getID());
		if ( n > iter_forward->getID()) sort(position + iter_forward->getID(), n - iter_forward->getID());
	}

public:
	template<typename E>
	class ListIterator: public Iterator<E>
	{
	protected:
		List *parent;
		Node *currentNode;
		long index;

	public:
		ListIterator(List *p_parent): parent(p_parent), index(0){}
		ListIterator(Node *p_node, long t_index, List *p_parent): currentNode(p_node), parent(p_parent), index(t_index){}
		~ListIterator(){parent = NULL; currentNode = NULL;}

		void first()
		{
			currentNode = parent->head;
			index = 0;
		}

		void last()
		{
			currentNode = parent->head->prev;
			index = parent->count - 1;
		}

		void goToID(long t_index)
		{
			if(t_index < parent->count)
			{
				if(t_index == 0)
					first();
				else
				{
					if(t_index > 0 && index < t_index)
					{
						while(t_index > index)
							next();
					}
					if(t_index > 0 && index > t_index)
					{
						while(t_index < index)
							prev();
					}
				}
			}
		}
		void next()
		{
			currentNode = currentNode->next;
			index++;
		}

		void prev()
		{
			currentNode = currentNode->prev;
			index--;
		}

		long getID()
		{
			if(parent->count)
			{
				if(index > parent->count)
					index = index % parent->count;
				if(index < 0)
				{
					index *= -1;
					index = index % parent->count;
				}
				return index;
			}
			else
				return 0;
		}

		E item()
		{
			return currentNode->item;
		}

		E popItem()
		{
			if(index != 0)
			{
				Node* tmp;
				currentNode->next->prev = currentNode->prev;
				currentNode->prev->next = currentNode->next;
				tmp = currentNode;
				currentNode = currentNode->next;
				parent->count--;
				return  tmp->item;//parent->pop(currentNode);
			}
			else
				return parent->pop();
		}
		void item(E p_item)
		{
			currentNode->item = p_item;
		}
	};

	List()
	{
		count = 0;
		head = new Node;
		//head->item = NULL;
		head->next = head;
		head->prev = head;
	}
	~List()
	{
		clear();
	}

	void insert(long position, E p_obj)
	{
		if(position >= 0 && position < count)
		{
			Node *tmp;
			tmp = head;
			while(position > 1)
			{
				tmp = tmp->next;
				position--;
			}
			Node *p_node = new Node(p_obj);
			p_node->next = tmp->next;
			p_node->prev = tmp;
			tmp->next = p_node;
			p_node->next->prev = p_node;
			count++;
		}
	}
	void push(E p_obj)
	{
		if(count > 0)
		{
			Node *tmp = new Node(p_obj);
			tmp->prev = head->prev;
			tmp->next = head;
			head->prev->next = tmp;
			head->prev = tmp;
			head = tmp;
			count++;
		}
		else
		{
			head->item = p_obj;
			count++;
		}
	}

	void pushBack(E p_obj)
	{
		if(count > 0)
		{
			Node *tmp = new Node(p_obj);
			tmp->prev = head->prev;
			tmp->next = head;
			head->prev->next = tmp;
			head->prev = tmp;
			count++;
		}
		else
		{
			head->item = p_obj;
			count++;
		}
	}

	E first()
	{
		return head->item;
	}

	E last()
	{
		return head->prev->item;
	}

	E pop()
	{
		Node *tmp;
		if(count > 0)
		{
			tmp = head;
			head->next->prev = head->prev;
			head->prev->next = head->next;
			head = head->next;
			count--;
		}
		else
			tmp = head;
		return tmp->item;
	}

	E popAt(long index)
	{
		long t_index = index;
		Node *tmp;
		if(count > 0 && index >= 0 && index < count)
		{
			if(index == 0)
			{
				tmp = head;
				head->next->prev = head->prev;
				head->prev->next = head->next;
				head = head->next;
			}
			else
			{
				tmp = head;
				while(index > 0)
				{
					tmp = tmp->next;
					index--;
				}
				tmp->next->prev = tmp->prev;
				tmp->prev->next = tmp->next;
			}
		}
		else
			return NULL;
		count--;
		return tmp->item;			
	}

	E getAt(long index)
	{
		Node *tmp;
		if(count > 0 && index >= 0 && index < count)
		{
			tmp = head;
			while(index > 0)
			{
				tmp = tmp->next;
				index--;
			}
		}
		else
			return NULL;
		return tmp->item;	
	}

	E operator[](long index)
	{
		return getAt(index);
	}

	long getCount() const
	{
		return count;
	}

	void sort()
	{
		if(count > 1)
			sort(0, count);
	}
	void clear()
	{
		if(count > 0)
		{
			head->prev->next = NULL;
			Node *tmp;
			tmp = head;
			while(tmp != NULL)
			{
				tmp = tmp->next;
				//head->item = NULL;
				delete head;
				head = tmp;
			}
			count = 0;
			head = new Node;
			head->next = head;
			head->prev = head;
			//head->item = NULL;
		}
	}

	Iterator<E>* getIterator(long index = 0)
	{
		Node *tmp = head;
		long t_index = index;
		if(count > 0 && index > 0 && index < count)
		{
			while(index > 0)
			{
				tmp = tmp->next;
				index--;
			}
		}
		return new ListIterator<E>(tmp, t_index, this);
	}

	void copy(List *list)
	{
		Iterator<E> *iter = list->getIterator();
		long t_count = count = list->getCount();
		clear();
		while(t_count > 0)
		{
			push(iter->item());
			iter->next();
			t_count--;
		}
	}
};

class Object: public Size, public Coordinates
{
public:
	enum ObjectType{group, table, list, image, unknown};
protected:
	ObjectType type;
public:
	Object();
	~Object();

	virtual ObjectType getType();
	virtual void merge(Object* p_obj){}
	virtual void resize(){}
	virtual Object* split(bool orient, double x, double y){return NULL;}

	//virtual bool isDefaultStyle();
	//virtual bool setStyle(Style& s);
	//virtual Style* getStyle();
	//virtual int getType();
	virtual void coord(Coordinates& c);
	virtual Coordinates* coord();
	virtual void size(Size& s);
	virtual Size* size() const;
};

class SymbolObject: public Object
{
public:
	enum ElementTypes{space, sign, word};
protected:
	ElementTypes elementType;
	wchar_t m_code;
	wchar_t symbol;
public:
	SymbolObject();
	SymbolObject(wchar_t *t_text, wchar_t *code):symbol(*t_text), m_code(*code){}
	~SymbolObject(){}
	virtual void setType(ElementTypes t_type);
	virtual ElementTypes getElementType();
	void code(wchar_t *code);
	wchar_t code();
	void setText(wchar_t *t_symbol);
	wchar_t getText() const;
};

class Word: public SymbolObject, public List<SymbolObject*>
{
public:
	Word(){}
	~Word(){}
};

class SymbolLine: public Object , public List<SymbolObject*>
{
protected:
	double m_baseLine;
public:
	SymbolLine();
	SymbolLine(SymbolLine &line);
	~SymbolLine(){}

	double baseLine();
	void baseLine(double t_baseLine);
	virtual void resize();
	virtual SymbolLine* split(double t_x);
	virtual SymbolLine* split(long position);
};

class LineGroup: public Object, public List<SymbolLine*>
{
public:
	enum Align {right, left, middle, justify, unknown};
protected:
	Align m_align;
	double commonVerticalSpace;
public:
	bool flag;// it is worst design ever... bat I didnit have a choice
	LineGroup();
	~LineGroup();
	int align();
	void align(Align t_a);
	double space();
	void space(double t_x);
	virtual void resize();
	virtual LineGroup* split(bool orient, double x, double y);
	virtual void merge(LineGroup* p_group);
};

class GroupList:public Object, public List<LineGroup*>
{
};

class Image: public Object
{
protected:
	CImageInfo info;
public:
	Image(){ type = Object::image;}
	Image(CImageInfo p_info):info(p_info){ type = Object::image;}
	~Image(){}
	void setImageInfo(CImageInfo p_info);
	CImageInfo getImageInfo() const;
};

class GraphicLine: public Object
{
protected:
	Coordinates m_endPoint;
	int m_orient; // 90 - vertical, 0 - horizontal
public:
	GraphicLine(){}
	GraphicLine(Coordinates t_startPoint, Coordinates t_endPoint): m_endPoint(t_endPoint) {m_x = t_startPoint.x(); m_y = t_startPoint.y();}
	~GraphicLine(){}

	void endPoint(Coordinates t_endPoint);
	Coordinates endPoint();
	void setOrientationHint(int orient);
	int orient(){return m_orient;}
};

class Cell: public Object
{
protected:
	Object *m_content;// something...
	long m_rowID, m_columnID;
	bool borders[4];// visability of borders
	int mergeXcount, mergeYcount;
public:
	Cell();
	Cell(long rowID, long columnID, Object* p_content = NULL);
	~Cell(){}

	bool merged;
	void addCell(int, int);
	int cellCountX();
	int cellCountY();
	//virtual bool Merged();
	virtual void resize();
	long rowID();
	void rowID(long t_id);
	long columnID();
	void columnID(long t_id);
	Cell* split(bool orient, double& x, double& y);
	bool setBordersVisability(int number, bool visible);
	void content(Object *t_object);
	Object* content() const;
};

class Table2: public Object
{
protected:
	List<List<Cell*>*> *rows;
	long m_columnCount;
public:
	Table2();
	Table2(long rowCount, long columnCount);
	~Table2();

	List<Cell*>* getRow(long id);
	Iterator<List<Cell*>*>* getRowIterator();
	void addRow();
	void pushRow(List<Cell*>* row);
	bool empty();
	long getCount();
	long columnCount();
	void columnCount(long);
	virtual void resize();
};

class Parser// bad time... bad classes...
{
protected:

	List<Page*> *pages;

	List<Table2*> *graphicTables;
	List<Table2*> *textTables;
	List<GroupList*> *lists;
	List<GraphicLine*> *graphicLines;
	List<SymbolLine*> *symbolLines;
	List<LineGroup*> *groups;
	List<Image*> *images;

	SymbolLine *currentLine;
	Word *currentWord;

	bool thereAreSpaces;
	Coordinates coord;

	BYTE *buffer;
	long pixelWidth;
	long pixelHeight;

	double currentYcoord;
	double maxLineWidth;
	double minSymbolWidth;
	double maxSymbolWidth;
	double verticalLineSpace;
	struct Space
	{
		double space;
		long count;
	};

	struct IntersectionPoint
	{
		GraphicLine *vertical, *horizontal;
		Coordinates coord;
		long columnId, rowId;
	};
	List<IntersectionPoint*> *points;
	struct PointMap
	{
		List<List<IntersectionPoint*>*> rowList;
		void addEmptyPoints()
		{
			long maxPointCount = 0;
			List<IntersectionPoint*> *rowExample;
			Iterator<List<IntersectionPoint*>*> *rowIterator = rowList.getIterator();
			while(rowIterator->getID() < rowList.getCount())
			{
				if(rowIterator->item()->getCount() > maxPointCount)
				{
					rowExample = rowIterator->item();
					maxPointCount = rowIterator->item()->getCount();
				}
				rowIterator->next();
			}
			Iterator<IntersectionPoint*> *examplePointIterator = rowExample->getIterator();
			rowIterator->first();
			while(rowIterator->getID() < rowList.getCount())
			{
				if(rowIterator->item()->getCount() < maxPointCount)
				{
					examplePointIterator->first();
					Iterator<IntersectionPoint*> *pointIterator = rowIterator->item()->getIterator();
					while(examplePointIterator->getID() < maxPointCount)
					{
						bool flag = true;
						pointIterator->first();
						while(flag && pointIterator->getID() < rowIterator->item()->getCount())
						{
							if(pointIterator->item()->coord.x() == examplePointIterator->item()->coord.x())
								flag = false;
							pointIterator->next();
						}
						if(flag)
						{
						//if(pointIterator->item()->coord.x() != examplePointIterator->item()->coord.x())
						//{
							pointIterator->prev();
							IntersectionPoint* point = new IntersectionPoint;
							point->coord.x(examplePointIterator->item()->coord.x());
							point->coord.y(pointIterator->item()->coord.y());
							if(pointIterator->item()->horizontal->x() < point->coord.x() && pointIterator->item()->horizontal->endPoint().x() > point->coord.x())
							{
								point->horizontal = pointIterator->item()->horizontal;
							}
							else
							{
								point->horizontal = NULL;
							}
							//----------------------------
							long id = rowIterator->getID();
							bool stop = false;
							while(!stop)
							{
								if(id == 0)
								{
									rowIterator->next();
								}
								else
								{
									rowIterator->prev();
								}
								Iterator<IntersectionPoint*> *aditionalPointIterator = rowIterator->item()->getIterator();
								while(!stop && aditionalPointIterator->getID() < rowIterator->item()->getCount())
								{
									if(aditionalPointIterator->item()->coord.x() == point->coord.x())
									{
										stop = true;
										if(aditionalPointIterator->item()->vertical->y() < point->coord.y() && aditionalPointIterator->item()->vertical->endPoint().y() > point->coord.y())
											point->vertical = aditionalPointIterator->item()->vertical;
										else
											point->vertical = NULL;
									}
									aditionalPointIterator->next();
								}
							}
							rowIterator->goToID(id);
							rowIterator->item()->push(point);
						//}
						//else
						//{
						//	pointIterator->next();
						}
						examplePointIterator->next();
					}
				}
				rowIterator->next();
			}
		}
		void sort()
		{
			sortRowList();
		}
	protected:
		void sortRowList()
		{
			Iterator<List<IntersectionPoint*>*> *iter1 = rowList.getIterator();
			Iterator<List<IntersectionPoint*>*> *iter2 = rowList.getIterator();
			iter2->next();
			while(iter1->getID() < rowList.getCount())
			{
				sortRow(iter1->item());
				iter1->next();
			}
			iter1->first();
			for(int i = 0; i < rowList.getCount(); i++)
			{
				while(iter2->getID() < rowList.getCount())
				{
					if(iter1->item()->first()->coord.y() > iter2->item()->first()->coord.y())
					{
						List<IntersectionPoint*> *row = iter1->item();
						iter1->item(iter2->item());
						iter2->item(row);
					}
					iter1->next();
					iter2->next();
				}
				iter1->first();
				iter2->first();
				iter2->next();
			}
		}
		/*void sortColumnList()
		{
			//List<List<IntersectionPoint*>*>::
			Iterator<List<IntersectionPoint*>*> *iter1 = columnList.getIterator();
			//List<List<IntersectionPoint*>*>::
			Iterator<List<IntersectionPoint*>*> *iter2 = columnList.getIterator();
			iter2->next();
			//while(iter1->getID() < columnList.getCount())
			//{
			//	sortColumn(iter1->item());
			//	iter1->next();
			//}
			iter1->first();
			for(int i = 0; i < columnList.getCount(); i++)
			{
				while(iter2->getID() < columnList.getCount())
				{
					if(iter1->item()->first()->coord.x() > iter2->item()->first()->coord.x())
					{
						List<IntersectionPoint*> *column = iter1->item();
						iter1->item(iter2->item());
						iter2->item(column);
					}
					iter1->next();
					iter2->next();
				}
				iter1->first();
				iter2->first();
				iter2->next();
			}
		}*/
		void sortRow(List<IntersectionPoint*>* row)
		{
			Iterator<IntersectionPoint*> *iter1 = row->getIterator();
			Iterator<IntersectionPoint*> *iter2 = row->getIterator();
			iter2->next();
			for(int i = 0; i < row->getCount(); i++)
			{
				while(iter2->getID() < row->getCount())
				{
					if(iter1->item()->coord.x() > iter2->item()->coord.x())
					{
						IntersectionPoint *point = iter1->item();
						iter1->item(iter2->item());
						iter2->item(point);
						iter1->item()->columnId = iter1->getID();
						iter2->item()->columnId = iter2->getID();
					}
					iter1->next();
					iter2->next();
				}
				iter1->first();
				iter2->first();
				iter2->next();
			}			
		}

		/*void sortColumn(List<IntersectionPoint*>* column)
		{
			//List<IntersectionPoint*>::
			Iterator<IntersectionPoint*> *iter1 = column->getIterator();
			//List<IntersectionPoint*>::
			Iterator<IntersectionPoint*> *iter2 = column->getIterator();
			iter2->next();
			for(int i = 0; i < column->getCount(); i++)
			{
				while(iter2->getID() < column->getCount())
				{
					if(iter1->item()->coord.y() > iter2->item()->coord.y())
					{
						IntersectionPoint *point = iter1->item();
						iter1->item(iter2->item());
						iter2->item(point);
						iter1->item()->rowId = iter1->getID();
						iter2->item()->rowId = iter2->getID();
					}
					iter1->next();
					iter2->next();
				}
				iter1->first();
				iter2->first();
				iter2->next();
			}			
		}*/
	} *pointMap;

	void createGraphicLines(long x, long y, long r, long b, bool &t);
	void findIntersections();
	void createPointMap();
	void createTable();
	void fillTables();
	bool findList(LineGroup* group, bool stage = false);
	void findGroup(SymbolLine *line);
	void findIntersectionPoint(GraphicLine *p_line1, GraphicLine *p_line2);
	bool findCell(Object* obj, Table2 *table);

	
public:
	Parser():currentLine(new SymbolLine), symbolLines(new List<SymbolLine*>), currentWord(new Word),
	pointMap(new PointMap), pages(new List<Page*>), images(new List<Image*>),
	groups(new List<LineGroup*>), graphicLines(new List<GraphicLine*>), points(new List<IntersectionPoint*>), lists(new List<GroupList*>),
	graphicTables(new List<Table2*>), textTables(new List<Table2*>), 
	maxLineWidth(0.0), minSymbolWidth(100.0), currentYcoord(0.0), verticalLineSpace(10.0), thereAreSpaces(false),
	maxSymbolWidth(0.0){currentWord->setType(SymbolObject::word);}//sheet happens

	~Parser(){delete currentLine; delete images;
	delete pointMap; delete groups; delete currentWord;
	delete graphicLines; delete symbolLines; delete points;}

	void clear();
	
	virtual void lineTo(double& x, double& y);
	virtual void curve(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3){}
	virtual void moveTo(double& x, double& y);
	virtual void image(CImageInfo p_ibfo, double& x, double& y, double& width, double& height);
	virtual void text(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset);
	virtual void beginPage();
	virtual void endPage();

	Page* finishPage();

	void setPixelBuffer(BYTE *buffer);
	void setPagePixelSize(long width, long height);
	bool analizeGraphic(long x, long y, long width, long height);

	void toTXT();// for test only
	void toHTML();// for test only
};

class Page
{
public:
	List<Object*> *items;
	
	List<Table2*> *tables;
	List<GroupList*> *lists;
	List<LineGroup*> *groups;
	List<GraphicLine*> *lines;
	List<Image*> *images;

	void createCommonList();
};

