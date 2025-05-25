#include "OleConverter.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

namespace HWP
{
COleConverter::COleConverter()
	: m_unCountCharts(0)
{}

void COleConverter::Clear()
{
	m_unCountCharts = 0;
	m_wsTempDir.clear();
}

unsigned int COleConverter::GetChartsCount() const
{
	return m_unCountCharts;
}

void COleConverter::SetTempDir(const std::wstring& wsTempDir)
{
	m_wsTempDir = wsTempDir;
}
void COleConverter::CreateChartData(const std::wstring& wsChartData)
{
	const std::wstring wsPath = m_wsTempDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"charts" + FILE_SEPARATOR_STR;

	NSFile::CFileBinary oChartFile;
	oChartFile.CreateFileW(wsPath + L"chart" + std::to_wstring(m_unCountCharts) + L".xml");
	oChartFile.WriteStringUTF8(wsChartData);
	oChartFile.CloseFile();

	NSStringUtils::CStringBuilder oColorData;
	oColorData.WriteString(L"<cs:colorStyle xmlns:cs=\"http://schemas.microsoft.com/office/drawing/2012/chartStyle\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" meth=\"cycle\" id=\"10\">");
	oColorData.WriteString(L"<a:schemeClr val=\"accent1\"/><a:schemeClr val=\"accent2\"/><a:schemeClr val=\"accent3\"/><a:schemeClr val=\"accent4\"/><a:schemeClr val=\"accent5\"/><a:schemeClr val=\"accent6\"/>");
	oColorData.WriteString(L"<cs:variation/><cs:variation><a:lumMod val=\"60000\"/></cs:variation><cs:variation><a:lumMod val=\"80000\"/><a:lumOff val=\"20000\"/></cs:variation>");
	oColorData.WriteString(L"<cs:variation><a:lumMod val=\"80000\"/></cs:variation><cs:variation><a:lumMod val=\"60000\"/><a:lumOff val=\"40000\"/></cs:variation>");
	oColorData.WriteString(L"<cs:variation><a:lumMod val=\"50000\"/></cs:variation><cs:variation><a:lumMod val=\"70000\"/><a:lumOff val=\"30000\"/></cs:variation>");
	oColorData.WriteString(L"<cs:variation><a:lumMod val=\"70000\"/></cs:variation><cs:variation><a:lumMod val=\"50000\"/><a:lumOff val=\"50000\"/></cs:variation>");
	oColorData.WriteString(L"</cs:colorStyle>");

	NSFile::CFileBinary oColorFile;
	oColorFile.CreateFileW(wsPath + L"colors" + std::to_wstring(m_unCountCharts) + L".xml");
	oColorFile.WriteStringUTF8(oColorData.GetData());
	oColorFile.CloseFile();

	NSStringUtils::CStringBuilder oStyleData;
	oStyleData.WriteString(L"<cs:chartStyle xmlns:cs=\"http://schemas.microsoft.com/office/drawing/2012/chartStyle\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" id=\"251\">");
	oStyleData.WriteString(L"<cs:axisTitle><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"1000\" kern=\"1200\"/></cs:axisTitle>");
	oStyleData.WriteString(L"<cs:categoryAxis><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"15000\"/><a:lumOff val=\"85000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:categoryAxis>");
	oStyleData.WriteString(L"<cs:chartArea mods=\"allowNoFillOverride allowNoLineOverride\"><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:solidFill><a:schemeClr val=\"bg1\"/></a:solidFill><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"15000\"/><a:lumOff val=\"85000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:chartArea>");
	oStyleData.WriteString(L"<cs:dataLabel><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"75000\"/><a:lumOff val=\"25000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:dataLabel>");
	oStyleData.WriteString(L"<cs:dataLabelCallout><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"dk1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:spPr><a:solidFill><a:schemeClr val=\"lt1\"/></a:solidFill><a:ln><a:solidFill><a:schemeClr val=\"dk1\"><a:lumMod val=\"25000\"/><a:lumOff val=\"75000\"/></a:schemeClr></a:solidFill></a:ln></cs:spPr><cs:defRPr sz=\"900\" kern=\"1200\"/><cs:bodyPr rot=\"0\" spcFirstLastPara=\"1\" vertOverflow=\"clip\" horzOverflow=\"clip\" vert=\"horz\" wrap=\"square\" lIns=\"36576\" tIns=\"18288\" rIns=\"36576\" bIns=\"18288\" anchor=\"ctr\" anchorCtr=\"1\"><a:spAutoFit/></cs:bodyPr></cs:dataLabelCallout>");
	oStyleData.WriteString(L"<cs:dataPoint><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"1\"><cs:styleClr val=\"auto\"/></cs:fillRef><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"19050\"><a:solidFill><a:schemeClr val=\"lt1\"/></a:solidFill></a:ln></cs:spPr></cs:dataPoint>");
	oStyleData.WriteString(L"<cs:dataPoint3D><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"1\"><cs:styleClr val=\"auto\"/></cs:fillRef><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"25400\"><a:solidFill><a:schemeClr val=\"lt1\"/></a:solidFill></a:ln></cs:spPr></cs:dataPoint3D>");
	oStyleData.WriteString(L"<cs:dataPointLine><cs:lnRef idx=\"0\"><cs:styleClr val=\"auto\"/></cs:lnRef><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"28575\" cap=\"rnd\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:round/></a:ln></cs:spPr></cs:dataPointLine>");
	oStyleData.WriteString(L"<cs:dataPointMarker><cs:lnRef idx=\"0\"><cs:styleClr val=\"auto\"/></cs:lnRef><cs:fillRef idx=\"1\"><cs:styleClr val=\"auto\"/></cs:fillRef><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></cs:spPr></cs:dataPointMarker>");
	oStyleData.WriteString(L"<cs:dataPointMarkerLayout symbol=\"circle\" size=\"5\"/>");
	oStyleData.WriteString(L"<cs:dataPointWireframe><cs:lnRef idx=\"0\"><cs:styleClr val=\"auto\"/></cs:lnRef><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"rnd\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:round/></a:ln></cs:spPr></cs:dataPointWireframe>");
	oStyleData.WriteString(L"<cs:dataTable><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:spPr><a:noFill/><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"15000\"/><a:lumOff val=\"85000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:dataTable>");
	oStyleData.WriteString(L"<cs:downBar><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:solidFill><a:schemeClr val=\"dk1\"><a:lumMod val=\"75000\"/><a:lumOff val=\"25000\"/></a:schemeClr></a:solidFill><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:downBar>");
	oStyleData.WriteString(L"<cs:dropLine><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"35000\"/><a:lumOff val=\"65000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:dropLine>");
	oStyleData.WriteString(L"<cs:errorBar><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:errorBar>");
	oStyleData.WriteString(L"<cs:floor><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:noFill/><a:ln><a:noFill/></a:ln></cs:spPr></cs:floor>");
	oStyleData.WriteString(L"<cs:gridlineMajor><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"15000\"/><a:lumOff val=\"85000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:gridlineMajor>");
	oStyleData.WriteString(L"<cs:gridlineMinor><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"5000\"/><a:lumOff val=\"95000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:gridlineMinor>");
	oStyleData.WriteString(L"<cs:hiLoLine><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"50000\"/><a:lumOff val=\"50000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:hiLoLine>");
	oStyleData.WriteString(L"<cs:leaderLine><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"35000\"/><a:lumOff val=\"65000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:leaderLine>");
	oStyleData.WriteString(L"<cs:legend><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:legend>");
	oStyleData.WriteString(L"<cs:plotArea mods=\"allowNoFillOverride allowNoLineOverride\"><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef></cs:plotArea>");
	oStyleData.WriteString(L"<cs:plotArea3D mods=\"allowNoFillOverride allowNoLineOverride\"><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef></cs:plotArea3D>");
	oStyleData.WriteString(L"<cs:seriesAxis><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:seriesAxis>");
	oStyleData.WriteString(L"<cs:seriesLine><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"35000\"/><a:lumOff val=\"65000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:seriesLine>");
	oStyleData.WriteString(L"<cs:title><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"1400\" b=\"0\" kern=\"1200\" spc=\"0\" baseline=\"0\"/></cs:title>");
	oStyleData.WriteString(L"<cs:trendline><cs:lnRef idx=\"0\"><cs:styleClr val=\"auto\"/></cs:lnRef><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:ln w=\"19050\" cap=\"rnd\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"sysDot\"/></a:ln></cs:spPr></cs:trendline>");
	oStyleData.WriteString(L"<cs:trendlineLabel><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:trendlineLabel>");
	oStyleData.WriteString(L"<cs:upBar><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:solidFill><a:schemeClr val=\"lt1\"/></a:solidFill><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></a:solidFill><a:round/></a:ln></cs:spPr></cs:upBar>");
	oStyleData.WriteString(L"<cs:valueAxis><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"><a:lumMod val=\"65000\"/><a:lumOff val=\"35000\"/></a:schemeClr></cs:fontRef><cs:defRPr sz=\"900\" kern=\"1200\"/></cs:valueAxis>");
	oStyleData.WriteString(L"<cs:wall><cs:lnRef idx=\"0\"/><cs:fillRef idx=\"0\"/><cs:effectRef idx=\"0\"/><cs:fontRef idx=\"minor\"><a:schemeClr val=\"tx1\"/></cs:fontRef><cs:spPr><a:noFill/><a:ln><a:noFill/></a:ln></cs:spPr></cs:wall>");
	oStyleData.WriteString(L"</cs:chartStyle>");

	NSFile::CFileBinary oStyleFile;
	oStyleFile.CreateFileW(wsPath + L"style" + std::to_wstring(m_unCountCharts) + L".xml");
	oStyleFile.WriteStringUTF8(oStyleData.GetData());
	oStyleFile.CloseFile();
}

void COleConverter::CreateChart(CHWPStream& oOleStream)
{
	std::string sData = std::string(oOleStream.GetCurPtr(), oOleStream.GetSize());

	size_t unBegin = sData.find("<?xml");

	if (std::string::npos == unBegin)
		return;

	size_t unEnd = sData.find("</c:chartSpace>", unBegin);

	if (std::string::npos == unEnd)
		return;

	const std::string sCharDataA = sData.substr(unBegin, unEnd - unBegin + 15);
	const std::wstring wsChartData = UTF8_TO_U(sCharDataA);

	const std::wstring wsPath = m_wsTempDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"charts";

	if (!NSDirectory::Exists(wsPath))
		NSDirectory::CreateDirectory(wsPath);

	++m_unCountCharts;

	CreateChartData(wsChartData);

	const std::wstring wsRelsPath = wsPath + FILE_SEPARATOR_STR + L"_rels";

	if (!NSDirectory::Exists(wsRelsPath))
		NSDirectory::CreateDirectory(wsRelsPath);

	NSStringUtils::CStringBuilder oRelsData;
	oRelsData.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
	oRelsData.WriteString(L"<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");
	oRelsData.WriteString(L"<Relationship Id=\"rId1\" Type=\"http://schemas.microsoft.com/office/2011/relationships/chartStyle\" Target=\"style" + std::to_wstring(m_unCountCharts) + L".xml\"/>");
	oRelsData.WriteString(L"<Relationship Id=\"rId2\" Type=\"http://schemas.microsoft.com/office/2011/relationships/chartColorStyle\" Target=\"colors" + std::to_wstring(m_unCountCharts) + L".xml\"/>");
	oRelsData.WriteString(L"</Relationships>");

	NSFile::CFileBinary oRelsFile;
	oRelsFile.CreateFileW(wsRelsPath + FILE_SEPARATOR_STR + L"chart" + std::to_wstring(m_unCountCharts) + L".xml.rels");
	oRelsFile.WriteStringUTF8(oRelsData.GetData());
	oRelsFile.CloseFile();
}
}
