#ifndef CCLIP_H
#define CCLIP_H

#include "MetaFileTypes.h"
#include "CPath.h"

namespace MetaFile
{
	typedef enum
	{
		CLIP_COMMAND_UNKNOWM,
		CLIP_COMMAND_INSTERSECT,
		CLIP_COMMAND_SETPATH,
		CLIP_COMMAND_EXCLUDE
	} EClipCommandType;

	class CClipCommandBase
	{
	public:
		CClipCommandBase();
		virtual ~CClipCommandBase();
		virtual EClipCommandType GetType() const;
	};

	class CClipCommandIntersect : public CClipCommandBase
	{
	public:
		CClipCommandIntersect(const TRectD& oRect);
		CClipCommandIntersect(const CClipCommandIntersect& oCommandIntersect);
		virtual ~CClipCommandIntersect();
		virtual EClipCommandType GetType() const override;

		const TRectD& GetRect() const;
	private:
		TRectD m_oRect;
	};

	class CClipCommandPath : public CClipCommandBase
	{
	public:
		CClipCommandPath(const CPath& oPath, unsigned int unMode, const TXForm& oTransform = TXForm());
		CClipCommandPath(const CClipCommandPath& oCommandPath);
		virtual ~CClipCommandPath();
		virtual EClipCommandType GetType() const override;

		const CPath&  GetPath()      const;
		const TXForm& GetTransform() const;
		unsigned int  GetMode()      const;
	private:
		CPath        m_oPath;
		TXForm       m_oTransform;
		unsigned int m_unMode;
	};
	
	class CClipCommandExlude : public CClipCommandBase
	{
	public:
		CClipCommandExlude(const TRectD& oClip, const TRectD& oBB);
		CClipCommandExlude(const CClipCommandExlude& oCommandExlude);
		virtual ~CClipCommandExlude();
		virtual EClipCommandType GetType() const override;

		const TRectD& GetClip() const;
		const TRectD& GetBB()   const;
	private:
		TRectD m_oClip;
		TRectD m_oBB;
	};

	class CClip
	{
	public:
		CClip();
		CClip(const CClip& oClip);
		~CClip();

		void Copy(const CClip& oClip);
		bool Empty() const;

		void Reset();
		bool Intersect(const TRectD& oRect);
		bool Exclude(const TRectD& oClip, const TRectD& oBB);
		bool SetPath(const CPath& oPath, unsigned int unMode, const TXForm& oTransform = TXForm());

		void DrawOnRenderer(IOutputDevice* pOutput) const;

		const std::vector<CClipCommandBase*> GetCommands() const;
	private:
		bool AddCommand(CClipCommandBase* pCommande);
		void Clear();

		std::vector<CClipCommandBase*> m_arCommands;
	};
}

#endif // CCLIP_H
