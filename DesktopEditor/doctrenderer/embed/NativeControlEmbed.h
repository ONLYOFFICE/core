#ifndef _BUILD_NATIVE_NATIVECONTROL_EMBED_H_
#define _BUILD_NATIVE_NATIVECONTROL_EMBED_H_

#include "../nativecontrol.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class CNativeControlEmbed : public CJSEmbedObject
{
public:
	NSNativeControl::CNativeControl* m_pInternal;
	CZipWorker m_oZipWorker;
public:
	CNativeControlEmbed() : m_pInternal(new NSNativeControl::CNativeControl()) {}
	~CNativeControlEmbed() { RELEASEOBJECT(m_pInternal); }

	virtual void* getObject() override { return (void*)m_pInternal; }

public:

	JSSmart<CJSValue> SetFilePath(JSSmart<CJSValue> path);
	JSSmart<CJSValue> GetFilePath();
	JSSmart<CJSValue> SetFileId(JSSmart<CJSValue> fileId);
	JSSmart<CJSValue> GetFileId();
	JSSmart<CJSValue> GetFileBinary(JSSmart<CJSValue> file);
	JSSmart<CJSValue> GetFontBinary(JSSmart<CJSValue> file);
	JSSmart<CJSValue> GetFontsDirectory();
	JSSmart<CJSValue> GetFileString(JSSmart<CJSValue> file);
	JSSmart<CJSValue> GetEditorType();
	JSSmart<CJSValue> CheckNextChange();
	JSSmart<CJSValue> GetCountChanges();
	JSSmart<CJSValue> GetChangesFile(JSSmart<CJSValue> index);
	/*[noexport]*/JSSmart<CJSValue> Save_AllocNative(JSSmart<CJSValue> nLen);
	/*[noexport]*/JSSmart<CJSValue> Save_ReAllocNative(JSSmart<CJSValue> pos, JSSmart<CJSValue> len);
	JSSmart<CJSValue> Save_End(JSSmart<CJSValue> pos, JSSmart<CJSValue> len);
	JSSmart<CJSValue> AddImageInChanges(JSSmart<CJSValue> img);
	JSSmart<CJSValue> ConsoleLog(JSSmart<CJSValue> message);
	/*[noexport]*/JSSmart<CJSValue> SaveChanges(JSSmart<CJSValue> sParam, JSSmart<CJSValue> nDeleteIndex, JSSmart<CJSValue> nCount);
	JSSmart<CJSValue> ZipOpen(JSSmart<CJSValue> name);
	JSSmart<CJSValue> ZipOpenBase64(JSSmart<CJSValue> name);
	JSSmart<CJSValue> ZipFileAsString(JSSmart<CJSValue> name);
	JSSmart<CJSValue> ZipFileAsBinary(JSSmart<CJSValue> name);
	JSSmart<CJSValue> ZipClose();
	JSSmart<CJSValue> GetImageUrl(JSSmart<CJSValue> sUrl);
	JSSmart<CJSValue> GetImagesPath();
	JSSmart<CJSValue> GetImageOriginalSize(JSSmart<CJSValue> sUrl);

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_NATIVECONTROL_EMBED_H_
