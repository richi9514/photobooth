#include "imageprozessor.h"

ImageProzessor::ImageProzessor(std::string pFolderLogo, std::string pFolderOvlerlay, int iDisplayHeight)
{
    //get sensor and preview size
    cam = new CameraControler(iDisplayHeight);
    logoFactor = float(iLogoSize) / 100;
    helperImg[0] = cam->GetPreview();
    iPvWidth = helperImg[0].cols;
    iPvHeight = helperImg[0].rows;
    helperImg[0] = cam->GetPhoto("testPhoto.jpg");
    iSensorWidth = helperImg[0].cols;
    iSensorHeight = helperImg[0].rows;
    previewFactor = float(iPvWidth) / float(iSensorWidth);

    background = cv::imread("backgound.jpg", cv::IMREAD_UNCHANGED);

    cv::resize(background, backgroundPreview, cv::Size(iPvWidth, iPvHeight), cv::INTER_NEAREST);

    pSavePhoto = "";

    for (const auto & entry : std::filesystem::directory_iterator("/media"))
    {
        for (const auto & entry2 : std::filesystem::directory_iterator(entry.path()))
        {
            pSavePhoto = entry2.path();
            break;
        }
        break;
    }

    if (pSavePhoto != "")
    {
        pSavePhoto = pSavePhoto + "/Fotos";
        if ( !std::filesystem::exists(pSavePhoto))
        {
            std::filesystem::create_directory(pSavePhoto);
        }
    }
    else
    {
        pSavePhoto = "./Fotos";
        if ( !std::filesystem::exists(pSavePhoto))
        {
            std::filesystem::create_directory(pSavePhoto);
        }
    }

    if (iPvWidth != iSensorWidth)
    {

        //search for logos
        for (const auto & entry : std::filesystem::directory_iterator(pFolderLogo))
        {
            pFiles.push_back(entry.path());
        }

        int iLogoWidth, iLogoHeight;

        for (int i = 0; i < pFiles.size() ; i++ )
        {
            if (pFiles[i].find(".png") != 0)
            {
                logos.push_back(cv::imread(pFiles[i], -1));
                iLogoWidth = int(logos[i].cols * previewFactor * logoFactor);
                iLogoHeight = int(logos[i].rows * previewFactor * logoFactor);
                cv::resize(logos[i], helperImg[0], cv::Size(iLogoWidth, iLogoHeight), cv::INTER_LINEAR);
                previewLogos.push_back(helperImg[0]);
                iLogoWidth = int(logos[i].cols * logoFactor);
                iLogoHeight = int(logos[i].rows * logoFactor);
                cv::resize(logos[i], helperImg[0], cv::Size(iLogoWidth, iLogoHeight), cv::INTER_LINEAR);
                photoLogos.push_back(helperImg[0]);
            }
        }

        //clear founded files
        pFiles.clear();

        //search for overlays
        for (const auto & entry : std::filesystem::directory_iterator(pFolderOvlerlay))
        {
            pFiles.push_back(entry.path());
        }

        for (int i = 0; i < pFiles.size() ; i++ )
        {
            //get file type
            if (pFiles[i].find(".png") != 0)
            {
                for (int j = 0; j < pFiles.size() ; j++ )
                {
                    if (pFiles[j] == (pFiles[i].substr(0,pFiles[i].length() - 4)  + ".meta"))
                    {
                        overlays.push_back(MetaOverlay(pFiles[i], pFiles[j], previewFactor, iSensorWidth, iSensorHeight));
                    }
                }
            }
        }

    }

}

