#pragma once

#include <zed/Camera.hpp>

class PoseHandler
{
public:
	virtual ~PoseHandler() {};

	virtual void setPosition(const sl::zed::MotionPoseData& poseData, unsigned long long currentTimeStamp) = 0;
};
