package objectRecognition;
import org.opencv.core.*;

//-----------------------------Not Complete--------------------------------//

public class PathDetectionV2 {

	Point[][] sections;
	Mat src;
	private int iterations ,minLineGap , minLineLength , maxLineLength 
	, lineGapTolerence , step , threshold , angleThreshold;
	
	
	private Point sectionDir = new Point(0 , -1);
	private Point[] directions;
	private int angleStep = 30;
	int sectionCounter = 0;
	
	final int ALL_DIRECTIONS = 0;
	final int HORIZONTAL = 1;
	
	PathDetectionV2(Mat src , int iterations , int minLineGap , int step , int threshold , int angleThreshold)
	{
		this.src = src;
		this.iterations = iterations;
		this.minLineGap = minLineGap;
		this.step = step;
		this.threshold = threshold;
		this.angleThreshold = angleThreshold;
		
		directions = new Point[360/angleStep];
		for (int i = 0; i < 360; i+=angleStep)
			directions[i/angleStep] = new Point(Math.cos(i) , -Math.sin(i));
		
		sections = new Point[iterations][3];
	}
	
	Point getDirection(int angle)
	{
		return new Point(Math.cos(angle) , -Math.sin(angle));
	}
	
	Point normalize(Point vector)
	{
		double mag = Math.sqrt(Math.pow(vector.x, 2) + Math.pow(vector.y, 2));
		return new Point(vector.x/mag , vector.y/mag);
	}
	
	Point timesConst(Point pt , double c)
	{
		Point p = new Point(pt.x*c , pt.y*c);
		return p;
	}
	
	Point addPoint(Point pt1, Point pt2)
	{
		return new Point(pt1.x+pt2.x , pt1.y+pt2.y);
	}
	
	int getLineGap(Point startPt , Point endPoint ,int state)
	{	
		int shortestLine = 1000;
		
		//startPt has to be on the border of the path
		//gets the length of the horizontal line
		if (state == HORIZONTAL)
		{
			shortestLine = countLightPixels(startPt , new Point(1 , 0) , false);
			endPoint = addPoint(timesConst(new Point(1 , 0), shortestLine) , startPt);
		}
			
		
		
		//startPt has to be on the border of the path
		//returns the length of the shortest possible line passing through startPt which is considered the thickness of the path
		//also calculates the end point of this shortest line and puts it in endPoint
		if (state == ALL_DIRECTIONS)
		{
			
			for (int i=0; i < 360/angleStep; i++)
			{
				int x = countLightPixels(startPt , directions[i] , false);
				if (x < shortestLine)
				{
					shortestLine = x;
					endPoint = addPoint( timesConst(directions[i],x) , startPt);
				}
			}	
		}
		
		return shortestLine;
	}
	
	int getLongestLine(Point startPt , Point endPoint , boolean parallelToSectioDir)
	{
		//returns the length of the longest possible line passing through startPt with the constraint of it being parallel to (sectionDir)
		//also calculates the endPoint of that line
		int longestLength = 0;
		int directionIndex = 0;
		for (int i=0; i < 180/angleStep; i++)
		{
			int x = countLightPixels(startPt , directions[i] , true);
			if (x > longestLength)
			{
				longestLength = x;
				directionIndex = i;
			}
		}
		if( directions[directionIndex].dot(sectionDir) > 0)
			endPoint = addPoint(timesConst(directions[directionIndex] , countLightPixels(startPt , directions[directionIndex] , false)) , startPt);
		else
			endPoint = addPoint(timesConst(directions[directionIndex] , countLightPixels(startPt , timesConst(directions[directionIndex],-1) , false)) , startPt);
		
		return longestLength;
	}
	
	boolean registerSection(Point startPt , Point dir)
	{
		int width;
		
		Point perpendicularDir = new Point(dir.y , -dir.x);
		int x = countLightPixels(startPt , perpendicularDir , false);
		Point pt1 = timesConst(perpendicularDir, x);
		
		width = x;
		
		perpendicularDir = timesConst(perpendicularDir,-1);
		x = countLightPixels(startPt , perpendicularDir , false);
		Point pt2 = timesConst(perpendicularDir, x);
		
		width += x;
		
		if (width >= minLineGap)
		{
		sections[sectionCounter][0] = pt1;
		sections[sectionCounter][1] = pt2;
		sections[sectionCounter][2] = dir;
		
		sectionCounter++;
		
		return true;
		}
		
		return false;
	}
	
	int countLightPixels(Point startPt , Point dir , boolean bothDirections)
	{
		double colorValue = src.get((int)startPt.y, (int)startPt.x)[0];
		int positiveTol = lineGapTolerence;
		int negativeTol = lineGapTolerence;
		Point endPoint = new Point();
		
		int length = 0;
		while (length < maxLineLength)
		{
			if (positiveTol >= 0)
			{
				endPoint = timesConst(dir , length/step);
				endPoint = addPoint(startPt , endPoint);			
				if (src.get((int)endPoint.y, (int)endPoint.x)[0] - colorValue > threshold)		
					positiveTol-=step;
				else positiveTol = lineGapTolerence;
				length+=step;
			}
			
			if (negativeTol >= 0 && bothDirections)
			{
				endPoint = timesConst(dir , -length/step);
				endPoint = addPoint(startPt , endPoint);			
				if (src.get((int)endPoint.y, (int)endPoint.x)[0] - colorValue > threshold)		
					negativeTol-=step;
				else negativeTol = lineGapTolerence;
				length+=step;
			}
			
			if (negativeTol < 0 && positiveTol < 0) return (length-2*lineGapTolerence);
			if (!bothDirections && positiveTol < 0) return (length-lineGapTolerence);
		}
		return 0;
	}
	
	void findBeginSegment()
	{
		Point pt = new Point();
		Point endPoint = new Point(); // pt will be = to endPoint if the first test succeeded
		
		//loop through all the Mat from bottom to top with step and test for possible path segment
        for (pt.y = src.rows(); pt.y > 0 ; pt.y-=step) 
        {
            for (pt.x = 0;  pt.x < src.cols(); pt.x+=step) 
            {
            	double colorValue = src.get((int)pt.y , (int)pt.x)[0];
            	
            	if(colorValue > threshold) //first test
            		if (getLineGap(pt , endPoint , HORIZONTAL) > minLineGap) //second test ,note that endPoint has been altered
            		{
            			Point midPoint = timesConst(addPoint(pt , endPoint) , 0.5);
            			Point tempPoint = new Point();
            			getLongestLine(midPoint , tempPoint); 
            			//tempPoint is equal to intersection of longest line passing through midPoint and 
            			//the actual path
            			
            		}
            		else pt = endPoint; //jump the white gap because it failed the second test
            }	
        }
	}
	
	boolean sweep(Point startPt , int stepDegree , int threshold)
	{
		double[] section = new double[3];
		double colorValue = src.get((int)startPt.y, (int)startPt.x)[0];
		int longestWay = 0;
		
		return true;
	}
}
