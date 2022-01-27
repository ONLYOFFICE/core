#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include "../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../PdfReader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LeftButton_clicked();
    void on_RightButton_clicked();
    void on_RenderButton_clicked();

    void on_OpenFileButton_clicked();

    void on_ScaleSlider_sliderMoved(int position);
    void on_verticalScrollBar_valueChanged(int value);
    void on_horizontalScrollBar_valueChanged(int value);

protected:

	void resizeEvent(QResizeEvent *event);
	void closeEvent(QCloseEvent *event);

private:

	void RenderPage();
    bool RenderOnByteData(int nPage, BYTE*& pBgraData, int& w, int& h);
    void SetImage();
    void OpenFile();

	bool IsFileOpened();
	double GetPageScaleByZoomType(const double& dPageWidth, const double& dPageHeight);

private:

	enum ZoomType
	{
		pageWidth,
		pageHeight,
		custom
	};

    QImage* m_pImage;
    QLabel* m_pLabel;

    QString m_sFile;
    int m_nPosX;
    int m_nPosY;
    double m_dScale;
	ZoomType m_eZoomType;

    // шрифты системы
    NSFonts::IApplicationFonts* m_pFonts;

    // кэши для отрисовщика
    NSFonts::IFontManager* m_pFontManager;
    NSImages::IImageFilesCache* m_pImageCache;

    PdfReader::CPdfReader* m_pReader;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