void ImageProzessor::overlayImage(cv::Mat* src, cv::Mat* overlay, const cv::Point& location)
{
    for (int y = cv::max(location.y, 0); y < src->rows; ++y)
    {
        fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = cv::max(location.x, 0); x < src->cols; ++x)
        {
            fX = x - location.x;

            if (fX >= overlay->cols)
                break;

            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}

bool ImageProzessor::getImageIsReady()
{
    return isReadyForNextImg;
}

cv::Mat ImageProzessor::getImage()
{
    isReadyForNextImg = FALSE;
    
    cv::Mat img;
    
    try
    {
        //do the matchig
        switch (iStateTakePhoto)
        {
        case 0 :
            img = cam->GetPreview();

            cv::flip(img,img,1);

            switch (iPiuctureMode)
            {
            case 0 :
                //usually not needed
                break;


            case 1 : //logo

                switch(iLogoEdge)
                {
                case 0:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     img.cols - previewLogos[iLogoIndex].cols - ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     img.rows - previewLogos[iLogoIndex].rows - ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 1:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     0 + ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     img.rows - previewLogos[iLogoIndex].rows - ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 2:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     0 + ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     0 + ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 3:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     img.cols - previewLogos[iLogoIndex].cols - ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     0 + ( img.rows * iLogoDistanceVertical ) / 100));
                    break;
                }
                break;

            case 2 : //overlay
                helperImg[10] = backgroundPreview;

                for(int i = 0; i < overlays[iOverlayIndex].getPhotosInOverlay(); i++)
                {
                    if (i > 1 && overlays[iOverlayIndex].getPreviewAlias(i) != -1)
                    {
                        helperImg[overlays[iOverlayIndex].getPreviewAlias(i)].copyTo(helperImg[i]);
                    }
                    else
                    {
                        cv::resize(img,
                                   helperImg[i],
                                   cv::Size(overlays[iOverlayIndex].getPreviewSizeX(i),
                                            overlays[iOverlayIndex].getPreviewSizeY(i)),
                                   cv::INTER_NEAREST);

                        try
                        {
                            if (overlays[iOverlayIndex].getPhotoRotate(i) != 0)
                            {
                                double angle = double(overlays[iOverlayIndex].getPhotoRotate(i));
                                cv::Point2f center((helperImg[i].cols - 1) / 2 , (helperImg[i].rows - 1) / 2);
                                cv::Mat rot =  cv::getRotationMatrix2D(center, angle, 1.0);

                                cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), helperImg[i].size(), angle).boundingRect2f();

                                rot.at<double>(0,2) += bbox.width / 2.0 - helperImg[i].cols / 2.0;
                                rot.at<double>(1,2) += bbox.height / 2.0 - helperImg[i].rows / 2.0;

                                cv::warpAffine(helperImg[i], helperImg[i], rot, bbox.size());

                            }
                        }
                        catch(...)
                        {
                            ;
                        }
                    }

                    try
                    {
                        helperImg[11] = cv::Mat( helperImg[10],
                                                cv::Rect(overlays[iOverlayIndex].getPreviewPosiX(i),
                                                         overlays[iOverlayIndex].getPreviewPosiY(i),
                                                         helperImg[i].cols,
                                                         helperImg[i].rows));
                        helperImg[i].copyTo(helperImg[11]);
                    }
                    catch(...)
                    {
                        ;
                    }
                }

                helperImg[12] = overlays[iOverlayIndex].getPreviewImg();

                overlayImage(&helperImg[10],&helperImg[12],cv::Point(0,0));

                img = helperImg[10];
                break;

            }

            iCounterPhotos = 0;

            break;

        case 1 :
            img = cam->GetPreview();

            cv::flip(img,img,1);

            switch (iPiuctureMode)
            {
            case 0 :
                //usually not needed
                break;


            case 1 : //logo

                switch(iLogoEdge)
                {
                case 0:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     img.cols - previewLogos[iLogoIndex].cols - ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     img.rows - previewLogos[iLogoIndex].rows - ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 1:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     0 + ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     img.rows - previewLogos[iLogoIndex].rows - ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 2:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     0 + ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     0 + ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 3:
                    overlayImage(&img,
                                 &previewLogos[iLogoIndex],
                                 cv::Point(
                                     img.cols - previewLogos[iLogoIndex].cols - ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     0 + ( img.rows * iLogoDistanceVertical ) / 100));
                    break;
                }
                break;

            case 2 : //overlay

                if (overlays[iOverlayIndex].getPhotosInOverlay() == 1)
                {
                    helperImg[10] = backgroundPreview;

                    for(int i = 0; i < overlays[iOverlayIndex].getPhotosInOverlay(); i++)
                    {
                        cv::resize(img,
                                   helperImg[i],
                                   cv::Size(overlays[iOverlayIndex].getPreviewSizeX(i),
                                            overlays[iOverlayIndex].getPreviewSizeY(i)),
                                   cv::INTER_NEAREST);

                        try
                        {
                            if (overlays[iOverlayIndex].getPhotoRotate(i) != 0)
                            {
                                double angle = double(overlays[iOverlayIndex].getPhotoRotate(i));
                                cv::Point2f center((helperImg[i].cols - 1) / 2 , (helperImg[i].rows - 1) / 2);
                                cv::Mat rot =  cv::getRotationMatrix2D(center, angle, 1.0);

                                cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), helperImg[i].size(), angle).boundingRect2f();

                                rot.at<double>(0,2) += bbox.width / 2.0 - helperImg[i].cols / 2.0;
                                rot.at<double>(1,2) += bbox.height / 2.0 - helperImg[i].rows / 2.0;

                                cv::warpAffine(helperImg[i], helperImg[i], rot, bbox.size());

                            }
                        }
                        catch(...)
                        {
                        }

                        try
                        {
                            helperImg[11] = cv::Mat( helperImg[10],
                                                    cv::Rect(overlays[iOverlayIndex].getPreviewPosiX(i),
                                                             overlays[iOverlayIndex].getPreviewPosiY(i),
                                                             helperImg[i].cols,
                                                             helperImg[i].rows));
                            helperImg[i].copyTo(helperImg[11]);
                        }
                        catch(...)
                        {
                            ;
                        }
                    }

                    helperImg[12] = overlays[iOverlayIndex].getPreviewImg();

                    overlayImage(&helperImg[10],&helperImg[12],cv::Point(0,0));

                    img = helperImg[10];
                }
                break;

            }

            if (time(0) >= start_time)
            {
                img = cv::imread("photo.jpg", cv::IMREAD_UNCHANGED);
                iStateTakePhoto = 2;
            }

            break;

        case 2 :
            
            iStateTakePhoto = 3;
            img = cv::imread("photo.jpg", cv::IMREAD_UNCHANGED);
            break;

        case 3 :

            pPhotos[iCounterPhotos] = pSavePhoto +
                                      "/img_" +
                                      std::to_string(std::localtime(&start_time)->tm_year + 1900) + "_" +
                                      std::to_string(std::localtime(&start_time)->tm_mon) + "_" +
                                      std::to_string(std::localtime(&start_time)->tm_mday) + "-" +
                                      std::to_string(std::localtime(&start_time)->tm_hour) + "_" +
                                      std::to_string(std::localtime(&start_time)->tm_min) + "_" +
                                      std::to_string(std::localtime(&start_time)->tm_sec) + ".jpg";

            cam->GetPhoto(pPhotos[iCounterPhotos]);

            iCounterPhotos++;

            img = cv::imread("processing.jpg", cv::IMREAD_UNCHANGED);

            iStateTakePhoto = 4;

            break;


        case 4 :

            if (iPiuctureMode != 2)
            {
                iStateTakePhoto = 5;
            }
            else
            {
                if (iCounterPhotos >= overlays[iOverlayIndex].getPhotosInOverlay())
                {
                    iStateTakePhoto = 5;
                }
                else
                {
                    iStateTakePhoto = 41;
                }

            }

            img = cv::imread("processing.jpg", cv::IMREAD_UNCHANGED);
            break;

        case 41:
        
            iStateTakePhoto++;
            img = cv::imread("processing.jpg", cv::IMREAD_UNCHANGED);
            break;
            
        case 42:
        
            iStateTakePhoto++;
            img = cv::imread("processing.jpg", cv::IMREAD_UNCHANGED);
            break;

        case 43:
        
            iStateTakePhoto = 1;
            start_time = time(0) + 10;
            img = cv::imread("processing.jpg", cv::IMREAD_UNCHANGED);
            break;
            
        case 5 :

            finalPhoto = pSavePhoto +
                         "/img_" +
                         std::to_string(std::localtime(&start_time)->tm_year + 1900) + "_" +
                         std::to_string(std::localtime(&start_time)->tm_mon) + "_" +
                         std::to_string(std::localtime(&start_time)->tm_mday) + "-" +
                         std::to_string(std::localtime(&start_time)->tm_hour) + "_" +
                         std::to_string(std::localtime(&start_time)->tm_min) + "_" +
                         std::to_string(std::localtime(&start_time)->tm_sec);

            switch (iPiuctureMode)
            {
            case 0 :

                img =  cv::imread(pPhotos[0], cv::IMREAD_UNCHANGED);
                cv::flip(img,img,1);
                finalPhoto = finalPhoto + "_fliped";
                break;


            case 1 : //logo


                img =  cv::imread(pPhotos[0], cv::IMREAD_UNCHANGED);
                cv::flip(img,img,1);

                switch(iLogoEdge)
                {
                case 0:
                    overlayImage(&img,
                                 &photoLogos[iLogoIndex],
                                 cv::Point(
                                     img.cols - photoLogos[iLogoIndex].cols - ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     img.rows - photoLogos[iLogoIndex].rows - ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 1:
                    overlayImage(&img,
                                 &photoLogos[iLogoIndex],
                                 cv::Point(
                                     0 + ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     img.rows - photoLogos[iLogoIndex].rows - ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 2:
                    overlayImage(&img,
                                 &photoLogos[iLogoIndex],
                                 cv::Point(
                                     0 + ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     0 + ( img.rows * iLogoDistanceVertical ) / 100));
                    break;

                case 3:
                    overlayImage(&img,
                                 &photoLogos[iLogoIndex],
                                 cv::Point(
                                     img.cols - photoLogos[iLogoIndex].cols - ( img.cols * iLogoDistanceHorizontal ) / 100,
                                     0 + ( img.rows * iLogoDistanceVertical ) / 100));
                    break;
                }

                finalPhoto = finalPhoto + "_logo";

                break;

            case 2 : //overlay
                cv::resize(background, helperImg[10], cv::Size(iSensorWidth, iSensorHeight), cv::INTER_NEAREST);


                for(int i = 0; i < overlays[iOverlayIndex].getPhotosInOverlay(); i++)
                {

                    img = cv::imread(pPhotos[i], cv::IMREAD_UNCHANGED);

                    cv::flip(img,img,1);

                    cv::resize(img,
                               helperImg[i],
                               cv::Size(overlays[iOverlayIndex].getPhotoSizeX(i),
                                        overlays[iOverlayIndex].getPhotoSizeY(i)),
                               cv::INTER_NEAREST);

                    try
                    {
                        if (overlays[iOverlayIndex].getPhotoRotate(i) != 0)
                        {
                            double angle = double(overlays[iOverlayIndex].getPhotoRotate(i));
                            cv::Point2f center((helperImg[i].cols - 1) / 2 , (helperImg[i].rows - 1) / 2);
                            cv::Mat rot =  cv::getRotationMatrix2D(center, angle, 1.0);

                            cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), helperImg[i].size(), angle).boundingRect2f();

                            rot.at<double>(0,2) += bbox.width / 2.0 - helperImg[i].cols / 2.0;
                            rot.at<double>(1,2) += bbox.height / 2.0 - helperImg[i].rows / 2.0;

                            cv::warpAffine(helperImg[i], helperImg[i], rot, bbox.size());

                        }
                    }
                    catch(...)
                    {
                        ;
                    }

                    try
                    {
                        helperImg[11] = cv::Mat( helperImg[10],
                                                cv::Rect(overlays[iOverlayIndex].getPhotoPosiX(i),
                                                         overlays[iOverlayIndex].getPhotoPosiY(i),
                                                         helperImg[i].cols,
                                                         helperImg[i].rows));
                        helperImg[i].copyTo(helperImg[11]);
                    }
                    catch(...)
                    {
                        ;
                    }
                }

                helperImg[12] = overlays[iOverlayIndex].getFullImg();

                overlayImage(&helperImg[10],&helperImg[12],cv::Point(0,0));

                img = helperImg[10];

                finalPhoto = finalPhoto + "_overlay";

                break;
            }

            finalPhoto = finalPhoto + ".jpg";

            cv::imwrite(finalPhoto, img);

            start_time = time(0) + 10;

            iStateTakePhoto = 6;

            break;

        case 6:

            img =  cv::imread(finalPhoto, cv::IMREAD_UNCHANGED);

            if (time(0) >= start_time)
            {
                iStateTakePhoto = 0;
            }

            break;

        case 7:

            img =  cv::imread(finalPhoto, cv::IMREAD_UNCHANGED);

            if (time(0) >= start_time)
            {
                iStateTakePhoto = 0;
            }

            break;

        default:
            img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
        }
    }
    catch(...)
    {
        cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
    }
    
    isReadyForNextImg = TRUE;

    return img;
}

