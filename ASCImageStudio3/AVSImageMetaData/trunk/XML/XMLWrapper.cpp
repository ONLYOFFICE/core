//---------------------------------------------------------------------------

#pragma hdrstop

#include "stdafx.h"
#include "XMLWrapper.h"
//---------------------------------------------------------------------------

using namespace XML;
//---------------------------------------------------------------------------

XMLDocumentWrapper::XMLDocumentWrapper()
{
    if ( FAILED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_ALL,
                                        IID_IXMLDOMDocument, (void **) &m_pXmlDoc)) )
    {
        m_pXmlDoc = NULL;
    }
}
//---------------------------------------------------------------------------

XMLDocumentWrapper::XMLDocumentWrapper(IXMLDOMDocument * pDoc, bool addRef)
{
    if ( ((m_pXmlDoc = pDoc) != 0) && addRef )
        m_pXmlDoc->AddRef();
}
//---------------------------------------------------------------------------

XMLDocumentWrapper::XMLDocumentWrapper(const XMLDocumentWrapper& Doc)
{
    if ( (m_pXmlDoc = Doc.m_pXmlDoc) != 0 )
        m_pXmlDoc->AddRef();
}
//---------------------------------------------------------------------------

XMLDocumentWrapper& XMLDocumentWrapper::operator=(IXMLDOMDocument * pDoc)
{
    if ( m_pXmlDoc )
        m_pXmlDoc->Release();

    m_pXmlDoc = pDoc;

    return *this;
}
//---------------------------------------------------------------------------

XMLDocumentWrapper& XMLDocumentWrapper::operator=(const XMLDocumentWrapper& Doc)
{
    if ( Doc.m_pXmlDoc )
        Doc.m_pXmlDoc->AddRef();

    if ( m_pXmlDoc )
        m_pXmlDoc->Release();

    m_pXmlDoc = Doc.m_pXmlDoc;

    return *this;
}
//---------------------------------------------------------------------------

XMLDocumentWrapper::~XMLDocumentWrapper()
{
    if ( m_pXmlDoc )
        m_pXmlDoc->Release();
}
//---------------------------------------------------------------------------

bool XMLDocumentWrapper::Load(BSTR path)
{
    if ( ! m_pXmlDoc )
        return false;

    VARIANT vtPath;
    vtPath.vt = VT_BSTR;
    vtPath.bstrVal = SysAllocString(path);

    VARIANT_BOOL success;

    try
    {
        m_pXmlDoc->put_async(VARIANT_FALSE);
        m_pXmlDoc->load(vtPath, &success);
    }
    catch(...)
    {
    }

    VariantClear(&vtPath);


    return (VARIANT_TRUE == success);
}
//---------------------------------------------------------------------------

bool XMLDocumentWrapper::LoadXML(BSTR xml)
{
    if ( ! xml )
    {
        Clear();
        return true;
    }

    VARIANT_BOOL success = VARIANT_FALSE;

    if ( m_pXmlDoc )
    {
        try
        {
            m_pXmlDoc->loadXML(xml, &success);
        }
        catch(...)
        {
        }
    }

    return (VARIANT_TRUE == success);
}
//---------------------------------------------------------------------------

bool XMLDocumentWrapper::Save(BSTR path)
{
    if ( ! m_pXmlDoc )
        return false;

    VARIANT vtPath;
    vtPath.vt = VT_BSTR;

    bool success = false;

    try
    {
        if ( 0 == SysStringLen(path) )
        {
            m_pXmlDoc->get_url(&vtPath.bstrVal);
        }
        else
        {
            vtPath.bstrVal = SysAllocString(path);
        }

        success = SUCCEEDED(m_pXmlDoc->save(vtPath));
    }
    catch(...)
    {
    }

    VariantClear(&vtPath);


    return success;
}
//---------------------------------------------------------------------------

bool XMLDocumentWrapper::GetXML(BSTR * xmlString)
{
    if ( ! m_pXmlDoc )
        return false;

    return SUCCEEDED(m_pXmlDoc->get_xml(xmlString));
}
//---------------------------------------------------------------------------

