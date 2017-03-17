#include "PoseStreamWriter.hpp"
#include "math.h"

void PoseStreamWriter::setPosition(const sl::zed::MotionPoseData& poseData, unsigned long long currentTimeStamp)
{
	switch (poseData.frame_status)
	{
		case sl::zed::TRACKING_GOOD:
			// do nothing...continue through the function
			break;

		case sl::zed::TRACKING_LOST:
			m_outputStream << "Tracking lost\n";
			return;

		case sl::zed::TRACKING_RELOC:
			m_outputStream << "Trying to relocate\n";
			return;

		case sl::zed::TRACKING_INIT:
			m_outputStream << "Initializing location\n";
			return;

		default:
			m_outputStream << "Unexpected error\n";
			return;
	}

	// x tab y tab tab confidence
	m_outputStream << round(poseData.translation[0]*100)/100 << "\t" << round(poseData.translation[2]*100)/100;
	m_outputStream << "\t\t" << poseData.pose_confidence;

	m_outputStream << std::endl;
}
