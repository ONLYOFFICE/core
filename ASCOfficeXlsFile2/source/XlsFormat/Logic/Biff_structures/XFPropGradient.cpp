
#include "XFPropGradient.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XFPropGradient::clone()
{
	return BiffStructurePtr(new XFPropGradient(*this));
}

void XFPropGradient::store(CFRecord& record)
{
#pragma message("####################### XFPropGradient record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradient record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradient::load(CFRecord& record)
{
	_UINT32 temp;
	
	record >> temp >> numDegree >> numFillToLeft >> numFillToRight >> numFillToTop >> numFillToBottom;
	
	type1 = temp;
}

void XFPropGradient::serialize_attr(CP_ATTR_NODE)
{
       //<xsl:if test="$gradient/@type = 'true'"> <!-- Default is 'linear' -->
    //    <xsl:attribute name="type">path</xsl:attribute>
    //  </xsl:if>
    //  <xsl:if test="$gradient/@numDegree != 0"> <!-- Default is 0 -->
    //    <xsl:attribute name="degree"><xsl:value-of select="$gradient/@numDegree"/></xsl:attribute>
    //  </xsl:if>
    //  <xsl:if test="$gradient/@numFillToLeft != 0"> <!-- Default is 0 -->
    //    <xsl:attribute name="left"><xsl:value-of select="$gradient/@numFillToLeft"/></xsl:attribute>
    //  </xsl:if>
    //  <xsl:if test="$gradient/@numFillToRight != 0"> <!-- Default is 0 -->
    //    <xsl:attribute name="right"><xsl:value-of select="$gradient/@numFillToRight"/></xsl:attribute>
    //  </xsl:if>
    //  <xsl:if test="$gradient/@numFillToTop != 0"> <!-- Default is 0 -->
    //    <xsl:attribute name="top"><xsl:value-of select="$gradient/@numFillToTop"/></xsl:attribute>
    //  </xsl:if>
    //  <xsl:if test="$gradient/@numFillToBottom != 0"> <!-- Default is 0 -->
    //    <xsl:attribute name="bottom"><xsl:value-of select="$gradient/@numFillToBottom"/></xsl:attribute>
    //  </xsl:if>

}

} // namespace XLS
