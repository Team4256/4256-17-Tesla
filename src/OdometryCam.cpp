#include <chrono>
#include <string>
#include <thread>

#include "OdometryCam.hpp"
#include "PoseHandler.hpp"

OdometryCam::OdometryCam(const std::string& inputFileName)
	: m_fromFile(!inputFileName.empty())
{
	if (inputFileName.empty())
		m_camera = new sl::zed::Camera(sl::zed::ZEDResolution_mode::HD720, 0.0);
	else
		m_camera = new sl::zed::Camera(inputFileName);
}

OdometryCam::~OdometryCam()
{
	delete m_camera;
}

void OdometryCam::run(PoseHandler& poseHandler, const std::chrono::milliseconds& queryInterval)
{
	sl::zed::MotionPoseData poseData;
	unsigned long long currentTimeStamp;
	auto startTime = std::chrono::high_resolution_clock::now();

	std::cout << "Start run\n";
	m_currentError = m_camera->grab(sl::zed::SENSING_MODE::FILL, true, true, false);//FILL vs STANDARD
	while (m_currentError == sl::zed::SUCCESS)
	{
		// Default arguments, except don't compute the point cloud
		m_camera->getPosition(poseData, sl::zed::MAT_TRACKING_TYPE::PATH);
		currentTimeStamp = m_fromFile ? poseData.timestamp : m_camera->getCurrentTimestamp();
		poseHandler.setPosition(poseData, currentTimeStamp);

		auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		if (elapsedTime < queryInterval)
			std::this_thread::sleep_for(queryInterval - elapsedTime);
		startTime = std::chrono::high_resolution_clock::now();
		
		m_currentError = m_camera->grab(sl::zed::SENSING_MODE::FILL, true, true, false);//FILL vs STANDARD
	}
}

void OdometryCam::printStatus()
{
	std::cout << "SDK version: " << sl::zed::Camera::getSDKVersion() << std::endl;
	std::cout << "Image size: " << m_camera->getImageSize().width << "x" << m_camera->getImageSize().height << std::endl;
	std::cout << "Frame rate: " << m_camera->getCurrentFPS() << std::endl;

	if (m_fromFile)
		std::cout << "Total number of frames: " << m_camera->getSVONumberOfFrames();
}

void OdometryCam::init(const cv::CommandLineParser& parser)
{
	sl::zed::InitParams params(sl::zed::PERFORMANCE, sl::zed::METER);
	params.minimumDistance = 0.5; // minimum distance of 0.5m.  Smaller values requires more computation

	std::cout << "Initializing camera...\n";
	m_currentError = m_camera->init(params);

	if (!isWorking())
		return;

	std::cout << "Calibrating...\n";
	while (m_camera->getSelfCalibrationStatus() == sl::zed::ZED_SELF_CALIBRATION_STATUS::SELF_CALIBRATION_RUNNING)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	m_camera->setDepthClampValue(10.); // 10 meters

	// Position initialized at 0
	Eigen::Matrix4f ident = Eigen::Matrix4f::Identity();
	m_camera->enableTracking(ident);
}

std::string OdometryCam::errorString() const
{
	switch(m_currentError)
	{
		case sl::zed::SUCCESS:
			return "No error";
		case sl::zed::NO_GPU_COMPATIBLE:
			return "No compatible GPU found";
		case sl::zed::NOT_ENOUGH_GPUMEM:
			return "Not enough GPU memory to perform this depth calculation";
		case sl::zed::ZED_NOT_AVAILABLE:
			return "ZED not plugged in";
		case sl::zed::ZED_RESOLUTION_INVALID:
			return "Resolution not supported";
		case sl::zed::ZED_SETTINGS_FILE_NOT_AVAILABLE:
			return "ZED settings file not found; use ZED Settings App Tool to correct";
		case sl::zed::INVALID_SVO_FILE:
			return "Invalid SVO file";
		case sl::zed::RECORDER_ERROR:
			return "A recorder related error occurred";
		case sl::zed::INVALID_COORDINATE_SYSTEM:
			return "The requested coordinate system is not available";
		case sl::zed::ZED_WRONG_FIRMWARE:
			return "Out of date firmware";
		case sl::zed::NO_NEW_FRAME:
			return "Finished processing all frames";
		case sl::zed::CUDA_ERROR_THROWN:
			return "CUDA_ERROR_THROWN";
		case sl::zed::ZED_NOT_INITIALIZED:
			return "The ZED was not initialized";
		case sl::zed::OUT_OF_DATE_NVIDIA_DRIVER:
			return "The Nvidia driver is out of date";
		case sl::zed::INVALID_FUNCTION_CALL:
			return "INVALID_FUNCTION_CALL";
		default:
			return "This should never happen";
	}
}
