#include "OdometryCam.hpp"
#include "PoseStreamWriter.hpp"

int main(int argc, char **argv)
{
	const cv::String keys =
#ifdef _SSL_JETSON_
	// This is the OpenCV 2.x case, for the TK1 board
		"{h | help        | | print this message}"
		"{p | printstatus | | print the status of the camera or svo file}"
		"{i | input       | | SVO filename (ex : -i=test.svo  or --input=test.svo) }"
		"{o | output      | | filename for pose data (ex : -o=data.txt  or --output=data.txt) }";
#else
	// This is the OpenCV 3.x case, which is what the Windows SDK uses
		"{help h usage ? || print this message}"
		"{printstatus p || print the status of the camera or svo file}"
		"{input i || SVO filename (ex : -i=test.svo  or --input=test.svo) }"
		"{output o || filename for pose data (ex : -o=data.txt  or --output=data.txt) }";
#endif

#ifdef _SSL_JETSON_
	cv::CommandLineParser parser(argc, argv, keys.c_str());
#else
	cv::CommandLineParser parser(argc, argv, keys);
#endif

	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	std::string inputFile = parser.get<std::string>("input");
	if (inputFile == "true")
	{
		std::cout << "Invalid syntax, expected \"-i=file\"\n";
		return 0;
	}

	std::string outputFile = parser.get<std::string>("output");
	if (outputFile == "true")
	{
		std::cout << "Invalid syntax, expected \"-o=file\"\n";
		return 0;
	}
	std::ostream* outputStream;
	if (outputFile.empty())
		outputStream = new std::ostream(std::cout.rdbuf());
	else
		outputStream = new std::ofstream(outputFile);

	OdometryCam camera(parser.get<std::string>("input"));

	if (parser.has("printstatus"))
	{
		camera.printStatus();
		delete outputStream;
		return 0;
	}

	PoseStreamWriter streamWriter(*outputStream);
	camera.init(parser);

	if (camera.isWorking())
	{
		camera.run(streamWriter);
		std::cout << camera.errorString() << std::endl;
	}
	else
		std::cout << "ZED SDK error: " << camera.errorString() << std::endl;

	delete outputStream;
	return 0;
}
