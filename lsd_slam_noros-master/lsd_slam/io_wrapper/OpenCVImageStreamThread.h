/**
* This file is part of LSD-SLAM.
*
* Copyright 2013 Jakob Engel <engelj at in dot tum dot de> (Technical University of Munich)
* For more information see <http://vision.in.tum.de/lsdslam> 
*
* LSD-SLAM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LSD-SLAM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with LSD-SLAM. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "io_wrapper/notify_buffer.h"
#include "io_wrapper/timestamped_object.h"
#include "io_wrapper/input_image_stream.h"


//#include <sensor_msgs/image_encodings.h>
//#include <sensor_msgs/Image.h>
//#include <sensor_msgs/CameraInfo.h>
//#include <geometry_msgs/PoseStamped.h>

#include "util/undistorter.h"
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


namespace lsd_slam
{


	 
/**
 * Image stream provider using ROS messages.
 */

	enum CAMERA_TEST_MODE
	{
		LIVE_CAM_MODE, STATIC_SEQUENCE_MODE, STATIC_VIDEO_MODE
	};

class OpenCVImageStreamThread : public InputImageStream
{
public: 
	OpenCVImageStreamThread();
	~OpenCVImageStreamThread();
	
	/**
	 * Starts the thread.
	 */
	void run();
	
	void setCalibration(std::string file);
	
	//어느 모드로 할 지 이 부분에서 설정
	void setCameraMode(CAMERA_TEST_MODE _cm);



	void setCameraInfo(std::string seqFileRootName, std::string seqFileMetaFileName, std::string seqTimeName);
	void setCameraInfo(int camDeviceNum);
	void setCameraInfo(std::string videoFileName);





	/**
	 * Thread main function.
	 */
	void operator()();
	
	// get called on ros-message callbacks
	//void vidCb(const sensor_msgs::ImageConstPtr img);
	//void infoCb(const sensor_msgs::CameraInfoConstPtr info);

private:

	CAMERA_TEST_MODE cm;
	bool haveCalib;
	Undistorter* undistorter;
	VideoCapture vc;

	std::vector<Mat> imgSeq;
	std::vector<double> timeStamp;

	int lastSEQ;
};

}
