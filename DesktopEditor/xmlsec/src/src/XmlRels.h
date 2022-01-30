#ifndef _XML_RELS_H_
#define _XML_RELS_H_

#include "../../../common/StringBuilder.h"
#include "../../../common/File.h"
#include "../../../common/Directory.h"
#include "../../../../OfficeUtils/src/ZipFolder.h"

class COOXMLRelationship
{
public:
    std::wstring rid;
    std::wstring type;
    std::wstring target;
    std::wstring target_mode;

public:

    COOXMLRelationship()
    {
    }

    COOXMLRelationship(XmlUtils::CXmlNode& node)
    {
        rid = node.GetAttribute("Id");
        type = node.GetAttribute("Type");
        target = node.GetAttribute("Target");
        target_mode = node.GetAttribute("TargetMode");

        CheckTargetMode();
    }

    std::wstring GetXml()
    {
        NSStringUtils::CStringBuilder builder;
        builder.WriteString(L"<Relationship Id=\"");
        builder.WriteEncodeXmlString(rid);
        builder.WriteString(L"\" Type=\"");
        builder.WriteEncodeXmlString(type);
        builder.WriteString(L"\" Target=\"");
        builder.WriteEncodeXmlString(target);
        builder.WriteString(L"\" TargetMode=\"");
        builder.WriteEncodeXmlString(target_mode);
        builder.WriteString(L"\" />");
        return builder.GetData();
    }

    static bool Compare(const COOXMLRelationship& i, const COOXMLRelationship& j)
    {
        return i.rid < j.rid;
    }

protected:
    void CheckTargetMode()
    {
        if (!target_mode.empty())
            return;

        if (0 == target.find(L"http") || 0 == target.find(L"www") || 0 == target.find(L"ftp"))
            target_mode = L"External";
        else
            target_mode = L"Internal";
    }
};

class COOXMLRelationships
{
public:
    std::vector<COOXMLRelationship> rels;
    IFolder* m_pFolder;

public:

    COOXMLRelationships()
    {
        m_pFolder = NULL;
    }

    COOXMLRelationships(const std::string& xml, std::map<std::wstring, bool>* check_need = NULL)
    {
        XmlUtils::CXmlNode oNode;
        if (!oNode.FromXmlStringA(xml))
            return;

        FromXmlNode(oNode, check_need);
    }

    COOXMLRelationships(const std::wstring& xml, IFolder* pFolder, std::map<std::wstring, bool>* check_need = NULL)
    {
        XmlUtils::CXmlNode oNode;

        if (NULL == pFolder)
        {
            if (!oNode.FromXmlString(xml))
                return;
        }
        else
        {
            m_pFolder = pFolder;
            oNode = pFolder->getNodeFromFile(xml);
            if (!oNode.IsValid())
                return;
        }

        FromXmlNode(oNode, check_need);
    }

    void FromXmlNode(XmlUtils::CXmlNode& oNode, std::map<std::wstring, bool>* check_need = NULL)
    {
        XmlUtils::CXmlNodes oNodes;
        if (!oNode.GetNodes(L"Relationship", oNodes))
            return;

        int nCount = oNodes.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode oRel;
            oNodes.GetAt(i, oRel);

            if (NULL == check_need)
            {
                rels.push_back(COOXMLRelationship(oRel));
            }
            else
            {
                std::wstring sRid = oRel.GetAttribute("Id");
                if (check_need->find(sRid) != check_need->end())
                    rels.push_back(COOXMLRelationship(oRel));
            }
        }
    }

    std::wstring GetXml()
    {
        NSStringUtils::CStringBuilder builder;

        builder.WriteString(L"<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

        // sort by rId
        std::sort(rels.begin(), rels.end(), COOXMLRelationship::Compare);

        for (std::vector<COOXMLRelationship>::iterator i = rels.begin(); i != rels.end(); i++)
            builder.WriteString(i->GetXml());

        builder.WriteString(L"</Relationships>");

        return builder.GetData();
    }

    std::wstring GetTransforms()
    {
        NSStringUtils::CStringBuilder builder;

        builder.WriteString(L"<Transforms><Transform Algorithm=\"http://schemas.openxmlformats.org/package/2006/RelationshipTransform\">");

        for (std::vector<COOXMLRelationship>::iterator i = rels.begin(); i != rels.end(); i++)
        {
            builder.WriteString(L"<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"");
            builder.WriteEncodeXmlString(i->rid);
            builder.WriteString(L"\" />");
        }

        builder.WriteString(L"</Transform><Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/></Transforms>");

        return builder.GetData();
    }

    void CheckOriginSigs(const std::wstring& file)
    {
        int rId = 0;
        std::string sReplace = "";
        std::vector<COOXMLRelationship>::iterator i = rels.begin();
        while (i != rels.end())
        {
            if (0 == i->target.find(L"_xmlsignatures/"))
            {
                sReplace = U_TO_UTF8(i->target);
                break;
            }

            std::wstring rid = i->rid;
            rid = rid.substr(3);

            int nTemp = std::stoi(rid);

            if (nTemp > rId)
                rId = nTemp;

            i++;
        }

        if (!sReplace.empty())
        {
            if (sReplace == "_xmlsignatures/origin.sigs")
                return;

            std::string sXmlA = m_pFolder->readXml(file);
            NSStringUtils::string_replaceA(sXmlA, sReplace, "_xmlsignatures/origin.sigs");
            m_pFolder->writeXmlA(file, sXmlA);
            return;
        }

        std::string sXmlA = m_pFolder->readXml(file);

        std::string::size_type pos = sXmlA.rfind("</Relationships>");
        if (pos == std::string::npos)
            return;

        rId++;
        std::string sRet = sXmlA.substr(0, pos);
        sRet += ("<Relationship Id=\"rId" + std::to_string(rId) + "\" \
Type=\"http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/origin\" Target=\"_xmlsignatures/origin.sigs\"/>\
</Relationships>");
        m_pFolder->writeXmlA(file, sRet);
    }
};

#endif //_XML_RELS_H_
