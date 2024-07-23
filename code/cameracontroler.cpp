#include "cameracontroler.h"

CameraControler::CameraControler(int newDisplayHeight)
{
    try
    {
        gp_camera_new(&camera);
        displayHeight = newDisplayHeight;
    }
    catch(...)
    {
    }
}

cv::Mat CameraControler::GetPhoto(std::string imagePath)
{
    try
    {
        gp_camera_exit(camera, context);
        gp_camera_new(&camera);
        CameraFile *file;
        CameraFilePath camera_file_path;
        CameraEventType event;
        void *event_data; 
        
        gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
        gp_camera_wait_for_event(camera, 5000, &event, &event_data, context );

        if ( event != GP_EVENT_CAPTURE_COMPLETE )
        {
            cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
            cv::flip(img,img,1);
                
            cv::imwrite(imagePath.c_str(), img);
            
            return img;
        }
        
        gp_file_new(&file);

        gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
                           GP_FILE_TYPE_NORMAL, file, context);

        gp_file_save(file, imagePath.c_str());


        gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
                              context);

        gp_file_free(file);

        return cv::imread(imagePath, cv::IMREAD_UNCHANGED);

    }
    catch(...)
    {
        cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
        cv::flip(img,img,1);
        return img;
    }
}

cv::Mat CameraControler::GetPreview()
{
    try
    {
        CameraFile *file;
        const char *buf;
        unsigned long *size;
        size = 0;

        if (xFirstPreview)
        {
            //capture preview to file and delete after
            //detect size of preview

            gp_file_new(&file);

            gp_camera_capture_preview(camera, file, context);

            gp_file_save(file, pathPreview);

            gp_file_free(file);

            cv::Mat preview = cv::imread(pathPreview, cv::IMREAD_UNCHANGED);

            colsPreview = preview.cols;
            
            rowsPreview = preview.rows;

            displayWidth = displayHeight * colsPreview / rowsPreview;

            xFirstPreview = FALSE;
            
            cv::resize(preview, preview, cv::Size(displayWidth, displayHeight), cv::INTER_NEAREST);
            
            return preview;
        }
        else
        {
            //capture to memory

            gp_file_new(&file);

            gp_camera_capture_preview(camera, file, context);

            gp_file_get_data_and_size (file, &buf, size);

            std::vector<unsigned char> imgData(buf, buf + (colsPreview * rowsPreview));

            cv::Mat Data = cv::Mat(imgData);

            cv::Mat preview = cv::imdecode(Data,cv::IMREAD_UNCHANGED);

            gp_file_free(file);

            imgData.clear();
            
            cv::resize(preview, preview, cv::Size(displayWidth, displayHeight), cv::INTER_NEAREST);
            
            return preview;
        }
    }
    catch(...)
    {

        cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
        cv::flip(img,img,1);
        return img;
    }
}
