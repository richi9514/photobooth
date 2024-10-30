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
    
	std::this_thread::sleep_for(std::chrono::milliseconds (1000));	
}


bool CameraControler::CameraInError()
{
	return xCameraInError;
}


void helper(std::string imagePath, Camera *camera)
{
	try
    {
		gp_camera_new(&camera);
		
		CameraFile *file;
		
		CameraFilePath camera_file_path;
		
		GPContext *context = gp_context_new();
		
		gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);

		      gp_file_new(&file);

        gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
                           GP_FILE_TYPE_NORMAL, file, context);

        gp_file_save(file, imagePath.c_str());

        gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
                              context);

        gp_file_free(file);
        
		gp_camera_exit(camera, context);
	}
    catch(...)
    {
    }
}

cv::Mat CameraControler::GetPhoto(std::string imagePath)
{
    try
    {
		if (!xCameraInError)
		
		{
			gp_camera_exit(camera, context);
			
			std::thread take_photo(helper,imagePath, camera);
			
			int cycles = 0;
			
			bool photoActive = true;
			bool photoDone = false;
			
			take_photo.detach();
			
			while(photoActive)
			{
				cycles++;
				
				if (!std::filesystem::exists(imagePath) && (cycles > 30))
				{
					photoActive = false;
					photoDone = false;
					take_photo.~thread();
				}
				
				if (std::filesystem::exists(imagePath) && !take_photo.joinable() && photoActive)
				{
					photoActive = false;
					photoDone = true;
				}
				
				std::this_thread::sleep_for(std::chrono::milliseconds (100));
			}	
			
			gp_camera_new(&camera);
			
			if (photoDone)
			{	
				
				cv::Mat img;
				try
				{	
					img = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
				}
				catch(...)
				{
					img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
					cv::flip(img,img,1);
					xCameraInError = TRUE;
					cv::imwrite(imagePath, img);
				}
				return img;
			}
			else
			{
				gp_camera_new(&camera);
				
				cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
				cv::flip(img,img,1);
				xCameraInError = TRUE;
				cv::imwrite(imagePath, img);
				
				return img;
			}
		}
		else
		{
			gp_camera_new(&camera);
			
	        cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
			cv::flip(img,img,1);
			xCameraInError = TRUE;
	        cv::imwrite(imagePath, img);
	        
	        return img;
		}

    }
    catch(...)
    {
		gp_camera_new(&camera);
				
		cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
		cv::flip(img,img,1);
		xCameraInError = TRUE;
		cv::imwrite(imagePath, img);
		return img;
    }
    
    if (xCameraInError)
    {
		std::this_thread::sleep_for(std::chrono::milliseconds (1000));
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
        
		try
		{
			std::filesystem::remove("testPreview.jpg");
		}
		catch(...)
		{}
		

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
        else if (xCameraInError)
        {
			cv::Mat img = cv::imread("error.jpg", cv::IMREAD_UNCHANGED);
			cv::flip(img,img,1);
			xCameraInError = TRUE;
			return img;
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
        xCameraInError = TRUE;
        return img;
    }
    
    if (xCameraInError)
    {
		std::this_thread::sleep_for(std::chrono::milliseconds (5000));
	}
}
