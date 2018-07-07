package com.example.bleadfast.smartcaropencv;


import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.List;

import org.opencv.imgproc.Imgproc;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.utils.Converters;

public class Recognition {
    protected Mat image , blurred , wraped , output ,hsv;
    protected List<MatOfPoint> cnts  = new ArrayList<>() ;

    Mat getOutput()
    {
        return output;
    }

    Mat getWraped()
    {
        return wraped;
    }

    protected void drawFrameGeneral(MatOfPoint2f frame)
    {
        //draw a red square on the frame
        if (frame.total() != 4) return;
        MatOfPoint2f orderedFrame = order_points(frame);

        Point[] orderedFrameArray = frame.toArray();
        Imgproc.line(image, orderedFrameArray[0], orderedFrameArray[3], new Scalar(0,0,255) , 5);
        for (int i = 0; i<3; i++)
            Imgproc.line(image, orderedFrameArray[i], orderedFrameArray[i+1], new Scalar(0,0,255) ,5);

    }

    MatOfPoint2f order_points(MatOfPoint2f m)
    {
        //array of ordered Points
        Point[] ordered = new Point[4];

        //make array of points
        Point[] corners = new Point[4];
        corners[0] = new Point(m.get(0, 0));
        corners[1] = new Point(m.get(1, 0));
        corners[2] = new Point(m.get(2, 0));
        corners[3] = new Point(m.get(3, 0));

        //find center point
        Point center = new Point(0,0);
        for (Point p : corners)
        {
            center.x += p.x/4;
            center.y += p.y/4;
        }

        //make array of vectors pointing from center point to corners
        Point[] cornerVectors = new Point[4];
        for (int i = 0; i < 4; i++)
            cornerVectors[i] =  normalize( new Point(corners[i].x-center.x , corners[i].y-center.y));



        //make array of directions in X shape
        Point[] directions = new Point[4];
        for (int i = 45; i < 360; i+=90)
            directions[(i-45)/90] = new Point(Math.cos((i*Math.PI)/180) , -Math.sin((i*Math.PI)/180));




        //find top left point by dot product of directions[1] and all points and take maximum product
        int PointIndex = 0;
        double maxProduct = -100000;
        for (int i = 0; i < 4; i++)
        {
            double dot = cornerVectors[i].dot(directions[1]);
            if (dot > maxProduct)
            {
                maxProduct = dot;
                PointIndex = i;
            }
        }
        //add to ordered list
        ordered[0]=corners[PointIndex];

        //find top right point by dot product of directions[0] and all points and take maximum product
        maxProduct = -100000;
        for (int i = 0; i < 4; i++)
        {
            double dot = cornerVectors[i].dot(directions[0]);
            if (dot > maxProduct)
            {
                maxProduct = dot;
                PointIndex = i;
            }
        }
        //add to ordered list
        ordered[1]=corners[PointIndex];

        //find bottom right point by dot product of directions[3] and all points and take maximum product
        maxProduct = -100000;
        for (int i = 0; i < 4; i++)
        {
            double dot = cornerVectors[i].dot(directions[3]);
            if (dot > maxProduct)
            {
                maxProduct = dot;
                PointIndex = i;
            }
        }
        //add to ordered list
        ordered[2]=corners[PointIndex];

        //find bottom left point by dot product of directions[2] and all points and take maximum product
        maxProduct = -100000;
        for (int i = 0; i < 4; i++)
        {
            double dot = cornerVectors[i].dot(directions[2]);
            if (dot > maxProduct)
            {
                maxProduct = dot;
                PointIndex = i;
            }
        }
        //add to ordered list
        ordered[3]=corners[PointIndex];
        MatOfPoint2f h = new MatOfPoint2f(ordered);

        return h;
    }

