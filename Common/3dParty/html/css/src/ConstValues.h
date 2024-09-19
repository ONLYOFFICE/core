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

		void Clear();

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
				B_CustomStyle,
				B_StyleId,
				B_Type,
				B_Default,

				B_Name,
				B_BasedOn,
				B_QFormat,
				B_Link,
				B_UnhideWhenUsed,
				B_UiPriority,
				B_SemiHidden
			} BasicProperties;

			typedef enum
			{
				P_Jc,
				P_Spacing,
				P_ContextualSpacing,
				P_Ind,
				P_OutlineLvl,
				P_Shd,
				// <pBdr>
					P_TopBorder,
					P_LeftBorder,
					P_BottomBorder,
					P_RightBorder,
				// </pBdr>
				P_KeepLines,
				P_KeepNext,
			} ParagraphProperties;

			typedef enum
			{
				R_RFonts ,
				R_Sz,
				R_B,
				R_I,
				R_Color,
				R_U,
				R_Highlight,
				R_Shd,
				R_SmallCaps,
				R_Kern
			} RunnerProperties;

			typedef enum
			{
				T_TblInd ,
				// <tblCellMar>
					T_CellTop,
					T_CellLeft,
					T_CellBottom,
					T_CellRight,
				// <tblCellMar>

				// <tblBorders>
					T_BorderTop ,
					T_BorderLeft,
					T_BorderBottom,
					T_BorderRight,
					T_BorderInsideH,
					T_BorderInsideV 
				// </tblBorders>
			} TableProperties;
		}
		extern const std::wstring PSEUDO_CLASSES[33];
		#endif
	}
}

#endif // CONSTVALUES_H
