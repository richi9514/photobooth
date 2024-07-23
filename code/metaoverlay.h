#ifndef METAOVERLAY_H
#define METAOVERLAY_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fstream>

class MetaOverlay
{
public:
    MetaOverlay(std::string pImg, std::string pMeta, float previewFactor, int Width, int height);

    cv::Mat getFullImg();
    cv::Mat getPreviewImg();

    int getPhotosInOverlay();
    int getPhotoSizeX(int index);
    int getPhotoSizeY(int index);
    int getPhotoPosiX(int index);
    int getPhotoPosiY(int index);
    int getPhotoRotate(int index);
    int getPreviewSizeX(int index);
    int getPreviewSizeY(int index);
    int getPreviewPosiX(int index);
    int getPreviewPosiY(int index);
    int getPreviewAlias(int index);



private:
    cv::Mat fullImg, previewImg;
    int photosInOverlay, photoSizeX[10], photoSizeY[10], photoPosiX[10], photoPosiY[10], photoRotate[10];
    int previewSizeX[10], previewSizeY[10], previewPosiX[10], previewPosiY[10], previewAlias[10];
};

#endif // METAOVERLAY_H
