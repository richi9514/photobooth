#include "gui.h"


QPixmap Gui::CvMatToQPixmap(cv::Mat matImg, int Width, int Height)
{
    if (matImg.rows != Height || matImg.cols != Width)
    {
        cv::resize(matImg, matImg, cv::Size(Width, Height), cv::INTER_NEAREST);
    }
    
    if (matImg.type() == CV_8UC4) //32 bit image
    {
        QImage img= QImage( matImg.data, //image data
                            matImg.cols, //image width
                            matImg.rows, //image height
                            matImg.step, //image matrix element type
                            QImage::Format_RGBA8888); //image pixel format

        return QPixmap::fromImage(img.rgbSwapped());
    }
    else if (matImg.type() == CV_8UC3) //24 bit image
    {
        QImage img= QImage( matImg.data, //image data
                            matImg.cols, //image width
                            matImg.rows, //image height
                            matImg.step, //image matrix element type
                            QImage::Format_RGB888); //image pixel format

        return QPixmap::fromImage(img.rgbSwapped());
    }
    else if (matImg.type() == CV_8UC1) //8 bit image
    {
        QImage img= QImage( matImg.data, //image data
                            matImg.cols, //image width
                            matImg.rows, //image height
                            matImg.step, //image matrix element type
                            QImage::Format_Grayscale8); //image pixel format

        return QPixmap::fromImage(img.rgbSwapped());
    }
    QPixmap img2;
    return img2;
}

