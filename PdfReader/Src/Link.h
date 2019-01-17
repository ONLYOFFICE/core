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
#ifndef _PDF_READER_LINK_H
#define _PDF_READER_LINK_H

#include "Object.h"

namespace PdfReader
{
	class StringExt;
	class Array;
	class Dict;

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkAction
	//-------------------------------------------------------------------------------------------------------------------------------

	enum LinkActionType
	{
		actionGoTo,        // Go to destination
		actionGoToR,       // Go to destination в новом файле
		actionLaunch,      // Запускаем приложение (или открываем документ)
		actionURI,         // URI
		actionNamed,       // Named
		actionMovie,       // Movie
		actionUnknown      // 
	};

	class LinkAction
	{
	public:

		virtual ~LinkAction() {}

		// Нормально ли создался объект LinkAction?
		virtual bool CheckValidate() = 0;

		// Тип LinkAction.
		virtual LinkActionType GetType() = 0;

		// Парсим Destination (старый вариант Аction) Name, String или Array.
		static LinkAction *ParseDestination(Object *pObject);

		// Парсим Action dictionary.
		static LinkAction *ParseAction(Object *pObject, StringExt *seBaseURI = NULL);

		// Достаем название файла для File specification.
		static StringExt *GetFileSpecName(Object *pFileSpecObject);
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkDestination
	//-------------------------------------------------------------------------------------------------------------------------------

	enum LinkDestinationType
	{
		destXYZ,
		destFit,
		destFitH,
		destFitV,
		destFitR,
		destFitB,
		destFitBH,
		destFitBV
	};

	class LinkDestination
	{
	public:

		LinkDestination(Array *pArray);

		LinkDestination *Copy()
		{
			return new LinkDestination(this);
		}


		bool CheckValidate()
		{
			return m_bValidate;
		}

		LinkDestinationType GetType()
		{
			return m_eType;
		}
		bool IsPageRef()
		{
			return m_bPageIsRef;
		}
		int GetPageNum()
		{
			return m_nPageNum;
		}
		Ref GetPageRef()
		{
			return m_oPageRef;
		}
		double GetLeft()
		{
			return m_dLeft;
		}
		double GetBottom()
		{
			return m_dBottom;
		}
		double GetRight()
		{
			return m_dRight;
		}
		double GetTop()
		{
			return m_dTop;
		}
		double GetZoom()
		{
			return m_dZoom;
		}
		bool GetChangeLeft()
		{
			return m_bChangeLeft;
		}
		bool GetChangeTop()
		{
			return m_bChangeTop;
		}
		bool GetChangeZoom()
		{
			return m_bChangeZoom;
		}

	private:

		LinkDestination(LinkDestination *pDest);

	private:

		LinkDestinationType m_eType;      // Тип
		bool                m_bPageIsRef; // Страница задана ссылкой или номером?
		union
		{
			Ref m_oPageRef;   // Ссылка на страницу
			int m_nPageNum;   // Относительный номер страницы
		};

		double m_dLeft;       //
		double m_dBottom;     // Позиция
		double m_dRight;      //
		double m_dTop;        //

		double m_dZoom;       // Zoom

		bool   m_bChangeLeft; // Для destXYZ. В данном случае может измениться
		bool   m_bChangeTop;  // как позиция, так и зуммирование
		bool   m_bChangeZoom; // 

