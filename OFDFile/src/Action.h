#ifndef ACTION_H
#define ACTION_H

#include "Utils/XmlReader.h"
#include "Types/Region.h"

namespace OFD
{
enum class EActionType
{
	Goto,
	GotoA, // Transition to attachment
	URI,   // External link
	Sound, // Audio playback
	Movie  // Video playback
};

class IAction
{
public:
	IAction() = default;
	virtual ~IAction() = default;

	virtual EActionType GetType() const = 0;
};

class CAction
{
	enum class EEvent
	{
		DO,   //Opened document
		PO,   //Opened page
		CLICK
	} m_eEvent;

	CRegion* m_pRegion;
	const IAction *m_pAction;
public:
	CAction(CXmlReader& oReader);
	~CAction();

	const IAction* GetAction() const;
};

struct TBookmark
{
	std::wstring m_wsName;

	TBookmark();

	static TBookmark* Read(CXmlReader& oReader);
};

class CGoto : public IAction
{
	const TDest*     m_pDest;
	const TBookmark* m_pBookmark;
public:
	CGoto(CXmlReader& oReader);
	~CGoto();

	EActionType GetType() const override;

	const TDest* GetDest() const;
	const TBookmark* GetBookmark() const;
};

class CGotoA : public IAction
{
	UINT m_unAttachID;
	bool m_bNewWindow;
public:
	CGotoA(CXmlReader& oReader);
	~CGotoA();

	EActionType GetType() const override;
};

class CURI : public IAction
{
	std::wstring m_wsURI;
	std::wstring m_wsBase;
public:
	CURI(CXmlReader& oReader);
	~CURI();

	EActionType GetType() const override;

	std::wstring GetURI() const;
};

class CSound : public IAction
{
	UINT m_unResourceID;
	int  m_nVolume;
	bool m_bRepeat;
	bool m_bSynchronous;
public:
	CSound(CXmlReader& oReader);
	~CSound();

	EActionType GetType() const override;
};

class CMovie : public IAction
{
	UINT m_unResourceID;
	enum class EOperator
	{
		Play,
		Stop,
		Pause,
		Resume
	} m_eOperator;
public:
	CMovie(CXmlReader& oReader);
	~CMovie();

	EActionType GetType() const override;
};
}

#endif // ACTION_H
