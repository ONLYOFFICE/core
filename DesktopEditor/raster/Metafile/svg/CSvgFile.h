#ifndef CSVGFILE_H
#define CSVGFILE_H

#include "../../../graphics/IRenderer.h"
#include "../../../graphics/config.h"
#include "../../../graphics/pro/Fonts.h"

#include "CSvgParser.h"
#include "SvgObjects/CStyle.h"
#include "SvgObjects/CDefs.h"

class GRAPHICS_DECL CSvgFile
{
	public:
		CSvgFile();
		~CSvgFile();

		bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize);
		bool OpenFromFile(const std::wstring& wsFile);
		bool Load(const std::wstring& wsContent);

		void Close();

		bool GetBounds(double& dX, double& dY, double& dWidth, double& dHeight) const;
		const SVG::CSvgCalculator* GetSvgCalculator() const;

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		void AddMarkedObject(const SVG::CSvgGraphicsObject* pObject);
		void AddStyles(const std::wstring& wsStyles);
		void AddDefs(XmlUtils::CXmlNode& oNode);

		const SVG::CSvgGraphicsObject* GetMarkedObject(const std::wstring& wsId) const;

		bool Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);

		SVG::CDefs *GetDefs();
	private:

		SVG::CSvgParser          m_oParser;
		SVG::CGraphicsContainer  *m_pContainer;
		SVG::CDefs               m_oDefs;
		SVG::CSvgCalculator      m_oSvgCalculator;

		typedef std::map<std::wstring, const SVG::CSvgGraphicsObject*> MarkedMap;
		MarkedMap m_mMarkedObjects;
};

#endif // CSVGFILE_H
