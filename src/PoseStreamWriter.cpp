#include "PoseStreamWriter.hpp"

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
			m_outputStream << "Initializing relocation\n";
			return;

		default:
			m_outputStream << "Unexpected error\n";
			return;
	}

	// x tab y 
	m_outputStream << poseData.translation[0] << "\t" << poseData.translation[1];
	m_outputStream << "\t" << poseData.pose_confidence;

	m_outputStream << std::endl;
}