		bool   m_bValidate;   // 

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkGoTo
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkGoTo : public LinkAction
	{
	public:

		LinkGoTo(Object *pDestObject);

		virtual ~LinkGoTo();

		virtual bool CheckValidate()
		{
			return m_pDestination || m_pNamedDestination;
		}

		virtual LinkActionType GetType()
		{
			return actionGoTo;
		}
		LinkDestination *GetDestination()
		{
			return m_pDestination;
		}
		StringExt *GetNamedDestination()
		{
			return m_pNamedDestination;
		}

	private:

		LinkDestination *m_pDestination;
		StringExt       *m_pNamedDestination;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkGoToR
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkGoToR : public LinkAction
	{
	public:

		LinkGoToR(Object *pFileSpecObject, Object *pDestObject);
		virtual ~LinkGoToR();

		virtual bool CheckValidate()
		{
			return m_seFileName && (m_pDestination || m_pNamedDestination);
		}

		virtual LinkActionType GetType()
		{
			return actionGoToR;
		}
		StringExt *GetFileName()
		{
			return m_seFileName;
		}
		LinkDestination *GetDestination()
		{
			return m_pDestination;
		}
		StringExt *GetNamedDestination()
		{
			return m_pNamedDestination;
		}

	private:

		StringExt       *m_seFileName;
		LinkDestination *m_pDestination;
		StringExt       *m_pNamedDestination;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkLaunch
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkLaunch : public LinkAction
	{
	public:

		LinkLaunch(Object *pActionObject);

		virtual ~LinkLaunch();

		virtual bool CheckValidate()
		{
			return m_seFileName != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionLaunch;
		}
		StringExt *GetFileName()
		{
			return m_seFileName;
		}
		StringExt *GetParameters()
		{
			return m_pParameters;
		}

	private:

		StringExt *m_seFileName;
		StringExt *m_pParameters;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkURI
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkURI : public LinkAction
	{
	public:

		LinkURI(Object *pURIObject, StringExt *seBaseURI);

		virtual ~LinkURI();

		virtual bool CheckValidate()
		{
			return m_seURI != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionURI;
		}
		StringExt *GetURI()
		{
			return m_seURI;
		}

	private:

		StringExt *m_seURI;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkNamed
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkNamed : public LinkAction
	{
	public:

		LinkNamed(Object *pNameObject);

		virtual ~LinkNamed();

		virtual bool CheckValidate()
		{
			return m_seName != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionNamed;
		}

		StringExt *GetName()
		{
			return m_seName;
		}

	private:

		StringExt *m_seName;

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkMovie
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkMovie : public LinkAction
	{
	public:

		LinkMovie(Object *pAnnotObject, Object *pTitleObject);

		virtual ~LinkMovie();

		virtual bool CheckValidate()
		{
			return m_oAnnotRef.nNum >= 0 || m_seTitle != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionMovie;
		}

		bool HasAnnotRef()
		{
			return m_oAnnotRef.nNum >= 0;
		}
		Ref *GetAnnotRef()
		{
			return &m_oAnnotRef;
		}
		StringExt *GetTitle()
		{
			return m_seTitle;
		}

	private:

		Ref        m_oAnnotRef;
		StringExt *m_seTitle;

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkUnknown
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkUnknown : public LinkAction
	{
	public:

		LinkUnknown(char *sAction);

		virtual ~LinkUnknown();

		virtual bool CheckValidate()
		{
			return m_seAction != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionUnknown;
		}

		StringExt *GetAction()
		{
			return m_seAction;
		}

	private:

		StringExt *m_seAction;

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Link
	//-------------------------------------------------------------------------------------------------------------------------------

	class Link
	{
	public:

		Link(Dict *pDict, StringExt *seBaseURI);

		~Link();

		bool CheckValidate()
		{
			return m_bValid;
		}

		// Попала ли точка в рект
		bool InRect(double dX, double dY)
		{
			return m_dLeft <= dX && dX <= m_dRight && m_dBottom <= dY && dY <= m_dTop;
		}

		LinkAction *GetAction()
		{
			return m_pAction;
		}

		void GetRect(double *pdLeft, double *pdBottom, double *pdRight, double *pdTop)
		{
			*pdLeft   = m_dLeft;
			*pdBottom = m_dBottom;
			*pdRight  = m_dRight;
			*pdTop    = m_dTop;
		}

	private:

		double      m_dLeft;
		double      m_dBottom;
		double      m_dRight;
		double      m_dTop;

		LinkAction *m_pAction;
		bool        m_bValid;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Links
	//-------------------------------------------------------------------------------------------------------------------------------

	class Links
	{
	public:

		Links(Object *pAnnots, StringExt *seBaseURI);

		~Links();

		int GetLinksCount()
		{
			return m_nLinksCount;
		}
		Link *GetLink(int nIndex)
		{
			return m_ppLinks[nIndex];
		}


		// Если точка (dX, dY) попадает в Link, тогда возвращаем соответствующий Action.
		LinkAction *Find(double dX, double dY);

		// Проверяем попадает ли точка в Link.
		bool OnLink(double dX, double dY);

	private:

		Link **m_ppLinks;
		int    m_nLinksCount;
	};
}

#endif // _PDF_READER_LINK_H
