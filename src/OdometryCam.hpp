#pragma once

#include <zed/Camera.hpp>
#include <opencv2/core/utility.hpp>

#include "PoseHandler.hpp"

class cv::CommandLineParser;

class OdometryCam
{
public:
	OdometryCam(const std::string& inputFileName);
	~OdometryCam();

	void printStatus();
	void run(PoseHandler& pose);
	void init(const cv::CommandLineParser& parser);

	bool isWorking() const { return m_currentError == sl::zed::SUCCESS; };
	std::string errorString() const;

private:
	sl::zed::Camera* m_camera = nullptr;
	sl::zed::ERRCODE m_currentError = sl::zed::SUCCESS;

	const bool m_fromFile;
};
