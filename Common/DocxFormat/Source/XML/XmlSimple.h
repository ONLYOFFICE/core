#pragma once

#include "xmlutils.h"
#include "../Base/Nullable.h"

//#include "../SystemUtility/File.h"
//#include "../SystemUtility/FileSystem/Directory.h"
#include "../../../DesktopEditor/common/File.h"
//#include "../../../DesktopEditor/common/Directory.h"
#include "../SystemUtility/SystemUtility.h"

namespace XmlUtils
{
	class CAttribute
	{
	public:
		CString m_strValue;

	public:
		CAttribute()
		{
			m_strValue = _T("");
		}
	public:
		AVSINLINE void Write(const CString& strName, const int& value)
		{
			CString s = _T(""); s.Format(_T("=\"%d\""), value);
			m_strValue += (_T(" ") + strName + s);
		}
		AVSINLINE void Write(const CString& strName, const size_t& value)
		{
			CString s = _T(""); s.Format(_T("=\"%u\""), value);
			m_strValue += (_T(" ") + strName + s);
		}
		AVSINLINE void Write(const CString& strName, const double& value)
		{
			CString s = _T(""); s.Format(_T("=\"%lf\""), value);
			m_strValue += (_T(" ") + strName + s);
		}
		AVSINLINE void Write(const CString& strName, const CString& value)
		{
			m_strValue += (_T(" ") + strName + _T("=\"") + value + _T("\""));
		}
		AVSINLINE void Write(const CString& strName, const bool& value)
		{
			if (value)
				m_strValue += (_T(" ") + strName + _T("=\"true\""));
			else
				m_strValue += (_T(" ") + strName + _T("=\"false\""));
		}
		template <typename T>
		AVSINLINE void WriteLimit(const CString& strName, const T& value)
		{
			m_strValue += (_T(" ") + strName + _T("=\"") + value.get() + _T("\""));
		}
		template <typename T>
		AVSINLINE void WriteNullable(const CString& strName, const nullable<T>& value)
		{
			if (value.IsInit())
				Write(strName, value->ToString());
		}
		AVSINLINE void WriteNullable(const CString& strName, const nullable_string& value)
		{
			if (value.IsInit())
				WriteLimit(strName, value);
		}
		template <typename T>
		AVSINLINE void WriteLimit2(const CString& strName, const T& value)
		{
			Write(strName, value.ToString());
		}
		
	public:
		AVSINLINE void Write(const CString& strName, const nullable_int& value)
		{
			if (!value.IsInit())
				return;
			CString s = _T(""); s.Format(_T("=\"%d\""), *value);
			m_strValue += (_T(" ") + strName + s);
		}
		AVSINLINE void Write(const CString& strName, const nullable_sizet& value)
		{
			if (!value.IsInit())
				return;
			CString s = _T(""); s.Format(_T("=\"%u\""), *value);
			m_strValue += (_T(" ") + strName + s);
		}
		AVSINLINE void Write(const CString& strName, const nullable_double& value)
		{
			if (!value.IsInit())
				return;
			CString s = _T(""); s.Format(_T("=\"%lf\""), *value);
			m_strValue += (_T(" ") + strName + s);
		}
		AVSINLINE void Write(const CString& strName, const nullable_string& value)
		{
			if (!value.IsInit())
				return;
			m_strValue += (_T(" ") + strName + _T("=\"") + *value + _T("\""));
		}
		AVSINLINE void Write(const CString& strName, const nullable_bool& value)
		{
			if (!value.IsInit())
				return;
			if (*value)
				m_strValue += (_T(" ") + strName + _T("=\"true\""));
			else
				m_strValue += (_T(" ") + strName + _T("=\"false\""));
		}
		template <typename T>
		AVSINLINE void WriteLimitNullable(const CString& strName, const nullable_limit<T>& value)
		{
			if (!value.IsInit())
				return;
			Write(strName, value->get());
		}

	public:
		CAttribute(const CAttribute& oSrc)
		{
			*this = oSrc;
		}
		CAttribute& operator=(const CAttribute& oSrc)
		{
			m_strValue = oSrc.m_strValue;
		}
	};

	class CNodeValue
	{
	public:
		CString m_strValue;