void XMLDocumentWrapper::Clear()
{
    if ( m_pXmlDoc )
    {
        IXMLDOMElement * pRootNode = NULL;
        m_pXmlDoc->get_documentElement(&pRootNode);

        if ( pRootNode )
        {
            IXMLDOMNode * pNode = NULL;
            m_pXmlDoc->removeChild(pRootNode, &pNode);

            if ( pNode )
                pNode->Release();

            pRootNode->Release();
        }
    }
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLDocumentWrapper::GetRootNode()
{
    IXMLDOMElement * pNode = NULL;

    if ( m_pXmlDoc )
        m_pXmlDoc->get_documentElement(&pNode);

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLDocumentWrapper::AppendNode(BSTR nodeName)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlDoc )
    {
        VARIANT vtType;
        vtType.vt = VT_I2;
        vtType.iVal = NODE_ELEMENT;

        IXMLDOMNode * pNewNode = NULL;
        m_pXmlDoc->createNode(vtType, nodeName, NULL, &pNewNode);

        if ( pNewNode )
        {
            m_pXmlDoc->appendChild(pNewNode, &pNode);
            pNewNode->Release();
        }
    }

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLDocumentWrapper::CreateNode(BSTR nodeName)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlDoc )
    {
        VARIANT vtType;
        vtType.vt   = VT_I2;
        vtType.iVal = NODE_ELEMENT;

        m_pXmlDoc->createNode(vtType, nodeName, NULL, &pNode);
    }

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

XMLNodeWrapper::XMLNodeWrapper() : m_pXmlNode(NULL)
{
}
//---------------------------------------------------------------------------

XMLNodeWrapper::XMLNodeWrapper(IXMLDOMNode * pNode, bool addRef)
{
    if ( ((m_pXmlNode = pNode) != 0) && addRef )
        m_pXmlNode->AddRef();
}
//---------------------------------------------------------------------------

XMLNodeWrapper::XMLNodeWrapper(const XMLNodeWrapper& Node)
{
    if ( (m_pXmlNode = Node.m_pXmlNode) != 0 )
        m_pXmlNode->AddRef();
}
//---------------------------------------------------------------------------

XMLNodeWrapper::~XMLNodeWrapper()
{
    if ( m_pXmlNode )
        m_pXmlNode->Release();
}
//---------------------------------------------------------------------------

XMLNodeWrapper& XMLNodeWrapper::operator=(IXMLDOMNode * pNode)
{
    if ( m_pXmlNode )
        m_pXmlNode->Release();

    m_pXmlNode = pNode;

    return *this;
}
//---------------------------------------------------------------------------

XMLNodeWrapper& XMLNodeWrapper::operator=(const XMLNodeWrapper& Node)
{
    if ( Node.m_pXmlNode )
      Node.m_pXmlNode->AddRef();

    if ( m_pXmlNode )
        m_pXmlNode->Release();

    m_pXmlNode = Node.m_pXmlNode;

    return *this;
}
//---------------------------------------------------------------------------

