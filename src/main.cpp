#include "OdometryCam.hpp"


int main(int argc, char **argv)
{
	const cv::String keys =
		"{help h usage ? || print this message}"
		"{printstatus p || print the status of the camera or svo file}"
		"{input i || SVO filename (ex : -i=test.svo  or --input=test.svo) }";

    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    OdometryCam camera(parser.get<std::string>("input"));

    if (parser.has("printstatus"))
    {
        camera.printStatus();
        return 0;
    }

    camera.init(parser);

    if (camera.isWorking())
        camera.run();
    else
        std::cout << "ZED SDK error: " << camera.errorString() << std::endl;

    return 0;
}
