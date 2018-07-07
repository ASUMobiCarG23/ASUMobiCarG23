package objectRecognition;

/*
 *  DynamicDetection dd = new DynamicDetection();
 *  dd.setScanPoint(new Point(450,940));
 *  dd.detectObject(image);
 *  dd.drawObject();
 */

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;


public class DynamicDetection extends Recognition{

    int[] objRect;
    Mat Image	;
    Point[] directions;
    int angleStep = 18 ,tolerence = 10 , step = 3 ,colorThres = 15;
    Point scanPoint;
    boolean startScan= false;

    DynamicDetection()
    {
        blurred = new Mat();

        directions = new Point[360/angleStep];
        for (int i = 0; i < 360; i+=angleStep)
            directions[i/angleStep] = new Point(Math.cos(i) , -Math.sin(i));
    }

    void drawObject()
    {
        if (!startScan) return;
        Rect frame = new Rect(new Point(objRect[2],objRect[1]), new Point(objRect[3],objRect[0]));
        Imgproc.rectangle(image, frame.br(), frame.tl(), new Scalar(255,50,10) ,5);
    }

    void setScanPoint(Point scanPoint)
    {
        this.scanPoint = scanPoint;
        startScan = true;
    }

    Point getScanPoint()
    {
        return scanPoint;
    }

    void setImage(Mat Image)
    {
        this.image = image;
    }

    int[] getObjectRange(Mat src , Point scanPoint)
    {
        //get row and col range of the Object based on color

        int minX = src.cols() ,maxX = 0;
        int minY = src.rows() ,maxY = 0;

        for (int i=0; i < 360/angleStep; i++)
        {
            Point p = getBoundaryInDirection(src , scanPoint , directions[i]);
            //Imgproc.line(image, scanPoint, p, new Scalar(0,50,255) ,5);
            if (p.x >= maxX) maxX = (int)p.x;
            if (p.x <= minX) minX = (int)p.x;
            if (p.y >= maxY) maxY = (int)p.y;
            if (p.y <= minY) minY = (int)p.y;
        }
        return new int[]{minY,maxY,minX,maxX};
    }

    Point getBoundaryInDirection(Mat src ,Point startPt , Point dir)
    {

        double[] currentPtColValue = src.get((int)startPt.y , (int)startPt.x);
        double[] EndPtColValue = new double[3];
        double[] diffColorValue;
        int Tol = tolerence;
        Point endPoint = new Point();

        int length = 0; //length of trace vector beginning from the start point
        while (Tol >= 0)
        {
            Tol = Tol > tolerence? tolerence:Tol; //make sure that var Tol is not above var tolerence

            endPoint = timesConst(dir , length);
            endPoint = addPoint(startPt , endPoint);

            //if the trace point is out of src image boundaries
            if (endPoint.x >= src.cols() || endPoint.y >= src.rows()) break;
            if (endPoint.x <= 0){endPoint.x = 0; break;}
            if (endPoint.y <= 0){endPoint.y = 0; break;}

            EndPtColValue = src.get((int)endPoint.y , (int)endPoint.x);



            diffColorValue = new double[]
                    {
                            Math.abs(EndPtColValue[0]-currentPtColValue[0]),
                            Math.abs(EndPtColValue[1]-currentPtColValue[1]),
                            Math.abs(EndPtColValue[2]-currentPtColValue[2])
                    };



            if (diffColorValue[0] > colorThres || diffColorValue[1] > colorThres || diffColorValue[2] > colorThres)
                Tol-=step; //Tolerance will decrement if color difference was over the threshold
            else
            {
                Tol+=step;//Tolerance will increment if color difference was over the threshold
                currentPtColValue = EndPtColValue;
            }

            length+=step; //increment the length of the trace vector

        }

        return endPoint;
    }


    boolean detectObject(Mat image)
    {
        if (!startScan) return false;

        this.image = image;
        Imgproc.GaussianBlur(image,blurred ,new Size (3,3), 0);
        objRect = getObjectRange(blurred , scanPoint);

        //calculate new scanPoint

        scanPoint = new Point(objRect[2]/2+objRect[3]/2,objRect[0]/2+objRect[1]/2);

        if (objRect[0] >= image.rows()-20 || objRect[1] <= 20 || objRect[2] >= image.cols()-20 || objRect[3] <= 20)
        {
            //System.out.println(objRect[2] <= 20);
            startScan = false;
            return false;
        }
        return true;
    }
}
