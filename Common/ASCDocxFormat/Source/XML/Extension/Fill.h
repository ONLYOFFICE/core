#pragma once
#ifndef XML_FILL_INCLUDE_H_
#define XML_FILL_INCLUDE_H_

#include "property.h"
#include "./../XElement.h"
#include "./../Private/XElementContainer.h"


namespace XML
{

	template<class T>
	void Fill(T& container, const XML::XElement& element)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			container.push_back(T::value_type(XElement(*i)));
			container.back().this_is_not_xobject_class();
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			container->push_back(T::value_type(XElement(*i)));
			container->back().this_is_not_xobject_class();
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			if (XElement(*i)->XName == name)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			if (XElement(*i)->XName == name)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7, const std::string& name8)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7 || name == name8)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7, const std::string& name8)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7 || name == name8)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7, const std::string& name8, const std::string& name9)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7 || name == name8 || name == name9)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7, const std::string& name8, const std::string& name9)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7 || name == name8 || name == name9)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(T& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7, const std::string& name8, const std::string& name9, const std::string& name10)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7 || name == name8 || name == name9 || name == name10)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T>
	void Fill(property<T>& container, const XML::XElement& element, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4, const std::string& name5, const std::string& name6, const std::string& name7, const std::string& name8, const std::string& name9, const std::string& name10)
	{
		for (Private::XElementContainer::const_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const std::string name = XElement(*i)->XName->Name;
			if (name == name1 || name == name2 || name == name3 || name == name4 || name == name5 || name == name6 || name == name7 || name == name8 || name == name9 || name == name10)
			{
				container->push_back(T::value_type(XElement(*i)));
				container->back().this_is_not_xobject_class();
			}
		}
	}

} // namespace XML

#endif // XML_FILL_INCLUDE_H_