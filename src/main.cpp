#include "OdometryCam.hpp"
#include "PoseStreamWriter.hpp"

#ifdef _SL_JETSON_
// OpenCV 2.x defines to fix method incompatibilities from 3.x
#define printMessage printParams
#define has(option) get<bool>(option)
#endif // _SL_JETSON_

int main(int argc, char **argv)
{
	const cv::String keys =
#ifdef _SL_JETSON_
	// This is the OpenCV 2.x case, for the TK1 board
		"{h | help        |false | print this message}"
		"{p | printstatus |false | print the status of the camera or svo file}"
		"{i | input       |      | SVO filename (ex: -i=test.svo) }"
		"{o | output      |      | filename for pose data (ex: -o=data.txt) }"
		"{q | queryinterval| 0   | seconds between camera queries (ex: -q=0.5 }"
;
#else
	// This is the OpenCV 3.x case, which is what the Windows SDK uses
		"{help h usage ? || print this message}"
		"{printstatus p || print the status of the camera or svo file}"
		"{input i || SVO filename (ex : -i=test.svo  or --input=test.svo) }"
		"{output o || filename for pose data (ex : -o=data.txt  or --output=data.txt) }";
#endif

#ifdef _SL_JETSON_
	cv::CommandLineParser parser(argc, argv, keys.c_str());
#else
	cv::CommandLineParser parser(argc, argv, keys);
#endif

	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	// parse --input
	std::string inputFile = parser.get<std::string>("input");
	if (inputFile == "true")
	{
		std::cout << "Invalid syntax, expected \"-i=file\"\n";
		return 0;
	}

	// parse --output
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

	// parse --queryinterval
	std::string queryIntervalStr = parser.get<std::string>("queryinterval");
	std::chrono::milliseconds queryInterval;
	try
	{
		double queryIntervalD = std::stof(queryIntervalStr);
		queryInterval = std::chrono::milliseconds((int)(queryIntervalD * 1000));
	}
	catch (...)
	{
		std::cout << "Invalid query interval, expected floating point or integer number in seconds.\n";
		return 0;
	}


	// Create camera object
	OdometryCam camera(parser.get<std::string>("input"));

	// parse --printstatus
	if (parser.has("printstatus"))
	{
		std::cout << "Printing status." << std::endl;
		camera.printStatus();
		delete outputStream;
		return 0;
	}

	// Init camera and run main camera loop
	PoseStreamWriter streamWriter(*outputStream);
	camera.init(parser);

	if (camera.isWorking())
	{
		camera.run(streamWriter, queryInterval);
		std::cout << camera.errorString() << std::endl;
	}
	else
		std::cout << "ZED SDK error: " << camera.errorString() << std::endl;

	delete outputStream;
	return 0;
}
