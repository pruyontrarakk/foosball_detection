#include <iostream>
#include "gui/gui.hpp"
#include <string>
#include <sstream>
using namespace std;

void gui::showOriginalFrame(bool originalEnabled, cv::Mat& frame)
{
	const string title = "Original frame";
	if(originalEnabled)
	{
		cv::imshow(title, frame);
	}
	else
	{
		try{
			cv::destroyWindow(title);
		}catch(...){}
	}
}

void gui::handlePressedKeys(int key, bool& originalEnabled, bool& trackingEnabled, bool& blueDetectionEnabled, bool& redDetectionEnabled, bool& pause, bool& debugMode)
{
	switch(key){
		case 27: //'esc' key has been pressed, exit program.
			exit(EXIT_SUCCESS);
		case 'o': //'t' has been pressed. this will toggle tracking
			originalEnabled = !originalEnabled;
			if(originalEnabled == false) std::cout<<"Origin frame disabled."<<std::endl;
			else std::cout<<"Origin frame enabled."<<std::endl;
			break;
		case 't': //'t' has been pressed. this will toggle tracking
			trackingEnabled = !trackingEnabled;
			if(trackingEnabled == false) std::cout<<"Tracking ball disabled."<<std::endl;
			else std::cout<<"Tracking ball enabled."<<std::endl;
			break;
		case 'b': //'b' has been pressed. this will toggle blue players detection
			blueDetectionEnabled = !blueDetectionEnabled;
			if(blueDetectionEnabled == false) std::cout<<"Blue players detection disabled."<<std::endl;
			else std::cout<<"Blue players detection enabled."<<std::endl;
			break;
		case 'r': //'r' has been pressed. this will toggle red players detection
			redDetectionEnabled = !redDetectionEnabled;
			if(redDetectionEnabled == false) std::cout<<"Red players detection disabled."<<std::endl;
			else std::cout<<"Red players detection enabled."<<std::endl;
			break;
		case 'd': //'d' has been pressed. this will debug mode
			debugMode = !debugMode;
			if(debugMode == false) std::cout<<"Debug mode disabled."<<std::endl;
			else std::cout<<"Debug mode enabled."<<std::endl;
			break;
		case 'p': //'p' has been pressed. this will pause/resume the code.
			pause = !pause;
			if(pause == true){ 
				std::cout<<"Code paused, press 'p' again to resume"<<std::endl;
				while (pause == true){
					//stay in this loop until 
					switch (cv::waitKey()){
						//a switch statement inside a switch statement? Mind blown.
						case 'p': 
						//change pause back to false
						pause = false;
						std::cout<<"Code resumed."<<std::endl;
						break;
					}
				}
			}
		}
}

void gui::printKeyDoc(cv::Mat& frame, int x, int y)
{
	cv::putText(frame,
	"o - show origin view",
	cv::Point(x, y), cv::FONT_HERSHEY_DUPLEX,
	0.5, cv::Scalar(255, 255, 255), 1, CV_AA);
	cv::putText(frame,
	"t - enable ball tracking",
	cv::Point(x, y+15), cv::FONT_HERSHEY_DUPLEX,
	0.5, cv::Scalar(255, 255, 255), 1, CV_AA);
	cv::putText(frame,
	"b - enable blue players detection",
	cv::Point(x+300, y), cv::FONT_HERSHEY_DUPLEX,
	0.5, cv::Scalar(255, 255, 255), 1, CV_AA);
	cv::putText(frame,
	"r - enable red players detection",
	cv::Point(x+300, y+15), cv::FONT_HERSHEY_DUPLEX,
	0.5, cv::Scalar(255, 255, 255), 1, CV_AA);
	cv::putText(frame,
	"d - enable view of debug frames",
	cv::Point(x+600, y), cv::FONT_HERSHEY_DUPLEX,
	0.5, cv::Scalar(255, 255, 255), 1, CV_AA);
	cv::putText(frame,
	"p - pause",
	cv::Point(x+600, y+15), cv::FONT_HERSHEY_DUPLEX,
	0.5, cv::Scalar(255, 255, 255), 1, CV_AA);
}

void gui::printScoreBoard(detection::ScoreCounter& scoreCounter, cv::Mat &res, int x, int y)
{
    stringstream ss;
    ss << scoreCounter.getScoreLeft() << " - "
       << scoreCounter.getScoreRight() << " (outs: " << scoreCounter.getScoreOuts() << ')';
    cv::putText(res, ss.str(), cv::Point(x, y), cv::FONT_HERSHEY_DUPLEX,
        1, cv::Scalar(255, 255, 255), 2, CV_AA);
}

void gui::showCenterPosition(cv::Mat& res, cv::Point center, int x, int y)
{
	string xOfCenter = to_string(center.x);
	string yOfCenter = to_string(center.y);
	string tmp = "Ball position: (" + xOfCenter + ", " + yOfCenter + ')';
	cv::putText(res, tmp, cv::Point(x,y), cv::FONT_HERSHEY_DUPLEX, 0.5, 
				cv::Scalar(255,255,255), 1, CV_AA);
}

void gui::showStatistics(cv::Mat& res, int founded, int all, int x, int y)
{
	int tmp = founded*1.0/(all*1.0)*100;
	string t = to_string(tmp);
	string result = "Ball tracking accuracy: " + t + '%';
	        
    cv::putText(res, result, cv::Point(x,y), cv::FONT_HERSHEY_DUPLEX, 0.5,
				 cv::Scalar(255,255,255), 1, CV_AA);
}