Gui::Gui(QWidget *parent)
    : QMainWindow(parent)
{
    std::ifstream metaData;
    metaData.open("config.txt");
    std::string newLine;

    if (metaData.is_open())
    {
        while(std::getline(metaData,newLine))
        {
            if (newLine.rfind("displayheigt") == 0)
            {
                iDisplayHeight = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }

            if (newLine.rfind("displaywidth") == 0)
            {
                iDisplayWidth = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }

            if (newLine.rfind("plogos") == 0)
            {
                pLogos = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("poverlays") == 0)
            {
                pOverlays = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("userMode1") == 0)
            {
                userMode1 = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("userMode2") == 0)
            {
                userMode2 = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("userMode3") == 0)
            {
                userMode3 = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("userMode4") == 0)
            {
                userMode4 = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("changeMode") == 0)
            {
                changeMode = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("changOverlay") == 0)
            {
                changOverlay = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("changeLogo") == 0)
            {
                changeLogo = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("changeEdgeLogo") == 0)
            {
                changeLogoEdge = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("setLogoSize") == 0)
            {
                setLogoSize = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("setLogoDisHori") == 0)
            {
                setLogoDisHori = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("setLogoDisVert") == 0)
            {
                setLogoDisVert = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("startBox") == 0)
            {
                startBox = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("printingIsOn") == 0)
            {
                printingIsOn = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("printingIsOff") == 0)
            {
                printingIsOff = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("startPhoto") == 0)
            {
                startPhoto = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("print") == 0)
            {
                print = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("isPrinting") == 0)
            {
                isPrinting = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }

            if (newLine.rfind("grid") == 0)
            {
                grid = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }
            
            if (newLine.rfind("name_printer") == 0)
            {
                printer = newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1);
            }
            
            if (newLine.rfind("startMode") == 0)
            {
                startMode = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startLogoNumber") == 0)
            {
                startLogo = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startOverlay") == 0)
            {
                startOverlay = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("autoStart") == 0)
            {
                autoStart = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startPrinting") == 0)
            {
                startPrinting = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startLogoSize") == 0)
            {
                startLogoSize = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startLogoDistanceX") == 0)
            {
                startLogoDistanceX = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startLogoDistanceY") == 0)
            {
                startLogoDistanceY = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
            
            if (newLine.rfind("startLogoEdge") == 0)
            {
                startLogoEdge = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }
        }
    }

    std::string helper = "";

    for(int i = 0; i < startPhoto.size(); i++)
    {
        helper = helper + startPhoto.substr(i,1);
        if (i < startPhoto.size() - 1)
        {
            helper = helper + "\n";
        }
    }

    startPhoto = helper;

    helper = "";

    for(int i = 0; i < print.size(); i++)
    {
        helper = helper + print.substr(i,1);
        if (i < print.size() - 1)
        {
            helper = helper + "\n";
        }
    }

    print = helper;

    helper = "";

    for(int i = 0; i < isPrinting.size(); i++)
    {
        helper = helper + isPrinting.substr(i,1);
        if (i < isPrinting.size() - 1)
        {
            helper = helper + "\n";
        }
    }

    isPrinting = helper;

    helper = "";

    for(int i = 0; i < grid.size(); i++)
    {
        helper = helper + grid.substr(i,1);
        if (i < grid.size() - 1)
        {
            helper = helper + "\n";
        }
    }

    grid = helper;

    overlayChangeable = false;

    imgWidth = ((iDisplayHeight * 3) / 4);
    imgHeight = (iDisplayHeight / 2);
    int controlWidth = ( iDisplayWidth - imgWidth - 10 ) / 2;

    imgProzess = new ImageProzessor(pLogos,pOverlays, iDisplayHeight);
    lb_img = new QLabel("I'm an Image", this);
    lb_img ->setGeometry(QRect(QPoint(0, 0), QSize(imgWidth, imgHeight)));
    if (imgProzess->getImageIsReady())
    {
        lb_img ->setPixmap(CvMatToQPixmap(imgProzess->getImage(), imgWidth, imgHeight));
    }


    //usermode
    lb_text = new QLabel(QString::fromStdString(userMode1), this);
    lb_text ->setGeometry(QRect(QPoint(imgWidth + 10, 0), QSize(controlWidth * 2, 50)));

    btn[0] = new QPushButton(QString::fromStdString(changeMode), this);
    btn[0]->setGeometry(QRect(QPoint(imgWidth + 10, 50), QSize(controlWidth, 50)));
    connect(btn[0], &QPushButton::released, this, &Gui::changeUsermodeButton);

    btn[1] = new QPushButton(QString::fromStdString(changOverlay), this);
    btn[1]->setGeometry(QRect(QPoint(imgWidth + 10 + controlWidth, 50), QSize(controlWidth, 50)));
    connect(btn[1], &QPushButton::released, this, &Gui::changeOverlayButton);

    btn[2] = new QPushButton(QString::fromStdString(changeLogo), this);
    btn[2]->setGeometry(QRect(QPoint(imgWidth + 10, 100), QSize(controlWidth, 50)));
    connect(btn[2], &QPushButton::released, this, &Gui::changeLogoButton);

    btn[3] = new QPushButton(QString::fromStdString(changeLogoEdge), this);
    btn[3]->setGeometry(QRect(QPoint(imgWidth + 10 + controlWidth, 100), QSize(controlWidth, 50)));
    connect(btn[3], &QPushButton::released, this, &Gui::changeLogoEdgeButton);

    Lines[0] = new QLineEdit(this);
    Lines[0]->setGeometry(QRect(QPoint(imgWidth + 10, 150), QSize(controlWidth, 50)));
    Lines[0]->setText(QString::fromStdString(std::to_string(imgProzess->getLogoSize())));

    btn[4] = new QPushButton(QString::fromStdString(setLogoSize), this);
    btn[4]->setGeometry(QRect(QPoint(imgWidth + 10 + controlWidth , 150), QSize(controlWidth, 50)));
    connect(btn[4], &QPushButton::released, this, [=](){this->changeLogoSizeButton(Lines[0]->text());});

    Lines[1] = new QLineEdit(this);
    Lines[1]->setGeometry(QRect(QPoint(imgWidth + 10, 200), QSize(controlWidth, 50)));
    Lines[1]->setText(QString::fromStdString(std::to_string(imgProzess->getLogoDistanceHorizontal())));

    btn[5] = new QPushButton(QString::fromStdString(setLogoDisHori), this);
    btn[5]->setGeometry(QRect(QPoint(imgWidth + 10 + controlWidth , 200), QSize(controlWidth, 50)));
    connect(btn[5], &QPushButton::released, this, [=](){this->changeLogoPosiHorizontalButton(Lines[1]->text());});

    Lines[2] = new QLineEdit(this);
    Lines[2]->setGeometry(QRect(QPoint(imgWidth + 10, 250), QSize(controlWidth, 50)));
    Lines[2]->setText(QString::fromStdString(std::to_string(imgProzess->getLogoDistanceVertical())));

    btn[6] = new QPushButton(QString::fromStdString(setLogoDisVert), this);
    btn[6]->setGeometry(QRect(QPoint(imgWidth + 10 + controlWidth, 250), QSize(controlWidth, 50)));
    connect(btn[6], &QPushButton::released, this, [=](){this->changeLogoPosiVerticalButton(Lines[2]->text());});

    btn[7] = new QPushButton(QString::fromStdString(printingIsOff), this);
    btn[7]->setGeometry(QRect(QPoint(imgWidth + 10, 300), QSize(controlWidth, 50)));
    connect(btn[7], &QPushButton::released, this, &Gui::changePrintingModeButton);

    btn[8] = new QPushButton(QString::fromStdString(startBox), this);
    btn[8]->setGeometry(QRect(QPoint(imgWidth + 10  + controlWidth, 300), QSize(controlWidth, 50)));
    connect(btn[8], &QPushButton::released, this, &Gui::startButton);

    updateGui = new QTimer(this);
    connect(updateGui, &QTimer::timeout, this, &Gui::updateImg);
    updateGui->start();
    
    if (autoStart == 1 )
    {
        if (startPrinting == 1)
        {
            printigIsEnabled = true;
        }
        else
        {
            printigIsEnabled = false;
        }
        
        switch (startMode)
        {
        case 0 :
            lb_text->setText(QString::fromStdString(userMode1));
            imgProzess->setPiuctureMode(0);
            break;
        case 1 :
            lb_text->setText(QString::fromStdString(userMode2));
            imgProzess->setPiuctureMode(1);
            if (imgProzess->getNumberOfLogo() > startLogo)
            {
                imgProzess->setLogo(startLogo);
            }
            else
            {
                imgProzess->setLogo(0);
            }
            
            try
            {
                imgProzess->setLogoSize(startLogoSize);
            }
            catch (...)
            {
                imgProzess->setLogoSize(50);
            }
                        
            try
            {
                imgProzess->setLogoDistanceHorizontal(startLogoDistanceX);
            }
            catch (...)
            {
                imgProzess->setLogoDistanceHorizontal(0);
            }
            
            try
            {
                imgProzess->setLogoDistanceVertical(startLogoDistanceY);
            }
            catch (...)
            {
                imgProzess->setLogoDistanceVertical(0);
            }
            
            if (startLogoEdge >= 0 && startLogoEdge <= 3 )
            {       
                imgProzess->setLogoEdge(startLogoEdge);
            }
            else
            {
                imgProzess->setLogoEdge(0);
            }
            
            break;
        case 2 :
            overlayChangeable = false;
            lb_text->setText(QString::fromStdString(userMode3));
            imgProzess->setPiuctureMode(2);
            if (imgProzess->getNumberOfOverlays() > startOverlay)
            {
                imgProzess->setOverlay(startOverlay);
            }
            else
            {
                imgProzess->setOverlay(0);
            }
            break;
        case 3 :
            overlayChangeable = true;
            lb_text->setText(QString::fromStdString(userMode4));
            imgProzess->setPiuctureMode(2);
            if (imgProzess->getNumberOfOverlays() > startOverlay)
            {
                imgProzess->setOverlay(startOverlay);
            }
            else
            {
                imgProzess->setOverlay(0);
            }
            break;
        default:
            lb_text->setText(QString::fromStdString(userMode1));
            imgProzess->setPiuctureMode(0);
            break;
        }
        startButton();
    }
}

void Gui::startButton()
{
    updateGui->stop();
    
    if (printigIsEnabled == true)
    {
        myPrinter = new QPrinter;

        myPrinter->setPrinterName(QString::fromStdString(printer));
        
        myPrinter->setFullPage(true);

        
        myPrinter->setPageOrientation(QPageLayout::Landscape);
        
        QMarginsF myQMarginsF;
        myQMarginsF.setBottom(0.0);
        myQMarginsF.setTop(0.0);
        myQMarginsF.setRight(0.0);
        myQMarginsF.setLeft(0.0);
        const QMarginsF& myQMarginsF2 = myQMarginsF;
        myPrinter->setPageMargins(myQMarginsF2);
        myPrinterDialog = new QPrintDialog(myPrinter);
        myPrinterDialog->exec();
    }

    if (!overlayChangeable)
    {
        btn[1]->hide();
    }
    btn[2]->hide();
    btn[3]->hide();
    btn[4]->hide();
    btn[5]->hide();
    btn[6]->hide();
    btn[7]->hide();
    btn[8]->hide();
    Lines[0]->hide();
    Lines[1]->hide();
    Lines[2]->hide();

    imgWidth = ((iDisplayHeight * 3) / 2);
    imgHeight = (iDisplayHeight);

    lb_img ->setGeometry(QRect(QPoint(0, 0), QSize(imgWidth, imgHeight)));

    lb_text->setText("");
    lb_text->setStyleSheet("font-size: 25px");
    lb_text->setAlignment(Qt::AlignCenter);

    if (overlayChangeable)
    {
        lb_text->setGeometry(QRect(QPoint(imgWidth + 10, 0), QSize(iDisplayWidth - imgWidth - 20, 50)));
        btn[0]->disconnect();
        btn[0]->setText(QString::fromStdString(startPhoto));
        btn[0]->setStyleSheet("font-size: 20px");
        btn[0]->setGeometry(QRect(QPoint(imgWidth + 10, 50), QSize(iDisplayWidth - imgWidth - 20, ( iDisplayHeight - 100) / 2)));
        connect(btn[0], &QPushButton::released, this, &Gui::photoPrintButton);

        btn[1]->disconnect();
        btn[1]->setText(QString::fromStdString(grid));
        btn[1]->setStyleSheet("font-size: 20px");
        btn[1]->setGeometry(QRect(QPoint(imgWidth + 10, 50 + ( iDisplayHeight - 100) / 2), QSize(iDisplayWidth - imgWidth - 20, ( iDisplayHeight - 100) / 2)));
        connect(btn[1], &QPushButton::released, this, &Gui::changeOverlayButton);
    }
    else
    {
        lb_text->setGeometry(QRect(QPoint(imgWidth + 10, ( iDisplayHeight - 100) / 4), QSize(iDisplayWidth - imgWidth - 20, 50)));
        btn[0]->disconnect();
        btn[0]->setText(QString::fromStdString(startPhoto));
        btn[0]->setStyleSheet("font-size: 20px");
        btn[0]->setGeometry(QRect(QPoint(imgWidth + 10, 50 + ( iDisplayHeight - 100) / 4), QSize(iDisplayWidth - imgWidth - 20, ( iDisplayHeight - 100) / 2)));
        connect(btn[0], &QPushButton::released, this, &Gui::photoPrintButton);
    }

    updateGui->disconnect();
    connect(updateGui, &QTimer::timeout, this, &Gui::updateFullGui);
    updateGui->start();
}

void Gui::updateFullGui()
{
    if (imgProzess->getImageIsReady())
    {
        lb_img ->setPixmap(CvMatToQPixmap(imgProzess->getImage(), imgWidth, imgHeight));
    }

    lb_text->setText(QString::fromStdString(imgProzess->getCoutdown()));

    if (imgProzess->readyToTrigger() && printigIsEnabled && btn[0]->text() != QString::fromStdString(startPhoto))
    {
        btn[0]->setText(QString::fromStdString(startPhoto));
    }

    if (imgProzess->readyToPrint() && printigIsEnabled  && btn[0]->text() != QString::fromStdString(print))
    {
        btn[0]->setText(QString::fromStdString(print));
    }

    if (imgProzess->isPrinting() && printigIsEnabled  && btn[0]->text() != QString::fromStdString(isPrinting))
    {
        btn[0]->setText(QString::fromStdString(isPrinting));
    }
}

void Gui::updateImg()
{
    if (imgProzess->getImageIsReady())
    {
        lb_img ->setPixmap(CvMatToQPixmap(imgProzess->getImage(), imgWidth, imgHeight));
    }
}

void Gui::changeUsermodeButton()
{
    int State = imgProzess->getPiuctureMode();
    switch (State)
    {
    case 0 :
        if (imgProzess->getNumberOfLogo() > 0)
        {
            State = 1;
        }
        else if (imgProzess->getNumberOfOverlays() > 0)
        {
            State = 2;
        }
        break;

    case 1 :
        if (imgProzess->getNumberOfOverlays() > 0)
        {
            State = 2;
        }
        else
        {
            State = 0;
        }
        break;

    case 2 :
        if (!overlayChangeable)
        {
            overlayChangeable = true;
        }
        else
        {
            overlayChangeable = false;
            State = 0;
        }
        break;

    default:
        State = 0;
        break;
    }

    switch (imgProzess->setPiuctureMode(State))
    {
    case 0 :
        lb_text->setText(QString::fromStdString(userMode1));
        break;
    case 1 :
        lb_text->setText(QString::fromStdString(userMode2));
        break;
    case 2 :
        if (overlayChangeable)
        {
            lb_text->setText(QString::fromStdString(userMode4));
        }
        else
        {
            lb_text->setText(QString::fromStdString(userMode3));
        }
        break;
    default:
        lb_text->setText("error");
        break;
    }

}

void Gui::changeLogoButton()
{
    imgProzess->changeLogo();
}

void Gui::changeLogoEdgeButton()
{
    imgProzess->changeLogoEdge();
}

void Gui::changeLogoSizeButton(QString newSize)
{
    try
    {
        imgProzess->setLogoSize(std::stoi(newSize.toStdString()));
        Lines[0]->setText(QString::fromStdString(std::to_string(imgProzess->getLogoSize())));
    }
    catch (...)
    {
        Lines[0]->setText("invalid");
    }
}

void Gui::changeLogoPosiHorizontalButton(QString newDistance)
{
    try
    {Gui::
            imgProzess->setLogoDistanceHorizontal(std::stoi(newDistance.toStdString()));
        Lines[1]->setText(QString::fromStdString(std::to_string(imgProzess->getLogoDistanceHorizontal())));
    }
    catch (...)
    {
        Lines[1]->setText("invalid");
    }
}

void Gui::changeLogoPosiVerticalButton(QString newDistance)
{
    try
    {
        imgProzess->setLogoDistanceVertical(std::stoi(newDistance.toStdString()));
        Lines[2]->setText(QString::fromStdString(std::to_string(imgProzess->getLogoDistanceVertical())));
    }
    catch (...)
    {
        Lines[2]->setText("invalid");
    }
}

void Gui::changeOverlayButton()
{
    imgProzess->changeOverlay();
}

void Gui::changePrintingModeButton()
{
    printigIsEnabled = !printigIsEnabled;

    if (printigIsEnabled)
    {
        btn[7]->setText(QString::fromStdString(printingIsOn));
    }
    else
    {
        btn[7]->setText(QString::fromStdString(printingIsOff));
    }
}

void Gui::photoPrintButton()
{
    if (imgProzess->readyToTrigger())
    {
        imgProzess->triggerPhoto();
    }

    if (imgProzess->readyToPrint() && printigIsEnabled)
    {
        imgProzess->printPhoto();
        QImage printImg(QString::fromStdString(imgProzess->getPathFinalPhoto()));
        qDebug() << QString::fromStdString(imgProzess->getPathFinalPhoto());
        QPainter painter(myPrinter);
        painter.drawImage(myPrinter->pageRect(QPrinter::DevicePixel),printImg, printImg.rect());
        painter.end();
    }
}
