#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iterator>

#include <QImage>
#include <QPixmap>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->pushButton->SetColor(Qt::black);

	m_oAppFonts = NSFonts::NSApplication::Create();
	m_oAppFonts->Initialize();

	m_oFontManager = m_oAppFonts->GenerateFontManager();
	NSFonts::IFontsCache* fonts_cache = NSFonts::NSFontCache::Create();
	fonts_cache->SetStreams(m_oAppFonts->GetStreams());
	m_oFontManager->SetOwnerCache(fonts_cache);

	for (auto it = m_oAppFonts->GetList()->GetFonts()->cbegin(); it != m_oAppFonts->GetList()->GetFonts()->cend(); it++)
		ui->comboBox->addItem(QString::fromWCharArray((*it)->m_wsFontName.c_str()));

	connect(ui->textEdit,	&QTextEdit::textChanged,		this, &MainWindow::Draw);
	connect(ui->spinBox,	&QSpinBox::valueChanged,		this, &MainWindow::Draw);
	connect(ui->comboBox,	&QComboBox::currentTextChanged, this, &MainWindow::Draw);
	connect(ui->checkBox,	&QCheckBox::stateChanged,		this, &MainWindow::Draw);
	connect(ui->checkBox_2, &QCheckBox::stateChanged,		this, &MainWindow::Draw);
	connect(ui->pushButton, &CColorButton::ColorChanged,	this, &MainWindow::Draw);
}

MainWindow::~MainWindow()
{
	RELEASEOBJECT(m_oAppFonts);
	RELEASEOBJECT(m_oFontManager);
	delete ui;
}


void MainWindow::Draw()
{
	NSGraphics::IGraphicsRenderer* renderer = NSGraphics::Create();
	renderer->SetFontManager(m_oFontManager);

	int width = ui->label->width(),
		height = ui->label->height();

	BYTE* data = new BYTE[4 * width * height];

	CBgraFrame frame;
	frame.put_Data(data);
	frame.put_Width(width);
	frame.put_Height(height);
	frame.put_Stride(4 * width);

	renderer->put_Width(100);
	renderer->put_Height(100);

	renderer->CreateFromBgraFrame(&frame);
	renderer->SetSwapRGB(false);

	renderer->put_FontName(ui->comboBox->currentText().toStdWString());
	renderer->put_FontSize(ui->spinBox->value());

	long font_style = 0;
	if (ui->checkBox->isChecked())
		font_style |= 0x01;
	if (ui->checkBox_2->isChecked())
		font_style |= 0x02;
	renderer->put_FontStyle(font_style);

	renderer->put_BrushColor1(ui->pushButton->GetColor().rgb());

	auto lines = ui->textEdit->toPlainText().split('\n');
	double x = 1.0;
	double y =  (ui->spinBox->value() * 25.4 / 96.0) + 3.0;
	double scale_y = y;

	renderer->BeginCommand(c_nTextGraphicType);
	for (auto it = lines.cbegin(); it != lines.cend(); it++)
	{
		renderer->CommandDrawText((*it).toStdWString(), x, y, 0.0, 0.0);
		y += scale_y;
	}
	renderer->EndCommand(c_nTextGraphicType);

	QImage img = QImage(data, width, height, QImage::Format_RGBA8888, [](void *data){
		delete[] (BYTE*)data;
	});
	ui->label->setPixmap(QPixmap::fromImage(img));

	RELEASEOBJECT(renderer);
}
