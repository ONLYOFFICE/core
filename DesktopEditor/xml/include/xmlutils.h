#ifndef _BUILD_XMLUTILS_CROSSPLATFORM_H_
#define _BUILD_XMLUTILS_CROSSPLATFORM_H_

#include <vector>
#include <map>
#include <string>

#include "../../common/File.h"
#include "../../common/StringBuilder.h"

namespace XmlUtils
{
	typedef
	enum XmlNodeType
	{
		XmlNodeType_None = 0,
		XmlNodeType_Element = 1,
		XmlNodeType_Attribute = 2,
		XmlNodeType_Text = 3,
		XmlNodeType_CDATA = 4,
		XmlNodeType_ENTITY_REFERENCE = 5,
		XmlNodeType_ENTITY = 6,
		XmlNodeType_ProcessingInstruction = 7,
		XmlNodeType_Comment = 8,
		XmlNodeType_Document = 9,
		XmlNodeType_DocumentType = 10,
		XmlNodeType_DOCUMENT_FRAGMENT = 11,
		XmlNodeType_NOTATION = 12,
		XmlNodeType_Whitespace = 13,
		XmlNodeType_SIGNIFICANT_WHITESPACE = 14,
		XmlNodeType_EndElement = 15,
		XmlNodeType_TYPE_END_ENTITY = 16,
		XmlNodeType_XmlDeclaration = 17,
		_XmlNodeType_Last = 17
	} XmlNodeType;

	class CXmlLiteReader_Private;
	class CXmlLiteReader
	{
	protected:
		CXmlLiteReader_Private* m_pInternal;
	public:

		CXmlLiteReader();
		virtual ~CXmlLiteReader();
		
	public:

		void Clear();
		bool IsValid();

		bool FromFile(const wchar_t* sFilePath);
		bool FromFile(const std::wstring& sFilePath);
		bool FromString(const wchar_t* sXml);
		bool FromString(const std::wstring& sXml);
		bool FromStringA(const std::string& sXml);

		bool Read(XmlNodeType &oNodeType);
		bool ReadNextNode();
		bool ReadNextSiblingNode(int nDepth);
		bool ReadNextSiblingNode2(int nDepth);
		bool ReadTillEnd(int nDepth = -2);
		
		std::wstring GetName();
		std::string GetNameA();
		int GetDepth();
		bool IsEmptyNode();

		std::wstring GetText();
		std::string GetTextA();

		std::wstring GetText2();
		std::string GetText2A();
		
		std::wstring GetOuterXml();
		std::wstring GetInnerXml();

		int GetAttributesCount();
		bool MoveToFirstAttribute();
		bool MoveToNextAttribute();
		bool MoveToElement();

		bool IsEmptyElement();
	};

	class IXmlDOMDocument
	{
	private:
		unsigned int m_lRef;

	public:
		IXmlDOMDocument()
		{
			m_lRef = 1;
		}
		virtual ~IXmlDOMDocument()
		{
		}

		virtual unsigned int AddRef()
		{
			++m_lRef;
			return m_lRef;
		}
		virtual unsigned int Release()
		{
			unsigned int lReturn = --m_lRef;
			if (0 == m_lRef)
				delete this;
			return lReturn;
		}
	};

	class CXmlNodeBase : public IXmlDOMDocument
	{
	public:
		IXmlDOMDocument* m_pDocument;

		std::map<std::string, std::string> m_attributes;
		std::vector<CXmlNodeBase*> m_nodes;
		std::wstring m_sText;
		std::wstring m_sName;

	public:
		CXmlNodeBase();
		~CXmlNodeBase();

		void AddRefDoc();
		void ReleaseDoc();

		std::wstring GetXml();
		void GetXml(NSStringUtils::CStringBuilder& oWriter);
	};

	class CXmlNode;
	class CXmlNodes
	{
	private:
		std::vector<CXmlNode> m_nodes;

	public:
		CXmlNodes();
		bool IsValid();
		int GetCount();
		bool GetAt(int nIndex, CXmlNode& oXmlNode);

		friend class CXmlNode;
	};


	class CXmlNode
	{
	private:
		CXmlNodeBase* m_pBase;

	public:
		CXmlNode();
		CXmlNode(const CXmlNode& oSrc);
		~CXmlNode();
		
	public:
		bool FromXmlFile(const wchar_t* sFile, bool bRemoveRootNode = false);
		bool FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode = false);
		
		bool FromXmlStringA(const std::string& sString);
		bool FromXmlString(const wchar_t* sString);
		bool FromXmlString(const std::wstring& sString);

