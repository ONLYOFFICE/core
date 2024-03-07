#ifndef CONSTVALUES_H
#define CONSTVALUES_H

#include "CNode.h"

namespace NSCSS
{
	#ifndef RGB_TO_INT
	#define RGB_TO_INT(r, g, b) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) ) )
	#endif

	typedef enum
	{
		ScalingDirectionNone = 0,
		ScalingDirectionX = 1,
		ScalingDirectionY = 2
	} ScalingDirection;

	struct StatistickElement
	{
		enum TypeElement
		{
			IsStyle = 0,
			IsId
		} m_enType;
		std::wstring sValue;

		bool operator<(const StatistickElement& oStatistickElement) const;
	};

	struct CTree
	{
		NSCSS::CNode m_oNode;
		std::vector<CTree> m_arrChild;

		static void CountingNumberRepetitions(const CTree &oTree, std::map<StatistickElement, unsigned int> &mStatictics);
	};

	namespace NSConstValues
	{
		extern const std::map<std::wstring, std::wstring> COLORS;
		extern const std::vector<std::wstring> DISPLAY_VALUES;

		#ifdef CSS_CALCULATOR_WITH_XHTML
		namespace NSProperties
		{
			typedef enum
			{
				B_CustomStyle = 0,
				B_StyleId = 1,
				B_Type = 2,
				B_Default = 3,

				B_Name = 4,
				B_BasedOn = 5,
				B_QFormat = 6,
				B_Link = 7,
				B_UnhideWhenUsed = 8,
				B_UiPriority = 9,
			} BasicProperties;

			typedef enum
			{
				P_Jc = 0,
				P_Spacing = 1,
				P_ContextualSpacing = 2,
				P_Ind = 3,
				P_OutlineLvl = 4,
				P_Shd = 5,
				// <pBdr>
					P_TopBorder = 6,
					P_LeftBorder = 7,
					P_BottomBorder = 8,
					P_RightBorder = 9,
				// </pBdr>
				P_KeepLines = 10,
				P_KeepNext = 11,
			} ParagraphProperties;

			typedef enum
			{
				R_RFonts = 0,
				R_Sz = 1,
				R_B = 2,
				R_I = 3,
				R_Color = 4,
				R_U = 5,
				R_Highlight = 6,
				R_SmallCaps = 7,
				R_Kern = 8
			} RunnerProperties;

			typedef enum
			{
				T_TblInd = 0,
				// <tblCellMar>
					T_CellTop = 1,
					T_CellLeft = 2,
					T_CellBottom = 3,
					T_CellRight = 4,
				// <tblCellMar>

				// <tblBorders>
					T_BorderTop = 5,
					T_BorderLeft = 6,
					T_BorderBottom = 7,
					T_BorderRight = 8,
					T_BorderInsideH = 9,
					T_BorderInsideV = 10
				// </tblBorders>
			} TableProperties;
		}
		extern const std::wstring PSEUDO_CLASSES[33];
		#endif
	}
}

#endif // CONSTVALUES_H
