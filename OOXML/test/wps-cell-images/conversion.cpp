/* SPDX-License-Identifier: AGPL-3.0-only; see LICENSE for additional terms. */
#include "../../XlsxFormat/Xlsx.h"
#include "../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../XlsxFormat/Worksheets/SheetData.h"
#include "../../XlsxFormat/Drawing/Drawing.h"
#include "../../XlsxFormat/Drawing/FromTo.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../../PPTXFormat/Logic/Pic.h"
#include "../../../DesktopEditor/common/Directory.h"
#include <gtest/gtest.h>

namespace
{
std::wstring GetWorkDir()
{
	return NSDirectory::CreateDirectoryWithUniqueName(NSFile::CFileBinary::GetTempPath());
}

void RemoveWorkDir(const std::wstring& dir)
{
	NSDirectory::DeleteDirectory(dir);
}

void WriteFixture(const std::wstring& dir, bool existingDrawing, bool externalImage = false)
{
	for (auto sub : {L"/_rels", L"/xl/_rels", L"/xl/worksheets/_rels", L"/xl/media", L"/xl/drawings/_rels"})
		NSDirectory::CreateDirectories(dir + sub);
	auto save = [&dir](const std::wstring& name, const std::wstring& xml) {
		NSFile::CFileBinary::SaveToFile(dir + name, xml);
	};
	save(L"/_rels/.rels", LR"(<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="xl/workbook.xml"/></Relationships>)");
	save(L"/xl/workbook.xml", LR"(<workbook xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"><sheets><sheet name="Photos" sheetId="1" r:id="rId1"/></sheets></workbook>)");
	// A nonstandard part name exercises relationship-based discovery.
	save(L"/xl/_rels/workbook.xml.rels", LR"(<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet" Target="worksheets/sheet1.xml"/><Relationship Id="rId2" Type="http://www.wps.cn/officeDocument/2020/cellImage" Target="photos.xml"/></Relationships>)");
	save(L"/xl/photos.xml", LR"(<etc:cellImages xmlns:etc="http://www.wps.cn/officeDocument/2017/etCustomData" xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"><etc:cellImage><xdr:pic><xdr:nvPicPr><xdr:cNvPr id="1" name="ID_PHOTO"/><xdr:cNvPicPr/></xdr:nvPicPr><xdr:blipFill><a:blip r:embed="rId1"/><a:stretch><a:fillRect/></a:stretch></xdr:blipFill><xdr:spPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="1000" cy="500"/></a:xfrm><a:prstGeom prst="rect"/></xdr:spPr></xdr:pic></etc:cellImage></etc:cellImages>)");
	save(L"/xl/_rels/photos.xml.rels", externalImage ?
		LR"(<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" Target="https://example.invalid/photo.png" TargetMode="External"/></Relationships>)" :
		LR"(<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" Target="media/photo.png"/></Relationships>)");
	std::wstring sheet = LR"(<worksheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"><cols><col min="2" max="2" width="20"/></cols><sheetData><row r="3" ht="80"><c r="A3"><v>17</v></c><c r="B3" t="e"><f>_xlfn.DISPIMG("ID_PHOTO",1)</f><v>#NAME?</v></c><c r="C3" t="e"><f>_xlfn.DISPIMG("MISSING",1)</f><v>#NAME?</v></c><c r="D3"><f>SUM(A3:A3)</f><v>17</v></c></row><row r="8" ht="80"><c r="B8" t="e"><f>_xlfn.DISPIMG("ID_PHOTO",1)</f><v>#NAME?</v></c></row></sheetData>)";
	if (existingDrawing) sheet += LR"(<drawing r:id="rId1"/>)";
	save(L"/xl/worksheets/sheet1.xml", sheet + L"</worksheet>");
	if (existingDrawing)
	{
		save(L"/xl/worksheets/_rels/sheet1.xml.rels", LR"(<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing" Target="../drawings/drawing1.xml"/></Relationships>)");
		save(L"/xl/drawings/drawing1.xml", LR"(<xdr:wsDr xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"><xdr:oneCellAnchor><xdr:from><xdr:col>0</xdr:col><xdr:colOff>0</xdr:colOff><xdr:row>0</xdr:row><xdr:rowOff>0</xdr:rowOff></xdr:from><xdr:ext cx="1000" cy="500"/><xdr:pic><xdr:nvPicPr><xdr:cNvPr id="42" name="Existing"/><xdr:cNvPicPr/></xdr:nvPicPr><xdr:blipFill><a:blip r:embed="rId1"/><a:stretch><a:fillRect/></a:stretch></xdr:blipFill><xdr:spPr><a:prstGeom prst="rect"/></xdr:spPr></xdr:pic><xdr:clientData/></xdr:oneCellAnchor></xdr:wsDr>)");
		save(L"/xl/drawings/_rels/drawing1.xml.rels", LR"(<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" Target="../media/photo.png"/></Relationships>)");
	}
	// Synthetic 2x1 PNG, unrelated to any user's workbook.
	const unsigned char png[] = {137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,2,0,0,0,1,8,2,0,0,0,123,64,232,221,0,0,0,13,73,68,65,84,120,156,99,248,207,0,4,255,1,7,0,1,255,226,35,158,89,0,0,0,0,73,69,78,68,174,66,96,130};
	NSFile::CFileBinary image;
	image.CreateFileW(dir + L"/xl/media/photo.png");
	image.WriteFile(png, sizeof(png));
	image.CloseFile();
}

OOX::Spreadsheet::CCell* Cell(OOX::Spreadsheet::CXlsx& book, const std::wstring& ref)
{
	for (auto row : book.m_arWorksheets[0]->m_oSheetData->m_arrItems)
		for (auto cell : row->m_arrItems)
			if (cell->getRef() == ref) return cell;
	return NULL;
}
}

