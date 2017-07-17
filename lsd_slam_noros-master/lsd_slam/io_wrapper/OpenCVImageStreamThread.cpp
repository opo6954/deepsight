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

#include "OpenCVImageStreamThread.h"
//#include <ros/callback_queue.h>

#include <boost/thread.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "cv_bridge/cv_bridge.h"
#include "util/settings.h"
 
#include <iostream>
#include <fstream>
#include <ctime>

namespace lsd_slam
{


	using namespace cv;

	OpenCVImageStreamThread::OpenCVImageStreamThread()
	{
		// wait for cam calib
		width_ = height_ = 0;


		// imagebuffer
		imageBuffer = new NotifyBuffer<TimestampedMat>(8);
		undistorter = 0;
		lastSEQ = 0;

		haveCalib = false;
		undistorter = 0;

		//기본을 카메라 모드로 설정
		cm = lsd_slam::LIVE_CAM_MODE;
	}

	OpenCVImageStreamThread::~OpenCVImageStreamThread()
	{
		delete imageBuffer;
	}

	void OpenCVImageStreamThread::setCalibration(std::string file)
	{
		if (file == "")
		{
			printf("NO camera calibration file!\n");
		}
		else
		{
			undistorter = Undistorter::getUndistorterForFile(file.c_str());

			if (!undistorter)
			{
				printf("Failed to read camera calibration from file... wrong syntax?\n");
				assert("Failed to read camera calibration from file... wrong syntax?");
				return;
			}

			fx_ = undistorter->getK().at<double>(0, 0);
			fy_ = undistorter->getK().at<double>(1, 1);
			cx_ = undistorter->getK().at<double>(2, 0);
			cy_ = undistorter->getK().at<double>(2, 1);

			width_ = undistorter->getOutputWidth();
			height_ = undistorter->getOutputHeight();
		}
		haveCalib = true;
	}


	void OpenCVImageStreamThread::setCameraMode(CAMERA_TEST_MODE _cm)
	{
		cm = _cm;
	}

	//camera info for sequence file
	void OpenCVImageStreamThread::setCameraInfo(std::string seqFileRootName, std::string seqFileMetaFileName, std::string seqTimeName)
	{
		//추후 구현...d
	}
	//camera info for webcam(device ID: 0)
	void OpenCVImageStreamThread::setCameraInfo(int camDevice)
	{
		if (haveCalib == false)
		{
			std::cout << "It needs calibration.." << std::endl;
			return;
		}

		
		vc.set(CV_CAP_PROP_FRAME_WIDTH, width());
		vc.set(CV_CAP_PROP_FRAME_HEIGHT, height());
		
		vc.open(camDevice);
	}
	//camera info for static video file
	void OpenCVImageStreamThread::setCameraInfo(std::string videoFileName)
	{
		vc.open(videoFileName);
	}


	void OpenCVImageStreamThread::run()
	{
		boost::thread thread(boost::ref(*this));
	}

	void OpenCVImageStreamThread::operator()()
	{
		//ros::spin();
		if (!haveCalib)
		{
			assert("no calibration");
			return;
		}
		if (!vc.isOpened())
		{
			assert("NO valid camera capture pointer");
			return;
		}
		//요 부분이 사실상 image stream이 돌아가는 부분일 듯...

		bool isLoopEnd = false;

		while (isLoopEnd == false)
		{
			//webcam으로 live에서 받을 시

			switch (cm)
			{
			case lsd_slam::LIVE_CAM_MODE:
			{	
				TimestampedMat bufferItem;

				bufferItem.timestamp = Timestamp::now();

				Mat frame;

				vc >> frame;
				
				

				if (frame.empty() == true)
				{
					isLoopEnd = true;
					break;
				}

				if (undistorter != 0)
				{
					assert(undistorter->isValid());
					undistorter->undistort(frame, bufferItem.data);
				}
				else
				{
					bufferItem.data = frame;
				}
				imageBuffer->pushBack(bufferItem);
				break;
			}

			case lsd_slam::STATIC_SEQUENCE_MODE:
				break;
			case lsd_slam::STATIC_VIDEO_MODE:
			{
				
				
				TimestampedMat bufferItem;

				bufferItem.timestamp = Timestamp::now();

				Mat frame;

				vc >> frame;



				if (frame.empty() == true)
				{
					isLoopEnd = true;
					break;
				}

				if (undistorter != 0)
				{
					assert(undistorter->isValid());
					undistorter->undistort(frame, bufferItem.data);
				}
				else
				{
					bufferItem.data = frame;
				}
				imageBuffer->pushBack(bufferItem);
				break;
			}
				break;
			default:
				break;
			}
		}
	}
}