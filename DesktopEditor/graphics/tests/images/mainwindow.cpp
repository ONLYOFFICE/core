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
	ui->label->setFixedWidth(310);
	ui->comboBox->addItems(m_arExtensions);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	m_arFileNames = QFileDialog::getOpenFileNames(this,
												  "Select one or more images",
												  "",
												  "*.bmp *.gif *.jpg *.png *.ico "
												  "*.tif *.tga *.pcx *.wbmp *.jp2 "
												  "*.jpc *.pgx *.pnm *.ras *.jbg "
												  "*.mng *.ska *.raw *.psd *.pic");

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

	auto ext = ui->comboBox->currentText();

	NSUnicodeConverter::CUnicodeConverter converter;
	for (const auto& f : m_arFileNames)
	{
		ui->label->setText("File:" + f);
		Aggplus::CImage img(converter.toUnicode(f.toStdString(), 1));
		img.SaveFile(converter.toUnicode(QDir(save_dir).filePath("res." + ext).toStdString(), 1),
					 m_mapExtensionCodes.value(ext));
	}
}



