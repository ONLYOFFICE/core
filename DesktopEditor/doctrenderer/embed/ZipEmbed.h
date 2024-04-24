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

#ifndef _BUILD_NATIVE_ZIP_EMBED_H_
#define _BUILD_NATIVE_ZIP_EMBED_H_

#include "../../../OfficeUtils/src/ZipFolder.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class JS_DECL CZipEmbed : public CJSEmbedObject
{
public:
	IFolder* m_pFolder;

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
	JSSmart<CJSValue> open(JSSmart<CJSValue> typedArray_or_Folder);
	JSSmart<CJSValue> create();
	JSSmart<CJSValue> save();
	JSSmart<CJSValue> getFile(JSSmart<CJSValue> filePath);
	JSSmart<CJSValue> addFile(JSSmart<CJSValue> filePath, JSSmart<CJSValue> typedArray);
	JSSmart<CJSValue> removeFile(JSSmart<CJSValue> filePath);
	JSSmart<CJSValue> close();
	JSSmart<CJSValue> getPaths();

	JSSmart<CJSValue> decodeImage(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> isRgba);
	JSSmart<CJSValue> encodeImageData(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> stride, JSSmart<CJSValue> format, JSSmart<CJSValue> isRgba);
	JSSmart<CJSValue> encodeImage(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> format);
	JSSmart<CJSValue> getImageType(JSSmart<CJSValue> typedArray);

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_ZIP_EMBED_H_
