package objectRecognition;

import java.util.Dictionary;
import java.util.Hashtable;
import org.opencv.imgproc.Imgproc;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;


/*

signDetect = new SignDetection(mat1);

if(signDetect.detectSign())
{
    signDetect.drawSignFrame();
    signDetect.drawSignText();
}
*/
public class SignDetection extends Recognition{

	Mat thresholdedImg;
    private int sign ;
    private String signName;
    private MatOfPoint2f frameCorners;
    private Dictionary<String, String> SIGNS_LOOKUP;
    
    public final int SIGN_TURN_LEFT = 13;
    public final int SIGN_TURN_RIGHT = 14;
    public final int SIGN_TURN_BACK = 15;
    
	SignDetection(Mat image)
	{
	    blurred = new Mat();
	    thresholdedImg = new Mat();
	    hsv = new Mat();
	    this.image = image;
	    frameCorners = new MatOfPoint2f();
	    
	    // define the dictionary of signs segments so we can identify
	    // each signs on the image
	    SIGNS_LOOKUP = new Hashtable<String, String>();	
	    SIGNS_LOOKUP.put("1001","Turn Right");
	    SIGNS_LOOKUP.put("0011","Turn Left");
	    //SIGNS_LOOKUP.put("0100","Move Straight");
	    SIGNS_LOOKUP.put("1011","Turn Back");
	    
		
	}
	
	void drawFrame()
	{
		drawFrameGeneral(frameCorners);
	}
	
	int getSign()
	{
		return sign;
	}

	
	
	String getSignName()
	{
		return signName;
	}
	   
    void drawSignText()
    {
    	Imgproc.putText(image, signName, new Point(frameCorners.get(0, 0)), Core.FONT_HERSHEY_SIMPLEX, 1, new Scalar(0, 255, 0) , 2);
    }

	boolean detectSign()
	{
	    //pre-process the image by blurring it, thresholding
	    //and computing an edge map
	    Imgproc.GaussianBlur(image,blurred ,new Size (5,5), 0);
	    Imgproc.cvtColor(blurred, hsv, Imgproc.COLOR_BGR2HSV);
	    Core.inRange(hsv ,new Scalar(85,100,70) ,new Scalar (115,255,255), thresholdedImg);


	    //apply morphological operations to thresholdedImg for smoothing and removing noise
	    //Mat kernel = new Mat(new Size(4,4) , CvType.CV_8UC1, new Scalar (1.0)); //kernel 3x3 all ones
	    //Imgproc.morphologyEx(thresholdedImg, thresholdedImg, Imgproc.MORPH_OPEN, kernel);
	    //Imgproc.morphologyEx(thresholdedImg, thresholdedImg, Imgproc.MORPH_CLOSE, kernel);
	    //System.out.println(kernel.dump());
	    
	    
		// find contours in the edge map and extract the border corners
	    
	    frameCorners = getFourPointContour(thresholdedImg);
	    if (frameCorners.empty()) return false;
	    
	    
	    // apply a perspective transform to it
	    // A common task in computer vision and image processing is to perform 
	    // a 4-point perspective transform of a ROI in an image and obtain a top-down, "birds eye view" of the ROI
	    
	    
	    wraped = four_point_transform(thresholdedImg, frameCorners);
	    output = four_point_transform(image, frameCorners);
	    
	    //divide the width and height wraped image and store in subWidth and subHeight
	    int subWidth = wraped.cols()/10;
	    int subHeight = wraped.rows()/10;
	    
	    //divide thresholdedImg to 4 parts
	    Mat leftBlock = wraped.submat(4*subHeight, 9*subHeight, subWidth, 3*subWidth);
	    Mat centerBlock = wraped.submat(4*subHeight, 9*subHeight, 4*subWidth, 6*subWidth);
	    Mat rightBlock = wraped.submat(4*subHeight, 9*subHeight, 7*subWidth, 9*subWidth);
	    Mat topBlock = wraped.submat(2*subHeight, 4*subHeight, 3*subWidth,7*subWidth);
	    
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