TEST(WpsCellImages, ImportsRepeatedPhotosWithoutChangingOtherCellsOrDrawings)
{
	for (bool existing : {false, true})
	{
		const std::wstring dir = GetWorkDir();
		WriteFixture(dir, existing);
		OOX::Spreadsheet::CXlsx book;
		ASSERT_TRUE(book.Read(OOX::CPath(dir)));
		ASSERT_EQ(1u, book.m_arWorksheets.size());
		auto sheet = book.m_arWorksheets[0];
		ASSERT_TRUE(sheet->m_oDrawing.IsInit());
		auto drawing = sheet->Get<OOX::Spreadsheet::CDrawing>(OOX::RId(sheet->m_oDrawing->m_oId->GetValue()));
		ASSERT_TRUE(drawing.IsInit());
		EXPECT_EQ(existing ? 3u : 2u, drawing->m_arrItems.size());
		EXPECT_FALSE(Cell(book,L"B3")->m_oFormula.IsInit());
		EXPECT_FALSE(Cell(book,L"B8")->m_oFormula.IsInit());
		EXPECT_EQ(L"_xlfn.DISPIMG(\"MISSING\",1)", Cell(book,L"C3")->m_oFormula->m_sText);
		EXPECT_EQ(L"SUM(A3:A3)", Cell(book,L"D3")->m_oFormula->m_sText);
		EXPECT_EQ(L"17", Cell(book,L"A3")->m_oValue->m_sText);
		int cellRow = 0, cellCol = 0;
		ASSERT_TRUE(Cell(book,L"D3")->getRowCol(cellRow, cellCol));
		EXPECT_EQ(2, cellRow);
		EXPECT_EQ(3, cellCol);
		const auto first = drawing->m_arrItems[existing ? 1 : 0];
		const auto second = drawing->m_arrItems[existing ? 2 : 1];
		EXPECT_EQ(2u, first->m_oFrom->m_oRow->GetValue());
		EXPECT_EQ(7u, second->m_oFrom->m_oRow->GetValue());
		EXPECT_EQ(1u, first->m_oFrom->m_oCol->GetValue());
		EXPECT_NE(first->m_oElement->as<PPTX::Logic::Pic>().nvPicPr.cNvPr.id,
			second->m_oElement->as<PPTX::Logic::Pic>().nvPicPr.cNvPr.id);
		if (existing)
			EXPECT_EQ(42, drawing->m_arrItems[0]->m_oElement->as<PPTX::Logic::Pic>().nvPicPr.cNvPr.id);
		const std::wstring saved = GetWorkDir();
		OOX::CContentTypes content;
		ASSERT_TRUE(book.WriteNative(OOX::CPath(saved), content));
		OOX::Spreadsheet::CXlsx reopened;
		ASSERT_TRUE(reopened.Read(OOX::CPath(saved)));
		auto reopenedSheet = reopened.m_arWorksheets[0];
		auto reopenedDrawing = reopenedSheet->Get<OOX::Spreadsheet::CDrawing>(OOX::RId(reopenedSheet->m_oDrawing->m_oId->GetValue()));
		ASSERT_TRUE(reopenedDrawing.IsInit());
		EXPECT_EQ(existing ? 3u : 2u, reopenedDrawing->m_arrItems.size());
		// Empty photo cells may be omitted when the worksheet is written.
		const auto photoCell = Cell(reopened,L"B3");
		EXPECT_TRUE(!photoCell || !photoCell->m_oFormula.IsInit());
		ASSERT_TRUE(Cell(reopened,L"D3")) << std::string(saved.begin(), saved.end());
		ASSERT_TRUE(Cell(reopened,L"D3")->m_oFormula.IsInit()) << std::string(saved.begin(), saved.end());
		EXPECT_EQ(L"SUM(A3:A3)", Cell(reopened,L"D3")->m_oFormula->m_sText);
		RemoveWorkDir(saved);
		RemoveWorkDir(dir);
	}
}

TEST(WpsCellImages, DesktopStreamingPathFallsBackToImportBeforeSerialization)
{
	const std::wstring dir = GetWorkDir();
	WriteFixture(dir, false);
	NSBinPptxRW::CXlsbBinaryWriter writer;
	writer.CreateFileW(dir + L"/editor.bin");
	OOX::Spreadsheet::CXlsx book;
	book.m_pXlsbWriter = &writer;
	ASSERT_TRUE(book.Read(OOX::CPath(dir)));
	EXPECT_EQ(&writer, book.m_pXlsbWriter);
	EXPECT_FALSE(book.m_arWorksheets[0]->m_oSheetData->m_oXlsbPos.IsInit());
	EXPECT_FALSE(Cell(book,L"B3")->m_oFormula.IsInit());
	EXPECT_TRUE(book.m_arWorksheets[0]->m_oDrawing.IsInit());
	writer.CloseFile();
	RemoveWorkDir(dir);
}

TEST(WpsCellImages, LeavesExternalAndMissingImagesUnchanged)
{
	for (bool external : {false, true})
	{
		const std::wstring dir = GetWorkDir();
		WriteFixture(dir, false, external);
		if (!external) NSFile::CFileBinary::Remove(dir + L"/xl/media/photo.png");
		OOX::Spreadsheet::CXlsx book;
		ASSERT_TRUE(book.Read(OOX::CPath(dir)));
		EXPECT_FALSE(book.m_arWorksheets[0]->m_oDrawing.IsInit());
		EXPECT_TRUE(Cell(book,L"B3")->m_oFormula.IsInit());
		EXPECT_TRUE(Cell(book,L"B3")->m_oValue.IsInit());
		RemoveWorkDir(dir);
	}
}
