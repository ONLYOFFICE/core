#ifndef _CPDOCCORE_ODF_OFFCIE_ELEMENTS_CREATE_H_
#define _CPDOCCORE_ODF_OFFCIE_ELEMENTS_CREATE_H_

#include <string>
#include <map>

#include <cpdoccore/CPSharedPtr.h>
#include "office_elements.h"
#include <boost/function.hpp>

namespace xml
{
    class sax;
}

namespace cpdoccore { 
namespace odf {

class document_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

#define CP_REGISTER_OFFICE_ELEMENT2(A) \
namespace { \
    RegisterElement<A> RegisterElement##A;\
}

class office_element_creator
{
public:
    typedef boost::function< office_element_ptr () > CreateFuncImpl;
        
public:
    // Зарегестрировать элемент
    bool register_element(const std::wstring &ns, const std::wstring & name, CreateFuncImpl f);

    // Создать элемент по имени
    office_element_ptr create(const ::std::wstring & ns, const ::std::wstring & name, document_context * Context = NULL, bool isRoot = false) const;

private:
    typedef std::map<::std::wstring, CreateFuncImpl> MapType;
    MapType map_; 
            
public:
    // Обеспечение синглтона
    static office_element_creator * get();

private:
    office_element_creator();
    static office_element_creator * instance_;
};

/// Класс, обеспечивающий регистрацию элементов
template <class T>
class RegisterElement
{
private:
    template <class T>
    class CreateImpl_
    {
    public:
        static typename shared_ptr<T>::Type create()
        {
            //return shared_ptr<T>::Type( new T() );
            return boost::make_shared<T>();
        }
    };

public:
    RegisterElement()
    {
        if (class_registered_++ == 0)
        {
            // Jerry Schwarz counter
            office_element_creator::get()->register_element(T::ns, T::name, &CreateImpl_<T>::create);            
        }
    };

    ~RegisterElement()
    {            
    };
private:
    static int class_registered_;    
};

template<class T> int RegisterElement<T>::class_registered_ = 0;

/// \brief  Создать элемент и в случае успеха прочитать его содержимое из SAX, поместить в shared_ptr
bool create_element_and_read(xml::sax * Reader,
                             const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr & _Element,
                             document_context * Context,
                             bool isRoot = false);

/// \brief  Создать элемент и в случае успеха прочитать его содержимое из SAX, поместить в array
bool create_element_and_read(xml::sax * Reader,
                             const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr_array & _Elements,
                             document_context * Context,
                             bool isRoot = false);


#define CP_CREATE_ELEMENT_SIMPLE(ELEMENT) create_element_and_read(Reader, Ns, Name, (ELEMENT), Context)
#define CP_CREATE_ELEMENT(ELEMENT) create_element_and_read(Reader, Ns, Name, (ELEMENT), getContext())
#define _CPDOCCORE_CREATE_ELEMENT_ROOT(ELEMENT) create_element_and_read(Reader, Ns, Name, (ELEMENT), getContext(), true)

#define CP_CHECK_NAME(NS, NAME) ((NS) == Ns && (NAME) == Name)

void not_applicable_element(const office_element * CurrentElm, xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
void not_applicable_element(const std::wstring & Current, xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

void not_applicable_element(const office_element * CurrentElm, const std::wstring & Ns, const std::wstring & Name);
void not_applicable_element(const std::wstring & Current, const std::wstring & Ns, const std::wstring & Name);

#define CP_NOT_APPLICABLE_ELM() \
    not_applicable_element(this, Reader, Ns, Name)

#define CP_NOT_APPLICABLE_ELM_SIMPLE(A) \
    not_applicable_element(A, Reader, Ns, Name)


}
}

#endif // #ifndef _CPDOCCORE_ODF_OFFCIE_ELEMENTS_CREATE_H_
