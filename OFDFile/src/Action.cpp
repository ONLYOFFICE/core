#include "Action.h"

namespace OFD
{
void SetAction(const IAction*& pAction, IAction* pValue)
{
	if (nullptr != pAction)
		delete pAction;

	pAction = pValue;
}

CAction::CAction(CXmlReader& oReader)
	: m_eEvent(EEvent::CLICK), m_pRegion(nullptr), m_pAction(nullptr)
{
	if (oReader.MoveToFirstAttribute())
	{
		do
		{
			if ("Event" != oReader.GetNameA())
				continue;

			const std::wstring wsEvent{oReader.GetText()};

			if (L"Click" == wsEvent)
				m_eEvent = EEvent::CLICK;
			else if (L"Do" == wsEvent)
				m_eEvent = EEvent::DO;
			else if (L"Po" == wsEvent)
				m_eEvent = EEvent::PO;

		}while (oReader.MoveToNextAttribute());

		oReader.MoveToElement();
	}

	const int nDepth{oReader.GetDepth()};
	std::string sName;

	while (oReader.ReadNextSiblingNode(nDepth))
	{
		sName = oReader.GetNameA();

		if ("ofd:Goto" == sName)
			SetAction(m_pAction, new CGoto(oReader));
		else if ("ofd:URI" == sName)
			SetAction(m_pAction, new CURI(oReader));
		else if ("ofd:GotoA" == sName)
			SetAction(m_pAction, new CGotoA(oReader));
		else if ("ofd:Sound" == sName)
			SetAction(m_pAction, new CSound(oReader));
		else if ("ofd:Movie" == sName)
			SetAction(m_pAction, new CMovie(oReader));
		else
			continue;

		return;
	}
}

CAction::~CAction()
{
	if(nullptr != m_pRegion)
		delete m_pRegion;

	if (nullptr != m_pAction)
		delete m_pAction;
}

const IAction* CAction::GetAction() const
{
	return m_pAction;
}

TBookmark::TBookmark()
{}

TBookmark* TBookmark::Read(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return nullptr;

	TBookmark* pBookmark = new TBookmark();

	if (nullptr == pBookmark)
	{
		oReader.MoveToElement();
		return nullptr;
	}

	do
	{
		if ("Name" == oReader.GetNameA())
			pBookmark->m_wsName = oReader.GetText();
	}while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	return pBookmark;
}

CGoto::CGoto(CXmlReader& oReader)
	: m_pDest(nullptr), m_pBookmark(nullptr)
{
	const int nDepth{oReader.GetDepth()};

	while (oReader.ReadNextSiblingNode(nDepth))
	{
		if("ofd:Dest" == oReader.GetNameA())
		{
			TDest *pDest{TDest::Read(oReader)};

			if (nullptr == pDest)
				continue;

			if (nullptr != m_pDest)
				delete m_pDest;

			m_pDest = pDest;
		}
		else if ("ofd:Bookmark" == oReader.GetNameA())
		{
			TBookmark *pBookmark{TBookmark::Read(oReader)};

			if (nullptr == pBookmark)
				continue;

			if (nullptr != m_pBookmark)
				delete m_pBookmark;

			m_pBookmark = pBookmark;
		}
	}
}

CGoto::~CGoto()
{
	if (nullptr != m_pDest)
		delete m_pDest;

	if (nullptr != m_pBookmark)
		delete m_pBookmark;
}

EActionType CGoto::GetType() const
{
	return EActionType::Goto;
}

const TDest* CGoto::GetDest() const
{
	return m_pDest;
}

const TBookmark* CGoto::GetBookmark() const
{
	return m_pBookmark;
}

CGotoA::CGotoA(CXmlReader& oReader)
	: m_bNewWindow(false)
{
	if (!oReader.MoveToFirstAttribute())
		return;

	do
	{
		if ("AttachID" == oReader.GetNameA())
			m_unAttachID = oReader.GetUInteger(true);
		else if ("NewWindow" == oReader.GetNameA())
			m_bNewWindow = oReader.GetBoolean(true);
	}while(oReader.MoveToNextAttribute());

	oReader.MoveToElement();
}

CGotoA::~CGotoA()
{}

EActionType CGotoA::GetType() const
{
	return EActionType::GotoA;
}

CURI::CURI(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return;

	do
	{
		if ("URI" == oReader.GetNameA())
			m_wsURI = oReader.GetText();
		else if ("Base" == oReader.GetNameA())
			m_wsBase = oReader.GetText();
	}while(oReader.MoveToNextAttribute());

	oReader.MoveToElement();
}

CURI::~CURI()
{}

EActionType CURI::GetType() const
{
	return EActionType::URI;
}

std::wstring CURI::GetURI() const
{
	return m_wsURI;
}

CSound::CSound(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return;

	std::string sName;

	do
	{
		sName = oReader.GetNameA();

		if ("ResourceID" == sName)
			m_unResourceID = oReader.GetUInteger(true);
		else if ("Volume" == sName)
			m_nVolume = oReader.GetInteger(true);
		else if ("Repeat" == sName)
			m_bRepeat = oReader.GetBoolean(true);
		else if ("Synchronous" == sName)
			m_bSynchronous = oReader.GetBoolean(true);
	}while(oReader.MoveToNextAttribute());

	oReader.MoveToElement();
}

CSound::~CSound()
{}

EActionType CSound::GetType() const
{
	return EActionType::Sound;
}

CMovie::CMovie(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return;

	do
	{
		if ("ResourceID" == oReader.GetNameA())
			m_unResourceID = oReader.GetUInteger(true);
		else if ("Operator" == oReader.GetNameA())
		{
			const std::string sValue{oReader.GetTextA()};

			if ("Play" == sValue)
				m_eOperator = EOperator::Play;
			else if ("Stop" == sValue)
				m_eOperator = EOperator::Stop;
			else if ("Pause" == sValue)
				m_eOperator = EOperator::Pause;
			else if ("Resume" == sValue)
				m_eOperator = EOperator::Resume;
		}
	}while(oReader.MoveToNextAttribute());

	oReader.MoveToElement();
}

CMovie::~CMovie()
{}

EActionType CMovie::GetType() const
{
	return EActionType::Movie;
}

}
