package com.example.bleadfast.smartcaropencv;

import android.util.Log;

import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.List;
import org.opencv.imgproc.Imgproc;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.utils.Converters;

public class SignDetection {
    public Mat image , blurred , wraped , output, thresholdedImg ,hsv;
    private List<MatOfPoint> cnts  = new ArrayList<>() ;
    private MatOfPoint2f frameCorners;
    private int sign , displayCntIndex = 0;
    private String signName;

    private Dictionary<String, String> SIGNS_LOOKUP;

    public final int SIGN_TURN_LEFT = 13;
    public final int SIGN_TURN_RIGHT = 14;
    public final int SIGN_TURN_BACK = 15;

    SignDetection(Mat image)
    {
        blurred = new Mat();
        thresholdedImg = new Mat();
        hsv = new Mat();

        // define the dictionary of signs segments so we can identify
        // each signs on the image
        SIGNS_LOOKUP = new Hashtable<String, String>();
        SIGNS_LOOKUP.put("1001","Turn Right");
        SIGNS_LOOKUP.put("0011","Turn Left");
        //SIGNS_LOOKUP.put("0100","Move Straight");
        SIGNS_LOOKUP.put("1011","Turn Back");

        this.image = image;
    }

    Mat getOutput()
    {
        return output;
    }

    Mat getWraped()
    {
        return wraped;
    }

    int getSign()
    {
        return sign;
    }

    String getSignName()
    {
        return signName;
    }




    void drawSignFrame()
    {
        //draw a red square on the image
        Imgproc.drawContours(image, cnts, displayCntIndex, new Scalar(255,0,0) , 5);
    }

    void drawSignText()
    {
        Imgproc.putText(image, signName, new Point(frameCorners.get(0, 0)), Core.FONT_HERSHEY_SIMPLEX, 1, new Scalar(0, 255, 0) , 2);
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

    public boolean detectSign()
    {
        //pre-process the image by blurring it, thresholding
        //and computing an edge map
        Imgproc.GaussianBlur(image,blurred ,new Size (5,5), 0);
        Imgproc.cvtColor(blurred, hsv, Imgproc.COLOR_BGR2HSV);
        Core.inRange(hsv ,new Scalar(0,90,120) ,new Scalar (60,255,255), thresholdedImg);


        //apply morphological operations to thresholdedImg for smoothing and removing noise
        Mat kernel = new Mat(new Size(2,2) , CvType.CV_8UC1, new Scalar (1.0)); //kernel 2x2 all ones
        Imgproc.morphologyEx(thresholdedImg, thresholdedImg, Imgproc.MORPH_OPEN, kernel);
        Imgproc.morphologyEx(thresholdedImg, thresholdedImg, Imgproc.MORPH_CLOSE, kernel);


        // find contours in the edge map, then sort them by their
        // size in descending order
        Imgproc.findContours(thresholdedImg, cnts , new Mat(), Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_SIMPLE);


        //loop over the contours and find the contour enclosing the biggest area
        //also it should have 4 vertices , put the 4 vertices in matrix frameCorners
        double biggestCntArea = 0;
        frameCorners = new MatOfPoint2f();
        displayCntIndex =0;
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
                displayCntIndex = cnts.indexOf(c);
                frameCorners = temp;
            }
        }

        // extract the sign borders, apply a perspective transform
        // to it
        // A common task in computer vision and image processing is to perform
        // a 4-point perspective transform of a ROI in an image and obtain a top-down, "birds eye view" of the ROI
        if(cnts.size() == 0)
            return false;

        wraped = four_point_transform(thresholdedImg, frameCorners);
        output = four_point_transform(image, frameCorners);

        // threshold the warped image, then apply a series of morphological
        // operations to cleanup the thresholded image
        // Imgproc.THRESH_OTSU. it automatically calculates a threshold value from image histogram
        // for a bimodal image
        //Imgproc.threshold(wraped, thresholdedImg, 0, 255, Imgproc.THRESH_BINARY | Imgproc.THRESH_OTSU);
        //Mat kernel = Imgproc.getStructuringElement(Imgproc.MORPH_ELLIPSE, new Size(1,5));
        //Imgproc.morphologyEx(thresholdedImg, thresholdedImg, Imgproc.MORPH_OPEN, kernel);


        //divide the width and height wraped image and store in subWidth and subHeight
        int subWidth = wraped.cols()/10;
        int subHeight = wraped.rows()/10;

        //divide thresholdedImg to 4 parts
        Mat leftBlock = wraped.submat(4*subHeight, 9*subHeight, subWidth, 3*subWidth);
        Mat centerBlock = wraped.submat(4*subHeight, 9*subHeight, 4*subWidth, 6*subWidth);
        Mat rightBlock = wraped.submat(4*subHeight, 9*subHeight, 7*subWidth, 9*subWidth);
        Mat topBlock = wraped.submat(2*subHeight, 4*subHeight, 3*subWidth,7*subWidth);


        if ((leftBlock.cols()*leftBlock.rows()) == 0) return false;
        if ((centerBlock.cols()*centerBlock.rows()) == 0) return false;
        if ((rightBlock.cols()*rightBlock.rows()) == 0) return false;
        if ((topBlock.cols()*topBlock.rows()) == 0) return false;

        double leftFraction = Core.countNonZero(leftBlock)*255/(leftBlock.cols()*leftBlock.rows());
        double centerFraction  = Core.countNonZero(centerBlock)*255/(centerBlock.cols()*centerBlock.rows());
        double rightFraction  = Core.countNonZero(rightBlock)*255/(rightBlock.cols()*rightBlock.rows());
        double topFraction = Core.countNonZero(topBlock)*255/(topBlock.cols()*topBlock.rows());
        double[] segments = new double[]{leftFraction , centerFraction , rightFraction , topFraction};
        for (int i = 0; i < 4; i++)
        {
            segments[i] = segments[i]<230?1:0;
        }
        signName = "";
        for (double s : segments)
            signName+=(int)s;
        Log.d("sdsds", "signName: " + signName);

        signName = SIGNS_LOOKUP.get(signName);
        if (signName != null)
        {
            if (signName == "Turn Right") sign = SIGN_TURN_RIGHT;
            if (signName == "Turn Left")  sign = SIGN_TURN_LEFT;
            if (signName == "Turn Back")  sign = SIGN_TURN_BACK;

            return true;
        }

        System.out.println((int)segments[0] + "," +(int)segments[1] + ","
                +(int)segments[2] + "," +(int)segments[3]);

        return false;
    }


}
