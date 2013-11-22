//---------------------------------------------------------------------------

#ifndef XMLWrapperH
#define XMLWrapperH
//---------------------------------------------------------------------------

#include <msxml.h>
//---------------------------------------------------------------------------

namespace XML
{
    class XMLNodeWrapper;
    class XMLNodeListWrapper;


    class XMLDocumentWrapper
    {
    public:

        XMLDocumentWrapper();
        XMLDocumentWrapper(IXMLDOMDocument * pDoc, bool addRef = false);
        XMLDocumentWrapper(const XMLDocumentWrapper& Doc);

        virtual ~XMLDocumentWrapper();

        XMLDocumentWrapper& operator=(IXMLDOMDocument * pDoc);
        XMLDocumentWrapper& operator=(const XMLDocumentWrapper& Doc);
        operator bool() const { return (m_pXmlDoc != 0); }

        bool Load(BSTR path);
        bool LoadXML(BSTR xml);
        bool Save(BSTR path = L"");
        bool GetXML(BSTR * xmlString);
        void Clear();

        XMLNodeWrapper GetRootNode();
        XMLNodeWrapper AppendNode(BSTR nodeName);

        XMLNodeWrapper CreateNode(BSTR nodeName);

    private:

        IXMLDOMDocument * m_pXmlDoc;
    };


    class XMLNodeWrapper
    {
    public:

        XMLNodeWrapper();
        XMLNodeWrapper(IXMLDOMNode * pNode, bool addRef = false);
        XMLNodeWrapper(const XMLNodeWrapper& Node);

        virtual ~XMLNodeWrapper();

        XMLNodeWrapper& operator=(IXMLDOMNode * pNode);
        XMLNodeWrapper& operator=(const XMLNodeWrapper& Node);
        operator bool() const { return (m_pXmlNode != 0); }

        int             GetNodesCount();

        XMLNodeWrapper  GetNode(BSTR nodeName);
        XMLNodeWrapper  GetNode(int nodeIndex);
        XMLNodeWrapper  FindNode(BSTR searchString);
        XMLNodeWrapper  GetPreviousSibling();
        XMLNodeWrapper  GetNextSibling();
        XMLNodeWrapper  GetParent();

        bool            GetXML(BSTR * xmlString);
        bool            GetText(BSTR * text);
        bool            SetText(BSTR text);

        bool            GetName(BSTR * name);

        XMLNodeWrapper  AppendNode(BSTR nodeName);
        XMLNodeWrapper  AppendNode(const XMLNodeWrapper& Node);

        XMLNodeWrapper  RemoveNode(const XMLNodeWrapper& Node);
        void            RemoveNodes(BSTR searchString);

        XMLNodeListWrapper FindNodes(BSTR searchString);


        int             GetAttributesCount();

        bool            GetAttributeValue(int index, BSTR * value);
        bool            GetAttributeName(int index, BSTR * name);
        bool            GetAttributeValue(int index, tagVARIANT * value);
        bool            GetAttributeValue(BSTR valueName, tagVARIANT * value);
        bool            GetAttributeValue(BSTR valueName, BSTR * value, BSTR defaultValue = L"");

        bool            SetAttributeValue(BSTR valueName, tagVARIANT value);
        bool            SetAttributeValue(BSTR valueName, int value);
        bool            SetAttributeValue(BSTR valueName, double value);
        bool            SetAttributeValue(BSTR valueName, BSTR value);

    private:

        IXMLDOMNode * m_pXmlNode;
    };


    class XMLNodeListWrapper
    {
    public:

        XMLNodeListWrapper();
        XMLNodeListWrapper(IXMLDOMNodeList * pNodeList, bool addRef = false);
        XMLNodeListWrapper(const XMLNodeListWrapper& NodeList);

        virtual ~XMLNodeListWrapper();

        XMLNodeListWrapper& operator=(IXMLDOMNodeList * pNodeList);
        XMLNodeListWrapper& operator=(const XMLNodeListWrapper& NodeList);
        operator bool() const { return (m_pNodeList != 0); }

        int             GetCount();
        XMLNodeWrapper  GetNode(int index);

        bool            Start();
        XMLNodeWrapper  Next();

    private:

        IXMLDOMNodeList * m_pNodeList;
    };


    void DeleteEmptyNodes(XMLNodeWrapper node);

}; // namespace XML
//---------------------------------------------------------------------------
#endif
