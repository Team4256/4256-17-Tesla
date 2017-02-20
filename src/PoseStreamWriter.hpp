#pragma once

#include <iostream>
#include "PoseHandler.hpp"

class PoseStreamWriter : public PoseHandler
{
public:
	PoseStreamWriter(std::ostream& outputStream) : m_outputStream(outputStream) { };
	virtual ~PoseStreamWriter() { };

	virtual void setPosition(const sl::zed::MotionPoseData& poseData, unsigned long long currentTimeStamp);

private:
	std::ostream& m_outputStream;
};
