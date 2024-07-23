#include "metaoverlay.h"

MetaOverlay::MetaOverlay(std::string pImg, std::string pMeta, float previewFactor, int width, int height)
{
    fullImg = cv::imread(pImg, cv::IMREAD_UNCHANGED);
    float imgOriginCols = float(fullImg.cols);
    cv::resize(fullImg, fullImg, cv::Size(width, height), cv::INTER_NEAREST);
    cv::resize(fullImg, previewImg, cv::Size(int(float(fullImg.cols) * previewFactor), int(float(fullImg.rows) * previewFactor)), cv::INTER_NEAREST);

    float imgFactor = float(fullImg.cols) / imgOriginCols;

    std::ifstream metaData;
    metaData.open(pMeta);

    std::string newLine;

    photosInOverlay = 0;

    for (int i = 0; i < 10; i++)
    {
        photoSizeX[i] = 0;
        photoSizeY[i] = 0;
        photoPosiX[i] = 0;
        photoPosiY[i] = 0;
        photoRotate[i] = 0;
        previewAlias[i] = -1;
    }

    if (metaData.is_open())
    {
        while(std::getline(metaData,newLine))
        {
            if (newLine.rfind("count") == 0)
            {
                photosInOverlay = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }

            if (newLine.rfind("photow") == 0)
            {
                int index = stoi(newLine.substr(6,7));
                photoSizeX[index] = int(stof(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1)) * imgFactor);
                previewSizeX[index] = int(float(photoSizeX[index]) * previewFactor);
            }

            if (newLine.rfind("photoh") == 0)
            {
                int index = stoi(newLine.substr(6,7));
                photoSizeY[index] = int(stof(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1)) * imgFactor);
                previewSizeY[index] = int(float(photoSizeY[index]) * previewFactor);
            }

            if (newLine.rfind("photor") == 0)
            {
                int index = stoi(newLine.substr(6,7));
                photoRotate[index] = stoi(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1));
            }

            if (newLine.rfind("photox") == 0)
            {
                int index = stoi(newLine.substr(6,7));
                photoPosiX[index] = int(stof(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1)) * imgFactor);
                previewPosiX[index] = int(float(photoPosiX[index]) * previewFactor);
            }

            if (newLine.rfind("photoy") == 0)
            {
                int index = stoi(newLine.substr(6,7));
                photoPosiY[index] = int(stof(newLine.substr(newLine.find_first_of(char(34)) + 1 , newLine.find_last_of(char(34)) - newLine.find_first_of(char(34)) - 1)) * imgFactor);
                previewPosiY[index] = int(float(photoPosiY[index]) * previewFactor);
            }
        }
    }

    if (photosInOverlay > 1)
    {
        for (int i = 1; i < photosInOverlay; i++)
        {
            for (int j = photosInOverlay; j >= 0; j--)
            {
                if (photoSizeX[i] == photoSizeX[j]
                    && photoSizeY[i] == photoSizeY[j]
                    && photoRotate[i] == photoRotate[j]
                    && j < i)
                {
                    previewAlias[i] = j;
                    break;
                }
            }
        }
    }

}

cv::Mat MetaOverlay::getFullImg()
{
    return fullImg;
}

cv::Mat MetaOverlay::getPreviewImg()
{
    return previewImg;
}

int MetaOverlay::getPhotosInOverlay()
{
    return photosInOverlay;
}

int MetaOverlay::getPhotoSizeX(int index)
{
    return photoSizeX[index];
}

int MetaOverlay::getPhotoSizeY(int index)
{
    return photoSizeY[index];
}

int MetaOverlay::getPhotoPosiX(int index)
{
    return photoPosiX[index];
}

int MetaOverlay::getPhotoPosiY(int index)
{
    return photoPosiY[index];
}

int MetaOverlay::getPhotoRotate(int index)
{
    return photoRotate[index];
}

int MetaOverlay::getPreviewSizeX(int index)
{
    return previewSizeX[index];
}

int MetaOverlay::getPreviewSizeY(int index)
{
    return previewSizeY[index];
}

int MetaOverlay::getPreviewPosiX(int index)
{
    return previewPosiX[index];
}

int MetaOverlay::getPreviewPosiY(int index)
{
    return previewPosiY[index];
}

 int MetaOverlay::getPreviewAlias(int index)
{
    return previewAlias[index];
 }
