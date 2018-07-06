package com.example.bleadfast.smartcaropencv;

/*
CarDetection cd = new CarDetection(image);
cd.setImage(image);
if (cd.detectCar())
{
	cd.drawBackFrame();
	cd.drawFrontFrame();
}
*/

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

public class CarDetection extends Recognition{

    Scalar[] carBackHSVRange;
    Scalar[] carFrontHSVRange;

    Mat thresholdFront , thresholdBack;
    private MatOfPoint2f frameCornersBack , frameCornersFront;
    CarDetection()
    {
        blurred = new Mat();
        hsv = new Mat();
        thresholdFront = new Mat();
        thresholdBack  = new Mat();
        this.image = image;


        //define HSV range of back color of the car
        carBackHSVRange  = new Scalar[2];
        carBackHSVRange[0] = new Scalar(80,65,35);
        carBackHSVRange[1] = new Scalar(110,255,255);
        //define HSV range of front color of the car
        carFrontHSVRange = new Scalar[2];
        carFrontHSVRange[0] = new Scalar(0,65,35);
        carFrontHSVRange[1] = new Scalar(20,255,255);
    }

    void setImage(Mat image)
    {
        this.image = image;
    }


    void drawBackFrame()
    {
        drawFrameGeneral(frameCornersBack);
    }

    void drawFrontFrame()
    {
        drawFrameGeneral(frameCornersFront);
    }


    boolean detectCar()
    {
        //pre-process the image by blurring it, thresholding
        //and computing an edge map
        //Imgproc.GaussianBlur(image,blurred ,new Size (3,3), 0);
        Imgproc.cvtColor(image, hsv, Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv ,carBackHSVRange[0] ,carBackHSVRange[1] , thresholdBack);
        Core.inRange(hsv ,carFrontHSVRange[0] ,carFrontHSVRange[1] , thresholdFront);


        //process the car back threshold
        frameCornersBack = getFourPointContour(thresholdBack);
        if (frameCornersBack.empty()) return false;

        //find center of the car back
        Point backCenter = new Point(0,0);
        for (int i = 0; i < 4; i++)
        {
            backCenter.x += frameCornersBack.get(i, 0)[0]/4;
            backCenter.y += frameCornersBack.get(i, 0)[1]/4;
        }

        //process the car front threshold
        frameCornersFront = getFourPointContour(thresholdFront , backCenter , 0.7);
        if (frameCornersFront.empty()) return false;

        //find center of the car front
        Point frontCenter = new Point(0,0);
        for (int i = 0; i < 4; i++)
        {
            frontCenter.x += frameCornersFront.get(i, 0)[0]/4;
            frontCenter.y += frameCornersFront.get(i, 0)[1]/4;
        }

        return true;
    }


}
