#pragma once	

#include <vector>
#include <string>

#include "..\docxdocconverter\prl.h"

typedef std::vector<AVSDocFileFormat::Prl>	PrlList;

namespace DOCXTODOC
{
	class CInternalStyle
	{
	public:

		CInternalStyle () : m_bIsRenewed(FALSE)
		{

		}

		CInternalStyle (std::string strName, const PrlList& arRun, const PrlList& arParagraph) : m_bIsRenewed(FALSE), m_strName(strName)
		{
			SetPrlsLists (arRun, arParagraph);
		}

		CInternalStyle (std::string strBaseStyle) : m_bIsRenewed(FALSE)
		{

		}

		inline void SetPrlsLists (const PrlList& arRun, const PrlList& arParagraph)
		{
			m_arRun			=	arRun;
			m_arParagraph	=	arParagraph;
		}

		inline void SetName (const std::string& strName)
		{
			m_strName	=	strName;
		}

		inline BOOL UpdateRunPrls (const PrlList& arRun)
		{
			if (0 == arRun.size())		//	не дополняем стиль
				return FALSE;

			if (0 == m_arRun.size())	//	стиль не имеет параметров для символов
			{
				m_arRun	= arRun;
				return TRUE;
			}

			BOOL bModifity	=	FALSE;

			for (size_t i = 0; i < arRun.size(); ++i)
			{
				int nInd = FindSprm (arRun[i].GetSprmCode());
				if (nInd >= 0)
				{
					if (m_arRun[nInd] != arRun[i])
					{
						m_arRun[nInd]	=	arRun[i];
						bModifity		=	TRUE;
					}
				}
				else
				{
					m_arRun.push_back (arRun[i]);
					bModifity			=	TRUE;
				}
			}

			m_bIsRenewed	=	bModifity;

			return bModifity;
		}

		inline BOOL IsRenewed ()
		{
			return m_bIsRenewed;
		}

		inline bool operator == (const CInternalStyle& oStyle)
		{
			if ((oStyle.m_arRun.size() != m_arRun.size()) || (oStyle.m_arParagraph.size() != m_arParagraph.size()) )
				return false;

			for (size_t i = 0; i < m_arRun.size(); ++i)
			{
				if (m_arRun[i] != oStyle.m_arRun[i])
					return false;
			}
			
			for (size_t i = 0; i < m_arParagraph.size(); ++i)
			{
				if (m_arParagraph[i] != oStyle.m_arParagraph[i])
					return false;
			}

			return true; // ((oStyle.m_strName == m_strName) );//&& (oStyle.m_arRun == m_arRun) && (oStyle.m_arParagraph == m_arParagraph));
		}

		inline const PrlList& GetRun()
		{
			return m_arRun;
		}

	private:
		
		inline int FindSprm (unsigned short sprm)
		{
			for (size_t i = 0; i < m_arRun.size(); ++i)
			{
				if (sprm == m_arRun[i].GetSprmCode())
					return i;
			}

			return -1;
		}

	private:

		BOOL m_bIsRenewed;

		std::string m_strName;

		PrlList	m_arRun;
		PrlList	m_arParagraph;
	};

}