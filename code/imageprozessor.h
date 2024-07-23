#ifndef IMAGEPROZESSOR_H
#define IMAGEPROZESSOR_H

#include "cameracontroler.h"
#include "metaoverlay.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgcodecs.hpp>

#include <filesystem>
#include <vector>

class ImageProzessor
{

public:
    ImageProzessor(std::string pFolderLogo, std::string pFolderOvlerlay, int iDisplayHeight);

    void overlayImage(cv::Mat* src, cv::Mat* overlay, const cv::Point& location);

    cv::Mat getImage();

    bool getImageIsReady();

    void triggerPhoto();
    void printPhoto();
    bool readyToTrigger();
    bool readyToPrint();
    bool isPrinting();
    std::string getCoutdown();
    std::string getPathFinalPhoto();

    //0 = only picture, 1 = with logo, 2 = with overlay not changeable, 3 = with overlay changeable
    int setPiuctureMode(int newMode);
    int getPiuctureMode();

    void changeOverlay();
    void setOverlay(int overlay);
    int getNumberOfOverlays();

    void changeLogo();
    void setLogo(int logo);
    int getNumberOfLogo();

    //moves logo clockwise from edge to edge
    int changeLogoEdge();
    int getLogoEdge();
    void setLogoEdge(int edge);

    //size of logo
    int setLogoSize(int newSize);
    int getLogoSize();

    //distance from boarder horizontal
    int setLogoDistanceHorizontal(int newDistance);
    int getLogoDistanceHorizontal();

    //distance from boarder vertical
    int setLogoDistanceVertical(int newDistance);
    int getLogoDistanceVertical();



private:
    CameraControler *cam;
    int iPiuctureMode = 0;
    int iLogoEdge = 0, iLogoDistanceHorizontal = 0, iLogoDistanceVertical = 0, iLogoSize = 10;
    int iLogoIndex = 0, iOverlayIndex = 0;
    std::vector <cv::Mat> logos;
    std::vector <cv::Mat> previewLogos;
    std::vector <cv::Mat> photoLogos;
    std::vector <MetaOverlay> overlays;
    std::vector <std::string> pFiles;
    cv::Mat background, backgroundPreview, helperImg[25];
    int iPvHeight = 0, iPvWidth = 0;
    int iSensorHeight = 0, iSensorWidth = 0;
    float previewFactor = 0.0, logoFactor;
    int iStateTakePhoto = 0, iCounterPhotos = 0;
    std::string pPhotos[10], pSavePhoto, finalPhoto, Countdown;
    std::time_t start_time;
    int fY, fX;
    bool isReadyForNextImg = TRUE;
};

#endif // IMAGEPROZESSOR_H
