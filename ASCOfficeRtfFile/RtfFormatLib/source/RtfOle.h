/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#include "RtfParagraph.h"
#include "RtfShape.h"

class RtfOle : public IDocumentElement
{
public: 
	enum _OleType{ ot_none, ot_emb, ot_link };
	
	_OleType		m_eOleType;
	int				m_nShapeId;
	int				m_nWidth;
	int				m_nHeight;
	CString			m_sOleClass;
	RtfShapePtr		m_oResultPic;

	RtfCharProperty	m_oCharProperty; // тут могут быть track changes ....
	RtfOle()
	{
		m_piStorage = NULL;
		SetDefault();
	}
	~RtfOle()
	{
		SetDefault();
	}
	int GetType( )
	{
		return TYPE_RTF_OLE;
	}
	bool IsValid()
	{
        return PROP_DEF != m_nWidth && PROP_DEF != m_nHeight && L"" != m_sOleFilename;
                /*&& ::GetFileAttributes( m_sOleFilename ) != DWORD( -1 )*/
	}
	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter);

	void SetFilename( CString sFilename )
	{
		m_sOleFilename = sFilename;
	}
	void SetOle( POLE::Storage* piOle )
	{
		if (piOle == NULL) return;

		m_piStorage = piOle;
	}
	void SetDefault()
	{
		m_eOleType	 = ot_none;
		m_nShapeId	= PROP_DEF;
		m_nWidth	= PROP_DEF;
		m_nHeight	= PROP_DEF;
		
		RELEASEOBJECT( m_piStorage ); 
		Utils::RemoveDirOrFile( m_sOleFilename );
		m_sOleFilename = L"";

		m_oCharProperty.SetDefault();
	}
private: 
	POLE::Storage*	m_piStorage;
	CString			m_sOleFilename;

	CString RenderToOOXOnlyOle(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfOle> RtfOlePtr;


#if defined (_WIN32) || defined (_WIN64)
	struct RtfOle1ToOle2Stream : OLESTREAM
	{
		BYTE* pBuffer;
		long nBufferSize;
		long nCurPos;
	};
	struct RtfOle2ToOle1Stream : OLESTREAM
	{
		std::vector<BYTE> aBuffer;
	};

	DWORD CALLBACK OleGet1(LPOLESTREAM oStream, void FAR* pTarget, DWORD dwRead);;
	DWORD CALLBACK OlePut1(LPOLESTREAM, const void FAR*, DWORD);
	DWORD CALLBACK OleGet2(LPOLESTREAM oStream, void FAR* pTarget, DWORD dwRead);;
	DWORD CALLBACK OlePut2(LPOLESTREAM, const void FAR*, DWORD);

#else
	struct RtfOle1ToOle2Stream :  POLE::Stream
	{
		BYTE* pBuffer;

		long nBufferSize;
		long nCurPos;
	};
	struct RtfOle2ToOle1Stream : POLE::Stream
	{
		std::vector<BYTE> aBuffer;
	};
#endif