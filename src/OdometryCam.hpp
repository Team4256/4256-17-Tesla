#pragma once

#include <chrono>
#include <zed/Camera.hpp>

#include <opencv2/core/core.hpp>

#ifndef _SL_JETSON_
// OpenCV 3
#include <opencv2/core/utility.hpp>
#endif

#include "PoseHandler.hpp"

class OdometryCam
{
public:
	OdometryCam(const std::string& inputFileName);
	~OdometryCam();

	void printStatus();
	void run(PoseHandler& pose, const std::chrono::milliseconds& queryInterval);
	void init(const cv::CommandLineParser& parser);

	bool isWorking() const { return m_currentError == sl::zed::SUCCESS; };
	std::string errorString() const;

private:
	sl::zed::Camera* m_camera = nullptr;
	sl::zed::ERRCODE m_currentError = sl::zed::SUCCESS;

	const bool m_fromFile;
};