	public:
		CNodeValue()
		{
			m_strValue = _T("");
		}
		template <typename T>
		AVSINLINE void Write(T& value)
		{
			m_strValue += value.toXML();
		}
		template <typename T>
		AVSINLINE void Write(const CString& strNodeName, T& value)
		{
			m_strValue += (_T("<") + strNodeName + _T(">"));
			m_strValue += value.toXML();
			m_strValue += (_T("</") + strNodeName + _T(">"));
		}
		template <typename T>
		AVSINLINE void WriteNullable(const nullable<T>& value)
		{
			if (value.IsInit())
				m_strValue += value->toXML();
		}
#ifdef _WIN32
		template <typename T>
		AVSINLINE void WriteArray(const CAtlArray<T>& oArray)
		{
			size_t count = oArray.GetCount();
			for (size_t i = 0; i < count; ++i)
				m_strValue += oArray[i].toXML();
		}
		template <typename T>
		AVSINLINE void WriteArray(const CString& strNodeName, const CAtlArray<T>& oArray)
		{
			m_strValue += (_T("<") + strNodeName + _T(">"));
			size_t count = oArray.GetCount();
			for (size_t i = 0; i < count; ++i)
				m_strValue += oArray[i].toXML();
			m_strValue += (_T("</") + strNodeName + _T(">"));
		}
#endif

        template <typename T>
        AVSINLINE void WriteArray(const std::vector<T>& oArray)
        {
            size_t count = oArray.size();
            for (size_t i = 0; i < count; ++i)
                m_strValue += oArray[i].toXML();
        }
        template <typename T>
        AVSINLINE void WriteArray(const CString& strNodeName, const std::vector<T>& oArray)
        {
            m_strValue += (_T("<") + strNodeName + _T(">"));
            size_t count = oArray.size();
            for (size_t i = 0; i < count; ++i)
                m_strValue += oArray[i].toXML();
            m_strValue += (_T("</") + strNodeName + _T(">"));
        }

		// --------------------------------------------------------------- //
		AVSINLINE void Write2(const CString& strName, const int& value)
		{
			Write2(strName, IntToString(value));
		}
		AVSINLINE void Write2(const CString& strName, const size_t& value)
		{
			CString s = _T(""); s.Format(_T("=\"%u\""), value);
			Write2(strName, s);
		}
		AVSINLINE void Write2(const CString& strName, const double& value)
		{
			CString s = _T(""); s.Format(_T("=\"%lf\""), value);
			Write2(strName, s);
		}
		AVSINLINE void Write2(const CString& strName, const CString& value)
		{
			m_strValue += (_T("<") + strName + _T(">") + value + _T("</") + strName + _T(">"));
		}
		AVSINLINE void Write2(const CString& strName, const bool& value)
		{
			if (value)
				m_strValue += (_T("<") + strName + _T(">true</") + strName + _T(">"));
			else
				m_strValue += (_T("<") + strName + _T(">false</") + strName + _T(">"));
		}

		// ---------------------------------------------------------------- //
		AVSINLINE void Write2(const CString& strName, const nullable_int& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const CString& strName, const nullable_sizet& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const CString& strName, const nullable_double& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const CString& strName, const nullable_string& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
		AVSINLINE void Write2(const CString& strName, const nullable_bool& value)
		{
			if (value.IsInit())
				Write2(strName, *value);
		}
	};

	AVSINLINE CString CreateNode(const CString& strName, const CAttribute& oAttr)
	{
		return _T("<") + strName + _T(" ") + oAttr.m_strValue + _T(" />");
	}
	AVSINLINE CString CreateNode(const CString& strName, const CNodeValue& oNode)
	{
		if (_T("") == oNode.m_strValue)
			return _T("<") + strName + _T("/>");

		return _T("<") + strName + _T(">") + oNode.m_strValue + _T("</") + strName + _T(">");
	}
	AVSINLINE CString CreateNode(const CString& strName, const CAttribute& oAttr, const CNodeValue& oNode)
	{
		if (_T("") == oNode.m_strValue)
			return CreateNode(strName, oAttr);
		
		return _T("<") + strName + _T(" ") + oAttr.m_strValue + _T(">") + oNode.m_strValue +  _T("</") + strName + _T(">");
	}
	AVSINLINE CString CreateNode(const CString& strName, const CAttribute& oAttr, const CString& strXml)
	{
		if (_T("") != strXml)
			return _T("<") + strName + _T(" ") + oAttr.m_strValue + _T(">") + strXml + _T("</") + strName + _T(">");
		return _T("<") + strName + _T(" ") + oAttr.m_strValue + _T("/>");
	}
	AVSINLINE CString CreateNode(const CString& strName, const CString& strXml)
	{
		return _T("<") + strName + _T(">") + strXml + _T("</") + strName + _T(">");
	}

	AVSINLINE void SaveToFile(const CString& strFile, const CString& strXml)
	{
		CDirectory::SaveToFile(strFile, strXml);
	}
}
