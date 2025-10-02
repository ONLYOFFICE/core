/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#ifndef _BUILD_DOCINFO_H_
#define _BUILD_DOCINFO_H_

#include "../config.h"
#include "../MetafileToRenderer.h"
class IMetafileToRenderter;

class GRAPHICS_DECL CHyperlinkCommand : public IAdvancedCommand
{
private:
	std::wstring m_sUrl;
	std::wstring m_sToolTip;

	double m_dX = 0;
	double m_dY = 0;
	double m_dW = 0;
	double m_dH = 0;

public:
	CHyperlinkCommand();
	virtual ~CHyperlinkCommand();

	double GetX();
	double GetY();
	double GetW();
	double GetH();

	void SetX(const double& val);
	void SetY(const double& val);
	void SetW(const double& val);
	void SetH(const double& val);

	std::wstring GetUrl();
	std::wstring GetToolTip();

	void SetUrl(const std::wstring& val);
	void SetToolTip(const std::wstring& val);

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);
};

class GRAPHICS_DECL CLinkCommand : public IAdvancedCommand
{
private:
	int m_nPage;
	double m_dDestX;
	double m_dDestY;

	double m_dX = 0;
	double m_dY = 0;
	double m_dW = 0;
	double m_dH = 0;

public:
	CLinkCommand();
	virtual ~CLinkCommand();

	double GetX();
	double GetY();
	double GetW();
	double GetH();

	void SetX(const double& val);
	void SetY(const double& val);
	void SetW(const double& val);
	void SetH(const double& val);

	int GetPage();
	double GetDestX();
	double GetDestY();

	void SetPage(const int& val);
	void SetDestX(const double& val);
	void SetDestY(const double& val);

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);
};

class GRAPHICS_DECL CDocInfoCommand : public IAdvancedCommand
{
private:
	std::wstring m_sTitle;
	std::wstring m_sCreator;
	std::wstring m_sSubject;
	std::wstring m_sKeywords;

public:
	CDocInfoCommand();
	virtual ~CDocInfoCommand();

	std::wstring GetTitle();
	std::wstring GetCreator();
	std::wstring GetSubject();
	std::wstring GetKeywords();

	void SetTitle(const std::wstring& val);
	void SetCreator(const std::wstring& val);
	void SetSubject(const std::wstring& val);
	void SetKeywords(const std::wstring& val);

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);
};

class GRAPHICS_DECL CShapeStart : public IAdvancedCommand
{
public:
	CShapeStart();
	~CShapeStart();

	std::string& GetShapeXML();
	const std::vector<std::wstring>& GetRedactID();
	Aggplus::CImage* GetShapeImage();

	void SetShapeXML(const std::string& sShapeXML);
	void SetShapeImage(BYTE* pImgData, int nWidth, int nHeight);

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector, int nLen);

private:
	std::string m_sShapeXML;
	std::vector<std::wstring> m_arrRedactID;
	Aggplus::CImage* m_pImage;
};

class GRAPHICS_DECL CEmptyComand : public IAdvancedCommand
{
public:
	CEmptyComand(AdvancedCommandType nType);
};

class GRAPHICS_DECL CPageRotate : public IAdvancedCommand
{
public:
	CPageRotate();

	int GetPageRotate();

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	int m_nPageRotate;
};

class GRAPHICS_DECL CHeadings : public IAdvancedCommand
{
public:
	struct CHeading
	{
		std::wstring wsTitle;
		int nPage;
		double dX;
		double dY;
		CHeading* pParent;
		std::vector<CHeading*> arrHeading;

		CHeading();
		~CHeading();
	};

	CHeadings();
	~CHeadings();

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

	const std::vector<CHeading*>& GetHeading();

private:
	std::vector<CHeading*> m_arrHeading;
};


#endif // _BUILD_DOCINFO_H_