    Mat four_point_transform(Mat Image, MatOfPoint2f pts)
    {
        // obtain a consistent order of the points and unpack them
        // individually
        MatOfPoint2f rect = order_points(pts);
        double[] tl = rect.get(0, 0);
        double[] tr = rect.get(1, 0);
        double[] br = rect.get(2, 0);
        double[] bl = rect.get(3, 0);


        // compute the width of the new image, which will be the
        // maximum distance between bottom-right and bottom-left
        // x-coordinates or the top-right and top-left x-coordinates
        int widthA = (int) Math.sqrt( Math.pow(br[0]-bl[0], 2) + Math.pow(br[1]-bl[1], 2) );
        int widthB = (int) Math.sqrt( Math.pow(tr[0]-tl[0], 2) + Math.pow(tr[1]-tl[1], 2) );
        int maxWidth = widthA > widthB?widthA:widthB;

        // compute the height of the new image, which will be the
        // maximum distance between the top-right and bottom-right
        // y-coordinates or the top-left and bottom-left y-coordinates
        int heightA = (int) Math.sqrt( Math.pow(tr[0]-br[0], 2) + Math.pow(tr[1]-br[1], 2) );
        int heightB = (int) Math.sqrt( Math.pow(tl[0]-bl[0], 2) + Math.pow(tl[1]-bl[1], 2) );
        int maxHeight = heightA > heightB?heightA:heightB;

        // now that we have the dimensions of the new image, construct
        // the set of destination points to obtain a "birds eye view",
        // (i.e. top-down view) of the image, again specifying points
        // in the top-left, top-right, bottom-right, and bottom-left
        // order
        List<Point> dst = new ArrayList<Point>();
        dst.add(new Point(0,0));
        dst.add(new Point(maxWidth-1,0));
        dst.add(new Point(maxWidth-1,maxHeight-1));
        dst.add(new Point(0,maxHeight-1));

        // compute the perspective transform matrix and then apply it
        Mat trans = Imgproc.getPerspectiveTransform(rect, Converters.vector_Point2f_to_Mat(dst));
        Mat warped = new Mat();
        Imgproc.warpPerspective(Image, warped, trans, new Size(maxWidth , maxHeight));

        //return the warped image
        return warped;
    }

    Point normalize(Point vector)
    {
        double mag = Math.sqrt(Math.pow(vector.x, 2) + Math.pow(vector.y, 2));
        return new Point(vector.x/mag , vector.y/mag);
    }

