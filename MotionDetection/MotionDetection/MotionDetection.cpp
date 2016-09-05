#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

#include <iostream>
#include <stdio.h>
#include <memory>

using namespace cv;
using namespace std;

static bool RectanglesOverlap(Rect r1, Rect r2)
{
    if ((r1.x + r1.width) < r2.x || (r1.y + r1.height < r2.y) || (r2.x + r2.width) < r1.x || (r2.y + r2.height < r1.y))
        return false;
    else
        return true;
}

static size_t RectArea(Rect r)
{
    return (r.width * r.height);
}

int main(void)
{
    auto capture = VideoCapture("Path_to_file");
    if (!capture.isOpened())
    {
        cerr << "Failed to open the video!\n" << endl;
    }

    Mat frame;
    Mat frameGray;
    Mat firstFrame;
    Mat frameDelta;
    Mat thresh;
    size_t frameInd = 0;
    size_t lastRecInd = 0;
    while (true)
    {
        ++frameInd;

        capture >> frame;
        if (frame.empty())
            break;

        cvtColor(frame, frameGray, CV_BGR2GRAY);

        GaussianBlur(frameGray, frameGray, Size(21, 21), 0);

        if (firstFrame.empty())
        {
            frameGray.copyTo(firstFrame);
            lastRecInd = frameInd;
        }

        absdiff(firstFrame, frameGray, frameDelta);

        threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
        dilate(thresh, thresh, Mat(), Point(-1, -1), 2);
        vector<vector<Point> > contours;
        findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // Get all rectangular contours
        vector<Rect> rects;
        rects.reserve(contours.size());
        for (auto& contour : contours)
        {
            rects.push_back(boundingRect(contour));
        }

        // Join overlapping rectangle contours
        vector<bool> joined(rects.size(), false);
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (size_t i = 0; i < rects.size(); i++)
            {
                if (joined[i] || RectArea(rects[i])< 8000) continue;
                for (size_t j = i + 1; j < rects.size(); j++)
                {
                    if (RectanglesOverlap(rects[i], rects[j]))
                    {
                        changed = true;
                        for (Point& p : contours[i])
                            contours[j].push_back(p);
                        rects[j] = boundingRect(contours[j]);

                        joined[i] = true;
                    }
                }
            }
        }

        rects.clear();
        for (size_t i = 0; i < joined.size(); i++)
        {
            if (!joined[i])
                rects.push_back(boundingRect(contours[i]));
        }

        std::stringstream ss;

        ss << frameInd;

        imwrite("D:\\imgout\\frame_" + ss.str() + ".jpg", frame);
    }

}

