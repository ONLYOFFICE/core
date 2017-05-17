#ifndef _XML_CANONICALIZATOR_H_
#define _XML_CANONICALIZATOR_H_

#include "../../../common/File.h"
#include "../../../common/Directory.h"

#include "../../../common/StringBuilder.h"
#include "../../../xml/include/xmlutils.h"
#include "../../../xml/libxml2/include/libxml/c14n.h"

#ifndef XML_UNUSED
#define XML_UNUSED( arg )  ( (arg) = (arg) )
#endif

class CXmlCanonicalizator
{
private:
    class CXmlBuffer
    {
    public:
        NSStringUtils::CStringBuilderA builder;

    public:
        CXmlBuffer()
        {
        }
        ~CXmlBuffer()
        {
        }
    };

    static int buffer_xmlBufferIOWrite(CXmlBuffer* buf, const char* buffer, int len)
    {
        buf->builder.WriteString(buffer, (size_t)len);
        return len;
    }

    static int buffer_xmlBufferIOClose(CXmlBuffer* buf)
    {
        XML_UNUSED(buf);
        return 0;
    }

    static int buffer_xmlC14NIsVisibleCallback(void * user_data, xmlNodePtr node, xmlNodePtr parent)
    {
        XML_UNUSED(user_data);
        XML_UNUSED(parent);
        if (node->type == XML_TEXT_NODE)
        {
            const char* cur = (char*)node->content;
            size_t size = strlen(cur);
            for (size_t i = 0; i < size; ++i, ++cur)
            {
                if (*cur != '\n' && *cur != '\r' && *cur != '\t')
                    return 1;
            }
            return 0;
        }
        return 1;
    }

public:
    static std::string Execute(const std::string& sXml, int mode = XML_C14N_1_0, bool withComments = false)
    {
        xmlDocPtr xmlDoc = xmlParseMemory((char*)sXml.c_str(), (int)sXml.length());

        CXmlBuffer bufferC14N;
        xmlOutputBufferPtr _buffer = xmlOutputBufferCreateIO((xmlOutputWriteCallback)buffer_xmlBufferIOWrite,
                                                             (xmlOutputCloseCallback)buffer_xmlBufferIOClose,
                                                             &bufferC14N,
                                                             NULL);

        xmlC14NExecute(xmlDoc, buffer_xmlC14NIsVisibleCallback, NULL, mode, NULL, withComments ? 1 : 0, _buffer);

        xmlOutputBufferClose(_buffer);

        return bufferC14N.builder.GetData();
    }

    static std::string Execute(const std::wstring& sXmlFile, int mode = XML_C14N_1_0, bool withComments = false)
    {
        std::string sXml;
        NSFile::CFileBinary::ReadAllTextUtf8A(sXmlFile, sXml);
        return Execute(sXml, mode, withComments);
    }
};

#endif //_XML_CANONICALIZATOR_H_
