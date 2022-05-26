#ifndef _BUILD_NATIVE_ZIP_EMBED_H_
#define _BUILD_NATIVE_ZIP_EMBED_H_

#include "../../OfficeUtils/src/ZipFolder.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class CZipEmbed : public CJSEmbedObject
{
public:
    CZipFolderMemory* m_pFolder;

public:
    CZipEmbed()
    {
        m_pFolder = NULL;
    }
    ~CZipEmbed()
    {
        RELEASEOBJECT(m_pFolder);
    }

    virtual void* getObject() override { return (void*)m_pFolder; }

public:
    JSSmart<CJSValue> open(JSSmart<CJSValue> typedArray);
    JSSmart<CJSValue> create();
    JSSmart<CJSValue> save();
    JSSmart<CJSValue> getFile(JSSmart<CJSValue> filePath);
    JSSmart<CJSValue> addFile(JSSmart<CJSValue> filePath, JSSmart<CJSValue> typedArray);
    JSSmart<CJSValue> removeFile(JSSmart<CJSValue> filePath);
    JSSmart<CJSValue> close();

    static void CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context);
};

#endif // _BUILD_NATIVE_ZIP_EMBED_H_