int XMLNodeWrapper::GetNodesCount()
{
    long length = 0;

    if ( m_pXmlNode )
    {
        IXMLDOMNodeList * pNodeList = NULL;
        m_pXmlNode->get_childNodes(&pNodeList);

        if ( pNodeList )
        {
            pNodeList->get_length(&length);
            pNodeList->Release();
        }
    }

    return length;
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::GetParent()
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
        m_pXmlNode->get_parentNode(&pNode);

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetName(BSTR * name)
{
    if ( ! m_pXmlNode )
        return false;

    return SUCCEEDED(m_pXmlNode->get_baseName(name));
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::GetNode(BSTR nodeName)
{
    IXMLDOMNode * pNode = NULL;

    try
    {
        if ( m_pXmlNode )
            m_pXmlNode->selectSingleNode(nodeName, &pNode);
    }
    catch(...)
    {
    }

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::GetNode(int nodeIndex)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
    {
        IXMLDOMNodeList * pNodeList = NULL;
        if ( S_OK == m_pXmlNode->get_childNodes(&pNodeList) && pNodeList )
        {
            pNodeList->get_item(nodeIndex, &pNode);
            pNodeList->Release();
        }
    }

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::FindNode(BSTR searchString)
{
    return GetNode(searchString);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::GetPreviousSibling()
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
        m_pXmlNode->get_previousSibling(&pNode);

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::GetNextSibling()
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
        m_pXmlNode->get_nextSibling(&pNode);

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetXML(BSTR * xmlString)
{
    if ( ! m_pXmlNode )
        return false;

    return SUCCEEDED(m_pXmlNode->get_xml(xmlString));
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetText(BSTR * text)
{
    if ( ! m_pXmlNode )
        return false;

    return SUCCEEDED(m_pXmlNode->get_text(text));
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::SetText(BSTR text)
{
    if ( ! m_pXmlNode )
        return false;     

    return SUCCEEDED(m_pXmlNode->put_text(text));
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::AppendNode(BSTR nodeName)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
    {
        IXMLDOMDocument * pDoc = NULL;
        m_pXmlNode->get_ownerDocument(&pDoc);

        if ( pDoc )
        {
            VARIANT vtType;
            vtType.vt = VT_I2;
            vtType.iVal = NODE_ELEMENT;

            IXMLDOMNode * pNewNode = NULL;
            pDoc->createNode(vtType, nodeName, NULL, &pNewNode);

            if ( pNewNode )
            {
                m_pXmlNode->appendChild(pNewNode, &pNode);
                pNewNode->Release();
            }
            pDoc->Release();
        }
    }

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::AppendNode(const XMLNodeWrapper& Node)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
    {
        if ( Node.m_pXmlNode )
        {
            IXMLDOMNode * pNewNode = NULL;
            Node.m_pXmlNode->cloneNode(VARIANT_TRUE, &pNewNode);

            if ( pNewNode )
            {
                m_pXmlNode->appendChild(pNewNode, &pNode);
                pNewNode->Release();
            }
        }
    }

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeWrapper::RemoveNode(const XMLNodeWrapper& Node)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pXmlNode )
        m_pXmlNode->removeChild(Node.m_pXmlNode, &pNode);

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

void XMLNodeWrapper::RemoveNodes(BSTR searchString)
{
    if ( ! m_pXmlNode )
        return;

    IXMLDOMNodeList * pNodeList = NULL;
    m_pXmlNode->selectNodes(searchString, &pNodeList);

    if ( pNodeList )
    {
        long cntNodes;
        pNodeList->get_length(&cntNodes);
        
        try
        {
            for ( int i = 0; i < cntNodes; ++i )
            {
                IXMLDOMNode * pNode = NULL;
                pNodeList->get_item(i, &pNode);

                if ( pNode )
                {
                    IXMLDOMNode * pParentNode = NULL;
                    pNode->get_parentNode(&pParentNode);

                    if ( pParentNode )
                    {
                        pParentNode->removeChild(pNode, NULL);
                        pParentNode->Release();
                    }

                    pNode->Release();
                }
            }
        }
        catch(...)
        {
        }

        pNodeList->Release();
    }
}
//---------------------------------------------------------------------------

XMLNodeListWrapper XMLNodeWrapper::FindNodes(BSTR searchString)
{
    IXMLDOMNodeList * pNodeList = NULL;

    if ( m_pXmlNode )
        m_pXmlNode->selectNodes(searchString, &pNodeList);

    return XMLNodeListWrapper(pNodeList);
}
//---------------------------------------------------------------------------

int    XMLNodeWrapper::GetAttributesCount()
{
    long count = 0;

    if ( m_pXmlNode )
    {
        IXMLDOMNamedNodeMap * pAttribs = NULL;
        m_pXmlNode->get_attributes(&pAttribs);

        if ( pAttribs )
        {
            pAttribs->get_length(&count);
            pAttribs->Release();
        }
    }

    return count;
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetAttributeValue(int index, BSTR * value)
{
    HRESULT hRes = E_FAIL;

    if ( m_pXmlNode )
    {
        IXMLDOMNamedNodeMap * pAttribs = NULL;
        m_pXmlNode->get_attributes(&pAttribs);

        if ( pAttribs )
        {
            IXMLDOMNode * pNode = NULL;
            pAttribs->get_item(index, &pNode);

            if ( pNode )
            {
                IXMLDOMAttribute * pAttr = NULL;
                pNode->QueryInterface(IID_IXMLDOMAttribute, (void **) &pAttr);

                if ( pAttr )
                {
                    hRes = pAttr->get_text(value);
                    pAttr->Release();
                }

                pNode->Release();
            }

            pAttribs->Release();
        }
    }

    return SUCCEEDED(hRes);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetAttributeValue(int index, tagVARIANT * value)
{
    HRESULT hRes = E_FAIL;

    if ( m_pXmlNode )
    {
        IXMLDOMNamedNodeMap * pAttribs = NULL;
        m_pXmlNode->get_attributes(&pAttribs);

        if ( pAttribs )
        {
            IXMLDOMNode * pNode = NULL;
            pAttribs->get_item(index, &pNode);

            if ( pNode )
            {
                IXMLDOMAttribute * pAttr = NULL;
                pNode->QueryInterface(IID_IXMLDOMAttribute, (void **) &pAttr);

                if ( pAttr )
                {
                    hRes = pAttr->get_value(value);
                    pAttr->Release();
                }

                pNode->Release();
            }

            pAttribs->Release();
        }
    }

    return SUCCEEDED(hRes);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetAttributeName(int index, BSTR * name)
{
    HRESULT hRes = E_FAIL;

    if ( m_pXmlNode )
    {
        IXMLDOMNamedNodeMap * pAttribs = NULL;
        m_pXmlNode->get_attributes(&pAttribs);

        if ( pAttribs )
        {
            IXMLDOMNode * pNode = NULL;
            pAttribs->get_item(index, &pNode);

            if ( pNode )
            {
                IXMLDOMAttribute * pAttr = NULL;
                pNode->QueryInterface(IID_IXMLDOMAttribute, (void **) &pAttr);

                if ( pAttr )
                {
                    hRes = pAttr->get_name(name);
                    pAttr->Release();
                }

                pNode->Release();
            }

            pAttribs->Release();
        }
    }

    return SUCCEEDED(hRes);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetAttributeValue(BSTR valueName, BSTR * value, BSTR defaultValue)
{
    HRESULT hRes = E_FAIL;

    if ( m_pXmlNode )
    {
        IXMLDOMNamedNodeMap * pAttribs = NULL;
        m_pXmlNode->get_attributes(&pAttribs);

        if ( pAttribs )
        {
            IXMLDOMNode * pNode = NULL;
            pAttribs->getNamedItem(valueName, &pNode);

            if ( pNode )
            {
                hRes = pNode->get_text(value);
                pNode->Release();
            }

            pAttribs->Release();
        }
    }

    if ( S_OK != hRes )
    {
        //SysFreeString(*value);
        *value = SysAllocString(defaultValue);
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::GetAttributeValue(BSTR valueName, tagVARIANT * value)
{
    HRESULT hRes = E_FAIL;

    if ( m_pXmlNode )
    {
        IXMLDOMElement * pElem = NULL;
        m_pXmlNode->QueryInterface(IID_IXMLDOMElement, (void **) &pElem);

        if ( pElem )
        {
            hRes = pElem->getAttribute(valueName, value);
            pElem->Release();
        }
    }

    return SUCCEEDED(hRes);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::SetAttributeValue(BSTR valueName, tagVARIANT value)
{
    HRESULT hRes = E_FAIL;

    if ( m_pXmlNode )
    {
        IXMLDOMElement * pElem = NULL;
        m_pXmlNode->QueryInterface(IID_IXMLDOMElement, (void **) &pElem);

        if ( pElem )
        {
            hRes = pElem->setAttribute(valueName, value);
            pElem->Release();
        }
    }

    return SUCCEEDED(hRes);
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::SetAttributeValue(BSTR valueName, int value)
{
    VARIANT vtValue;
    vtValue.vt = VT_I4;
    vtValue.lVal = value;

    bool bRes = SetAttributeValue(valueName, vtValue);

    VariantClear(&vtValue);
    return bRes;
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::SetAttributeValue(BSTR valueName, double value)
{
    VARIANT vtValue;
    vtValue.vt = VT_R8;
    vtValue.dblVal = value;

    bool bRes = SetAttributeValue(valueName, vtValue);

    VariantClear(&vtValue);
    return bRes;
}
//---------------------------------------------------------------------------

bool XMLNodeWrapper::SetAttributeValue(BSTR valueName, BSTR value)
{
    VARIANT vtValue;
    vtValue.vt = VT_BSTR;
    vtValue.bstrVal = SysAllocString(value);

    bool bRes = SetAttributeValue(valueName, vtValue);

    VariantClear(&vtValue);
    return bRes;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

XMLNodeListWrapper::XMLNodeListWrapper() : m_pNodeList(NULL)
{
}
//---------------------------------------------------------------------------

XMLNodeListWrapper::XMLNodeListWrapper(IXMLDOMNodeList * pNodeList, bool addRef)
{
    if ( ((m_pNodeList = pNodeList) != 0) && addRef )
        m_pNodeList->AddRef();
}
//---------------------------------------------------------------------------

XMLNodeListWrapper::XMLNodeListWrapper(const XMLNodeListWrapper& NodeList)
{
    if ( (m_pNodeList = NodeList.m_pNodeList) != 0 )
        m_pNodeList->AddRef();
}
//---------------------------------------------------------------------------

XMLNodeListWrapper::~XMLNodeListWrapper()
{
    if ( m_pNodeList )
        m_pNodeList->Release();
}
//---------------------------------------------------------------------------

XMLNodeListWrapper& XMLNodeListWrapper::operator=(IXMLDOMNodeList * pNodeList)
{
    if ( m_pNodeList )
        m_pNodeList->Release();

    m_pNodeList = pNodeList;

    return *this;
}
//---------------------------------------------------------------------------

XMLNodeListWrapper& XMLNodeListWrapper::operator=(const XMLNodeListWrapper& NodeList)
{
    if ( NodeList.m_pNodeList )
        NodeList.m_pNodeList->AddRef();

    if ( m_pNodeList )
        m_pNodeList->Release();

    m_pNodeList = NodeList.m_pNodeList;

    return *this;
}
//---------------------------------------------------------------------------

int XMLNodeListWrapper::GetCount()
{
    long length = 0;

    if ( m_pNodeList )
        m_pNodeList->get_length(&length);

    return length;
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeListWrapper::GetNode(int index)
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pNodeList )
        m_pNodeList->get_item(index, &pNode);

    return XMLNodeWrapper(pNode);
}
//---------------------------------------------------------------------------

bool XMLNodeListWrapper::Start()
{
    if ( ! m_pNodeList )
        return false;

    return SUCCEEDED(m_pNodeList->reset());
}
//---------------------------------------------------------------------------

XMLNodeWrapper XMLNodeListWrapper::Next()
{
    IXMLDOMNode * pNode = NULL;

    if ( m_pNodeList )
        m_pNodeList->nextNode(&pNode);

    return XMLNodeWrapper(pNode);
}

//---------------------------------------------------------------------------

void XML::DeleteEmptyNodes(XMLNodeWrapper node)
{
    for ( int i = node.GetNodesCount() - 1; i >= 0; --i )
    {
        XMLNodeWrapper currNode = node.GetNode(i);

        DeleteEmptyNodes(currNode);

        wchar_t * text = NULL;
        currNode.GetText(&text);

        if ( currNode.GetNodesCount() == 0 && currNode.GetAttributesCount() == 0 && SysStringLen(text) == 0 )
        {
            node.RemoveNode(currNode);
        }

        SysFreeString(text);
    }
}
//---------------------------------------------------------------------------



