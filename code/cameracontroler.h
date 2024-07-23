#ifndef CAMERACONTROLER_H
#define CAMERACONTROLER_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-file.h>
#include <gphoto2/gphoto2-context.h>


class CameraControler
{
public:
    CameraControler(int newDisplayHeight);

    cv::Mat GetPhoto(std::string imagePath);

    cv::Mat GetPreview();

private:
    Camera *camera;
    GPContext *context = gp_context_new();
    bool xFirstPreview = TRUE, xCameraConnectionOk;
    int colsPreview, rowsPreview;
    int displayHeight, displayWidth;
    char pathPreview[16] = "testPreview.jpg";
};

#endif // CAMERACONTROLER_H
