/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