void ImageProzessor::triggerPhoto()
{
    if (iStateTakePhoto == 0)
    {
        start_time = time(0) + 5;
        iStateTakePhoto = 1;
    }
}


void ImageProzessor::printPhoto()
{
    if (iStateTakePhoto == 6)
    {
        //std::string command;
        //command = "lpr " + finalPhoto;
        //system(command.c_str());
        iStateTakePhoto++;
    }
}

std::string ImageProzessor::getPathFinalPhoto()
{
    return finalPhoto;
}


bool ImageProzessor::readyToTrigger()
{
    if (iStateTakePhoto == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ImageProzessor::readyToPrint()
{
    if (iStateTakePhoto == 6)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ImageProzessor::isPrinting()
{
    if (iStateTakePhoto == 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string ImageProzessor::getCoutdown()
{
    if ( iStateTakePhoto >= 2 && iStateTakePhoto <= 5)
    {
        Countdown = "GO";
    }
    else if ( iStateTakePhoto == 1 || iStateTakePhoto > 5)
    {
        int displayTime = int(start_time) - int(time(0));
        
        if (displayTime >= 0 )
        {
            Countdown = std::to_string(displayTime);
        }
        else
        {
            Countdown = "GO";
        }
    }
    else
    {
        Countdown = "";
    }

    return Countdown;
}

//0 = only picture, 1 = with logo, 2 = with overlay
int ImageProzessor::setPiuctureMode(int newMode)
{
    if (newMode >= 0 && newMode < 3)
    {
        iPiuctureMode = newMode;
    }
    return iPiuctureMode;
}

int ImageProzessor::getPiuctureMode()
{
    return iPiuctureMode;
}

//0 = 0 overlay, 1 = 1 overlay.......
void ImageProzessor::changeOverlay()
{
    if (iOverlayIndex < overlays.size() - 1 && readyToTrigger())
    {
        iOverlayIndex++;
    }
    else
    {
        iOverlayIndex = 0;
    }
}

int ImageProzessor::getNumberOfOverlays()
{
    return overlays.size();
}

void ImageProzessor::setOverlay(int overlay)
{
    if (overlay > overlays.size() - 1)
    {
        iOverlayIndex = 0;
    }
    else
    {
        iOverlayIndex = overlay;
    }
}

void  ImageProzessor::changeLogo()
{
    if (iLogoIndex < logos.size() - 1)
    {
        iLogoIndex++;
    }
    else
    {
        iLogoIndex = 0;
    }
}

int ImageProzessor::getNumberOfLogo()
{
    return logos.size();
}


void ImageProzessor::setLogo(int logo)
{
    if (logo > logos.size() - 1)
    {
        iLogoIndex = 0;
    }
    else
    {
        iLogoIndex = logo;
    }
}

//moves logo clockwise from edge to edge
//0 down right, 1 down left, 2 up left, 3 up right
int ImageProzessor::changeLogoEdge()
{
    switch (iLogoEdge)
    {
    case 0 :
        iLogoEdge = 1;
        break;

    case 1 :
        iLogoEdge = 2;
        break;

    case 2 :
        iLogoEdge = 3;
        break;

    case 3 :
        iLogoEdge = 0;
        break;

    default:
        iLogoEdge = 0;
        break;
    }

    return iLogoEdge;
}

void ImageProzessor::setLogoEdge(int edge)
{
    if (edge >= 0 && edge <= 3 )
    {
        iLogoEdge = edge;
    }
}   

int ImageProzessor::getLogoEdge()
{
    return iLogoEdge;
}

//size of logo
int ImageProzessor::setLogoSize(int newSize)
{
    if (newSize > 0)
    {
        iLogoSize = newSize;

        if (logos.size() > 0)
        {
            int iLogoWidth;
            int iLogoHeight;

            logoFactor = float(iLogoSize) / 100;

            previewLogos.clear();
            photoLogos.clear();

            for(int i = 0; i < logos.size(); i++)
            {
                iLogoWidth = int(logos[i].cols * previewFactor * logoFactor);
                iLogoHeight = int(logos[i].rows * previewFactor * logoFactor);
                cv::resize(logos[i], helperImg[0], cv::Size(iLogoWidth, iLogoHeight), cv::INTER_LINEAR);
                previewLogos.push_back(helperImg[0]);
                iLogoWidth = int(logos[i].cols * logoFactor);
                iLogoHeight = int(logos[i].rows * logoFactor);
                cv::resize(logos[i], helperImg[0], cv::Size(iLogoWidth, iLogoHeight), cv::INTER_LINEAR);
                photoLogos.push_back(helperImg[0]);
            }
        }

    }
    return iLogoSize;
}

int ImageProzessor::getLogoSize()
{
    return iLogoSize;
}

//distance from boarder horizontal
int ImageProzessor::setLogoDistanceHorizontal(int newDistance)
{
    if (newDistance >= 0 && newDistance <= 100)
    {
        iLogoDistanceHorizontal = newDistance;
    }
    return iLogoDistanceHorizontal;
}

int ImageProzessor::getLogoDistanceHorizontal()
{
    return iLogoDistanceHorizontal;
}

//distance from boarder vertical
int ImageProzessor::setLogoDistanceVertical(int newDistance)
{
    if (newDistance >= 0 && newDistance <= 100)
    {
        iLogoDistanceVertical = newDistance;
    }
    return iLogoDistanceVertical;
}

int ImageProzessor::getLogoDistanceVertical()
{
    return iLogoDistanceVertical;
}
