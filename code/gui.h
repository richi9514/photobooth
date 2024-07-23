#ifndef GUI_H
#define GUI_H

#include "imageprozessor.h"

#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include <QPainter>

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui( QWidget *parent = nullptr);

    QPixmap CvMatToQPixmap(cv::Mat matImg, int Width, int Height);
    
private slots:
    void changeUsermodeButton();
    void changeLogoButton();
    void changeLogoEdgeButton();
    void changeLogoPosiHorizontalButton(QString newDistance);
    void changeLogoPosiVerticalButton(QString newDistance);
    void changeLogoSizeButton(QString newSize);
    void changeOverlayButton();
    void changePrintingModeButton();
    void startButton();
    void photoPrintButton();
    void updateFullGui();
    void updateImg();

private:
    QPushButton *btn[9];
    QLabel *lb_img, *lb_text;
    QLineEdit *Lines[3];
    QPrinter* myPrinter;
    QPrintDialog* myPrinterDialog;
    ImageProzessor *imgProzess;
    bool overlayChangeable, printigIsEnabled = FALSE;
    int iDisplayHeight = 480, iDisplayWidth = 800 , imgWidth, imgHeight;
    int autoStart, startOverlay, startLogo, startMode, startPrinting, startLogoSize, startLogoDistanceX, startLogoDistanceY, startLogoEdge;
    QTimer *updateGui;
    std::string userMode1 = "only picture";
    std::string userMode2 = "picture + logo";
    std::string userMode3 = "picture + overlay not changeable";
    std::string userMode4 = "picture + overlay changeable";
    std::string pLogos = ".\\Logos";
    std::string pOverlays = ".\\Overlays";
    std::string changeMode = "change usermode";
    std::string changOverlay = "change overlay";
    std::string changeLogo = "change logo";
    std::string changeLogoEdge = "change logo edge";
    std::string setLogoSize = "set logo size";
    std::string setLogoDisHori = "set logo distance hori";
    std::string setLogoDisVert = "set logo distance vert";
    std::string startBox = "start";
    std::string printingIsOn = "printing is on";
    std::string printingIsOff = "printing is off";
    std::string startPhoto = "s\nt\na\nr\nt";
    std::string print = "p\nr\ni\nn\nt";
    std::string isPrinting = "p\nr\ni\nn\nt\ns";
    std::string grid = "g\nr\ni\nd";
    std::string printer = "printer";
};

#endif // GUI_H
