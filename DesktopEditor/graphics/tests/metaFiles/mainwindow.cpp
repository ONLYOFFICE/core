#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QSizePolicy>
#include <QDir>

#include "../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../pro/Graphics.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->label->setFixedWidth(160);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	m_arFileNames = QFileDialog::getOpenFileNames(this,
												  "Select one or more metafiles to convert",
												  "",
												  "*.emf *.wmf *.svm *.svg");
	if (!m_arFileNames.empty())
		ui->label->setText("File:" + m_arFileNames[0]);
}


void MainWindow::on_pushButton_2_clicked()
{
	auto save_dir = QFileDialog::getExistingDirectory(this,
													  "Open Directory",
													  "",
													  QFileDialog::ShowDirsOnly |
													  QFileDialog::DontResolveSymlinks);

	NSUnicodeConverter::CUnicodeConverter converter;
	for (const auto& f : m_arFileNames)
	{
		ui->label->setText("File:" + f);
		NSFonts::IApplicationFonts* app_fonts = NSFonts::NSApplication::Create();
		app_fonts->Initialize();
		MetaFile::IMetaFile* meta_file = MetaFile::Create(app_fonts);
		if (!meta_file->LoadFromFile(converter.toUnicode(f.toStdString(), 1).c_str()))
			return;

		double x, y, w, h;
		meta_file->GetBounds(&x, &y, &w, &h);

		if (ui->radioButton->isChecked())
			meta_file->ConvertToRaster(converter.toUnicode(QDir(save_dir).filePath("res.bmp").toStdString(), 1).c_str(), 1,
									   static_cast<int>(w) + 1, static_cast<int>(h) + 1);
		else if (ui->radioButton_2->isChecked())
			meta_file->ConvertToXml(converter.toUnicode(QDir(save_dir).filePath("res.xml").toStdString(), 1).c_str());
		else if (ui->radioButton_3->isChecked())
			meta_file->ConvertToRaster(converter.toUnicode(QDir(save_dir).filePath("res.bmp").toStdString(), 1).c_str(), 1,
									   static_cast<int>(w) + 1, static_cast<int>(h) + 1,
									   converter.toUnicode(QDir(save_dir).filePath("res.xml").toStdString(), 1).c_str());
		else if (ui->radioButton_4->isChecked())
			meta_file->ConvertToSvg(static_cast<int>(w) + 1, static_cast<int>(h) + 1);
		else if (ui->radioButton_5->isChecked())
			meta_file->ConvertToEmf(converter.toUnicode(QDir(save_dir).filePath("res.emf").toStdString(), 1).c_str());

		RELEASEOBJECT(app_fonts);
		RELEASEOBJECT(meta_file);
	}
}

