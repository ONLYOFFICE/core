/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef _XPS_XPSLIB_UTILS_H
#define _XPS_XPSLIB_UTILS_H

#include <string>
#include <vector>
#include "WString.h"

#include "../../DesktopEditor/common/Types.h"

namespace XmlUtils
{
	class CXmlLiteReader;
}

class IRenderer;

namespace XPS
{
	struct TIndicesEntry
	{
	public:

		TIndicesEntry() : nUnicode(0), nGid(0), dAdvance(0.0), dHorOffset(0.0), dVerOffset(0.0),
			bUnicode(false), bGid(false), bAdvance(false), bHorOffset(false), bVerOffset(false)
		{
		}
		void Reset()
		{
			bUnicode   = false;
			bGid       = false;
			bAdvance   = false;
			bHorOffset = false;
			bVerOffset = false;

			nUnicode   = 0;
			nGid       = 0;
			dAdvance   = 0;
			dHorOffset = 0;
			dVerOffset = 0;
		}

	public:

		unsigned int   nUnicode;
		unsigned short nGid;
		double         dAdvance;
		double         dHorOffset;
		double         dVerOffset;

		bool           bUnicode;
		bool           bGid;
		bool           bAdvance;
		bool           bHorOffset;
		bool           bVerOffset;

		std::vector<unsigned int> vRemainUnicodes;
	};


	bool   IsAlpha(wchar_t wChar);
    double GetDouble(const CWString& wsString);
	double GetDouble(const std::wstring& wsString);
	int    GetInteger(const std::wstring& wsString);
	bool   GetBool(const std::wstring& wsString);
	void   GetBgra(const std::wstring& wsString, int& nBgr, int& nAlpha);

    unsigned char GetCapStyle(const std::string& wsCapStyle);

	std::wstring NormalizePath(const std::wstring& wsPath);
	std::wstring GetPath(const std::wstring& wsPath);
	std::wstring GetFileName(const std::wstring& wsPath);
	std::wstring GetFileExtension(const std::wstring& wsPath);
	std::wstring RemoveNamespace(const std::wstring& wsString);

	std::vector<std::vector<std::wstring>> Split(const std::wstring& wsString, wchar_t wDelim1, wchar_t wDelim2);

	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, std::wstring& wsAttr);
	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, CWString& wsAttr);
	bool VmlToRenderer(const CWString& wsString, IRenderer* pRenderer);
	bool GetNextGlyph(const wchar_t* wsIndices, int& nIndicesPos, const int& nIndicesLen, unsigned short* pUtf16, int& nUtf16Pos, const int& nUtf16Len, TIndicesEntry& oEntry);

	void ReadSTPoint(const CWString& wsString, double& dX, double& dY);
	void ReadSTColor(const CWString& wsString, int& nBgr, int& nAlpha);
	void ReadSTColor(const CWString& wsString, LONG& lColor);
	void ReadSTDouble(const CWString& wsString, double& dValue);

	void ReadMatrixTransform(XmlUtils::CXmlLiteReader& oReader, CWString& wsTransform, CWString* pwsKey = NULL);
	void ReadTransform      (XmlUtils::CXmlLiteReader& oReader, CWString& wsTransform, CWString* pwsKey = NULL);
	void ReadPathGeometry   (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTransform, CWString* pwsKey = NULL);
	void ReadPathFigure     (XmlUtils::CXmlLiteReader& oReader, std::wstring&, bool bEvenOdd);
	void ReadClip           (XmlUtils::CXmlLiteReader& oReader, CWString& wsClip);

	void ReadGradientStops(XmlUtils::CXmlLiteReader& oReader, std::vector<LONG>& vColors, std::vector<double>& vPositions, const double& dOpacity);
}

#endif // _XPS_XPSLIB_UTILS_H
