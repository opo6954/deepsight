
//Test for PCL
//For testing lsd_slam_noros, annotate below line
//#define TEST

#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "live_slam_wrapper.h"
#include "util/settings.h"
#include "util/global_funcs.h"

#include "util/undistorter.h"
#include "io_wrapper/OpenCVImageStreamThread.h"
#include "slam_system.h"
#include "DebugOutput3DWrapper.h"
#include "pclVisualizer.h"


//#define LIVE_SLAM
//#define STATIC_VIDEO_SLAM
#define PCL_TEST


using namespace std;
using namespace lsd_slam;
using namespace cv;
char key;

std::string staticVideoPath = "../../../data/short_test_video_data/video.avi";
std::string staticCalibPath = "../../../data/short_test_video_data/out_camera_data.xml";

std::string liveCalibPath = "../../../data/out_camera_data.xml";



int main(int argc, char** argv) {
		
#ifndef PCL_TEST
	if (argc < 2) {
		printf(
			"Usage: sample_app <camera id>\ncamera id is 0 /dev/video0, 1 for /dev/video1 etc.\n");
		exit(1);
	}

	int cameraId = atoi(argv[1]);
	

	cvNamedWindow("Camera_Output_Undist", 1); //Create window
#endif

#ifdef LIVE_SLAM

	//for Capture;
	
	OpenCVImageStreamThread* inputStream = new OpenCVImageStreamThread();

	inputStream->setCalibration(liveCalibPath.c_str());
	inputStream->setCameraInfo(0);	
	inputStream->setCameraMode(lsd_slam::LIVE_CAM_MODE);

	inputStream->run();
#endif

	//for from static video
	
#ifdef STATIC_VIDEO_SLAM


	OpenCVImageStreamThread* inputStream = new OpenCVImageStreamThread();

	inputStream->setCalibration(staticCalibPath.c_str());
	inputStream->setCameraMode(lsd_slam::STATIC_VIDEO_MODE);
	inputStream->setCameraInfo(staticVideoPath);
	
	
	inputStream->run();
#endif

#ifdef PCL_TEST
	processor('s');
	

#else
	Output3DWrapper* outputWrapper = new DebugOutput3DWrapper(
		inputStream->width(), inputStream->height());
	//LiveSLAMWrapper로 input stream하고 outputWrapper(아마 출력을 관리하는 부분일 듯...)
	LiveSLAMWrapper slamNode(inputStream, outputWrapper);


	slamNode.Loop();

	/*
	if (frame != nullptr)
	{
	printf("wh(%d, %d)\n", frame->width, frame->height);
	cv::Mat mymat = cv::Mat(frame, true);
	cv::Mat tracker_display = cv::Mat::ones(frame->width, frame->height, CV_8UC3);
	cv::circle(mymat, cv::Point(100, 100), 20, cv::Scalar(255, 1, 0), 5);
	cv::imshow("Camera_Output_Undist", mymat);

	slamNode.Loop();
	}
	*/
	//Undistorter* undistorter = Undistorter::getUndistorterForFile("out_camera_data.xml");

	//while (1){ //Create infinte loop for live streaming
	//	IplImage* frame = cvQueryFrame(capture); //Create image frames from capture
	//	TimestampedMat bufferItem;
	//	bufferItem.timestamp = Timestamp::now();
	//	
	//	cv::Mat mymat = cv::Mat(frame, true);

	//	
	//	undistorter->undistort(frame, mymat);
	//    
	//	cvShowImage("Camera_Output", frame); //Show image frames on created window
	//	cv::imshow("Camera_Output_Undist", mymat);
	//	key = cvWaitKey(10); //Capture Keyboard stroke
	//	if (char(key) == 27){
	//		break; //If you hit ESC key loop will break.
	//	}
	//}

	if (inputStream != nullptr)
		delete inputStream;
	if (outputWrapper != nullptr)
		delete outputWrapper;

	//cvDestroyWindow("Camera_Output"); //Destroy Window
	return 0;
#endif

	


}