		bool IsValid() const;
		void Clear();
		std::wstring GetName();
		std::wstring GetText();
		bool GetTextIfExist(std::wstring& sOutput);
		std::wstring GetTextExt(const std::wstring& strDefaultValue = L"");
		std::wstring GetXml(const std::wstring& strDefaultValue = L"");

		std::wstring ReadAttributeBase(const wchar_t* bstrName);		
		std::wstring ReadAttribute(const std::wstring& strAttibuteName);
		
		std::string GetAttributeA(const std::string& sName, const std::string& _default = "");
		std::string GetAttributeA(const std::wstring& sName, const std::string& _default = "");

		std::wstring GetAttribute(const std::string& sName, const std::wstring& _default = L"");
		std::wstring GetAttribute(const std::wstring& sName, const std::wstring& _default = L"");
		std::wstring GetAttribute(const wchar_t* sName, const std::wstring& _default = L"");

		std::wstring GetAttributeBase(const wchar_t* strAttributeName, const std::wstring& strDefaultValue = L"");
		bool GetAttributeIfExist(const std::wstring& sName, std::wstring& sOutput);

		int GetAttributeInt(const std::string& sName, const int& _default = 0);
		int GetAttributeInt(const std::wstring& sName, const int& _default = 0);
		int ReadAttributeInt(const std::wstring& str, const int& nDef = 0);
		double ReadAttributeDouble(const std::wstring& str, const double& nDef = 0);

		double GetAttributeDouble(const std::string& sName, const double& _default = 0);
		double GetAttributeDouble(const std::wstring& sName, const double& _default = 0);

		std::wstring ReadNodeTextBase(const wchar_t* bsName);		
		
		std::wstring ReadNodeText(const std::wstring& strName);
		std::wstring ReadValueString(const std::wstring& sName, const std::wstring& nDef = L"");
		int ReadValueInt(const std::wstring& str, const int& nDef = 0);

		std::wstring GetAttributeOrValue(const std::wstring& strAttributeName, const std::wstring& strDefaultValue = L"");

		CXmlNode ReadNode(const wchar_t* strNodeName);
		CXmlNode ReadNode(const std::wstring& strNodeName);
		CXmlNode ReadNodeNoNS(const std::wstring& strNodeName);

		CXmlNode GetNode(const std::wstring& sName);
		CXmlNodes GetNodes(const std::wstring& sName);
		bool GetChilds(CXmlNodes& oXmlNodes);

		bool GetNode(const std::wstring& sName, CXmlNode& oNode);
		bool GetNodes(const std::wstring& sName, CXmlNodes& oNodes);

		CXmlNode& operator=(const CXmlNode& oSrc);

	public:
		std::wstring private_GetXml();
		std::wstring private_GetXml(const std::wstring& strDefaultValue = L"");
		std::wstring private_GetXmlFast();
		std::wstring private_GetXmlFast(const std::wstring& strDefaultValue);

	public:
		template <typename T>
		void LoadArray(const std::wstring& sName, std::vector<T>& arList)
		{
			CXmlNodes oNodes;
			if (GetNodes(sName, oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					CXmlNode oItem;
					oNodes.GetAt(i, oItem);

					arList.push_back(T());
					arList[i].fromXML(oItem);
				}
			}
		}
		template <typename T>
		void LoadArray(const std::wstring& sName, const std::wstring& sSubName, std::vector<T>& arList)
		{
			CXmlNode oNode;
			if (GetNode(sName, oNode))
				oNode.LoadArray(sSubName, arList);
		}
		template<typename T>
		void ReadAttributeBase(const wchar_t* bsName, T& value)
		{
			std::wstring sAttr;
			if (GetAttributeIfExist(std::wstring(bsName), sAttr))
				value = sAttr.GetString();
		}
		template<typename T>
		void ReadAllAttributes(T& strNames, T& strValues)
		{
			if (!IsValid())
				return;

			std::map<CStringA, CStringA>::iterator p;
			for (p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
			{
				strNames.push_back(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.GetString(), p->first.GetLength(), true));
				strValues.push_back(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.GetString(), p->second.GetLength(), true));
			}
		}
		template <typename T>
		void ReadNodeValueBase(const wchar_t* bsName, T& value)
		{
			value = ReadNodeTextBase(bsName);
		}

	private:
		void SetBase(CXmlNodeBase* pBase);
		std::wstring GetNamespace(const std::wstring& strNodeName);
		std::wstring GetNameNoNS(const std::wstring& strNodeName);
	};
}

#endif // _BUILD_XMLUTILS_CROSSPLATFORM_H_