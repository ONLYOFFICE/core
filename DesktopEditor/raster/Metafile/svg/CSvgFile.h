#ifndef CSVGFILE_H
#define CSVGFILE_H

#include "../../../graphics/IRenderer.h"
#include "../../../graphics/pro/Fonts.h"

#include "CSvgParser.h"
#include "SvgObjects/CStyle.h"

class CSvgFile
{
	public:
		CSvgFile();
		~CSvgFile();

		bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize);
		bool OpenFromFile(const std::wstring& wsFile);

		bool GetBounds(double& dX, double& dY, double& dWidth, double& dHeight) const;
		const SVG::CSvgCalculator* GetSvgCalculator() const;

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		bool MarkObject(SVG::CObject* pObject);
		SVG::CObject* GetMarkedObject(const std::wstring& wsId) const;

		std::wstring GetWorkingDirectory() const;
		
		void AddStyles(const std::wstring& wsStyles);

		bool Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
	private:
		void Clear();

		SVG::CSvgParser          m_oParser;
		SVG::CGraphicsContainer  m_oContainer;
		SVG::CSvgCalculator      m_oSvgCalculator;

		typedef std::map<std::wstring, SVG::CObject*> MarkedMap;

		MarkedMap    m_mMarkedObjects;
		std::wstring m_wsWorkingDirectory;
};

#endif // CSVGFILE_H