    protected MatOfPoint2f getFourPointContour(Mat thresholdMat)
    {
        //this function will overwrite the following variables
        //frameCorners
        //displayCntIndex
        //cnts

        cnts.clear();

        MatOfPoint2f frameCorners = new MatOfPoint2f();

        // find contours in the edge map, then sort them by their
        // size in descending order
        Imgproc.findContours(thresholdMat, cnts , new Mat(), Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_SIMPLE);

        //loop over the contours and find the contour enclosing the biggest area
        //also it should have 4 vertices , put the 4 vertices in matrix frameCorners
        double biggestCntArea = 0;
        for (MatOfPoint c : cnts)
        {

            //convert MatOfPoint to MatOfPoint2f because arcLength takes only MatOfPoint2f
            MatOfPoint2f dst = new MatOfPoint2f();
            MatOfPoint2f temp = new MatOfPoint2f();
            c.convertTo(dst, CvType.CV_32F);
            double peri = Imgproc.arcLength(dst, true);
            Imgproc.approxPolyDP(dst, temp, 0.02 * peri, true);

            //if the contour has four vertices, then we have found
            //the thermostat display
            if (temp.total() == 4 && Imgproc.contourArea(c) > biggestCntArea)
            {
                biggestCntArea = Imgproc.contourArea(c);
                frameCorners=temp;
            }
        }

        // extract the sign borders, apply a perspective transform
        // to it
        // A common task in computer vision and image processing is to perform
        // a 4-point perspective transform of a ROI in an image and obtain a top-down, "birds eye view" of the ROI
        return frameCorners;
    }
    protected MatOfPoint2f getFourPointContour(Mat thresholdMat , Point scanPoint ,double cropRatio)
    {
        //this function will overwrite the following variables
        //frameCorners
        //displayCntIndex
        //cnts

        cnts.clear();

        MatOfPoint2f frameCorners = new MatOfPoint2f();

        int height = thresholdMat.rows();
        int width = thresholdMat.cols();
        int scanY= (int) scanPoint.y;
        int scanX= (int) scanPoint.x;


        int rowRangeStart = (int) ((scanY-height)*cropRatio);
        int rowRangeEnd = (int) ((scanY+height)*cropRatio);
        int colRangeStart = (int) ((scanX-width)*cropRatio);
        int colRangeEnd = (int) ((scanX+width)*cropRatio);

        rowRangeStart = rowRangeStart < 0? 0 : rowRangeStart;
        rowRangeEnd = rowRangeEnd > height? height : rowRangeEnd;
        colRangeStart = colRangeStart < 0? 0 : colRangeStart;
        colRangeEnd = colRangeEnd > width? width : colRangeEnd;

        Mat CroppedThres = thresholdMat.submat(rowRangeStart, rowRangeEnd
                , colRangeStart, colRangeEnd);

        Imgcodecs.imwrite("C:\\Users\\BleadFast\\Desktop\\imageSamples\\ObjectRecognition\\SignDetection\\detectedFront.jpg"
                , image);

        // find contours in the edge map, then sort them by their
        // size in descending order
        Imgproc.findContours(CroppedThres, cnts , new Mat(), Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_SIMPLE);


        //loop over the contours and find the contour enclosing the biggest area
        //also it should have 4 vertices , put the 4 vertices in matrix frameCorners
        double biggestCntArea = 10000;
        for (MatOfPoint c : cnts)
        {

            //convert MatOfPoint to MatOfPoint2f because arcLength takes only MatOfPoint2f
            MatOfPoint2f dst = new MatOfPoint2f();
            MatOfPoint2f temp = new MatOfPoint2f();
            c.convertTo(dst, CvType.CV_32F);
            double peri = Imgproc.arcLength(dst, true);
            Imgproc.approxPolyDP(dst, temp, 0.02 * peri, true);

            //if the contour has four vertices, then we have found
            //the thermostat display
            if (temp.total() == 4 && Imgproc.contourArea(c) > biggestCntArea)
            {
                biggestCntArea = Imgproc.contourArea(c);
                frameCorners = temp;
            }
        }

        return frameCorners;

    }
    protected MatOfPoint2f getFourPointContour(Mat thresholdMat , int[] cropRect)
    {
        //this function will overwrite the following variables
        //frameCorners
        //displayCntIndex
        //cnts

        cnts.clear();

        MatOfPoint2f frameCorners = new MatOfPoint2f();

        int height = thresholdMat.rows();
        int width = thresholdMat.cols();


        int rowRangeStart = cropRect[0];
        int rowRangeEnd = cropRect[1];
        int colRangeStart = cropRect[2];
        int colRangeEnd = cropRect[3];

        rowRangeStart = rowRangeStart < 0? 0 : rowRangeStart;
        rowRangeEnd = rowRangeEnd > height? height : rowRangeEnd;
        colRangeStart = colRangeStart < 0? 0 : colRangeStart;
        colRangeEnd = colRangeEnd > width? width : colRangeEnd;

        Mat CroppedThres = thresholdMat.submat(rowRangeStart, rowRangeEnd
                , colRangeStart, colRangeEnd);


        // find contours in the edge map, then sort them by their
        // size in descending order
        Imgproc.findContours(CroppedThres, cnts , new Mat(), Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_SIMPLE);


        //loop over the contours and find the contour enclosing the biggest area
        //also it should have 4 vertices , put the 4 vertices in matrix frameCorners
        double biggestCntArea = 10000;
        for (MatOfPoint c : cnts)
        {

            //convert MatOfPoint to MatOfPoint2f because arcLength takes only MatOfPoint2f
            MatOfPoint2f dst = new MatOfPoint2f();
            MatOfPoint2f temp = new MatOfPoint2f();
            c.convertTo(dst, CvType.CV_32F);
            double peri = Imgproc.arcLength(dst, true);
            Imgproc.approxPolyDP(dst, temp, 0.02 * peri, true);

            //if the contour has four vertices, then we have found
            //the thermostat display
            if (temp.total() == 4 && Imgproc.contourArea(c) > biggestCntArea)
            {
                biggestCntArea = Imgproc.contourArea(c);
                frameCorners = temp;
            }
        }

        return frameCorners;

    }
    Point addPoint(Point pt1, Point pt2)
    {
        return new Point(pt1.x+pt2.x , pt1.y+pt2.y);
    }

    Point getDirection(int angle)
    {
        return new Point(Math.cos(angle) , -Math.sin(angle));
    }


    Point timesConst(Point pt , double c)
    {
        Point p = new Point(pt.x*c , pt.y*c);
        return p;
    }

}
