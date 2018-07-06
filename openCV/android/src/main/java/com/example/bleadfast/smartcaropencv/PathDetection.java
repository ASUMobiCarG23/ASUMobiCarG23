package com.example.bleadfast.smartcaropencv;

import org.opencv.core.*;

public class PathDetection {
    private Mat lines;
    private double maxDstSqr , highestY , angleThreshold;
    private Point pathPt , pathDir , filteredPathDir;
    public Point[] path , filteredPath;
    public int pathPtsCounter ,filteredPathPtsCounter ;
    public boolean recursion;
    private int iterations;

    private static final int PATH = 0;
    private static final int FILTERED_PATH = 0;

    PathDetection(Mat m , double maxDst , double a , int i)
    {
        lines = new Mat();
        lines = m;
        maxDstSqr = maxDst*maxDst;
        angleThreshold = a;
        iterations = i;
        path = new Point[lines.rows()*2];
        filteredPath = new Point[lines.rows()*2];
        recursion = false;
        pathPt = new Point();
        pathDir = new Point();
        pathPtsCounter=0;
        filteredPathPtsCounter=0;
    }


    Point getDirection(double[] line)
    {
        Point vector = new Point (line[2]-line[0] , line[3]-line[1]);
        return normalize(vector);
    }
    Point getDirection(Point pt1 , Point pt2)
    {
        Point vector = new Point (pt2.x-pt1.x , pt2.y-pt1.y);
        return normalize(vector);
    }






    Point normalize(Point vector)
    {
        double mag = Math.sqrt(Math.pow(vector.x, 2) + Math.pow(vector.y, 2));
        return new Point(vector.x/mag , vector.y/mag);
    }

    Point timesConst(Point pt , int c)
    {
        Point p = new Point(pt.x*c , pt.y*c);
        return p;
    }

    Point getPt(double[] line , int index)
    {
        return new Point(line[index*2] , line[1+index*2]);
    }

    Point getVec(Point pt1 , Point pt2)
    {

        return new Point(pt2.x-pt1.x , pt2.y-pt1.y);
    }

    double sqrMag(Point v)
    {
        return Math.pow(v.x, 2) + Math.pow(v.y, 2);
    }
    double sqrMag(double[] line , int index)
    {
        Point v = new Point(line[index*2] , line[1+index*2]);
        return Math.pow(v.x, 2) + Math.pow(v.y, 2);
    }

    boolean chkParallelism(Point vec1 , Point vec2 , double angle)
    {
        Point normVec1 = normalize(vec1);
        Point normVec2 = normalize(vec2);
        if (Math.abs(normVec1.dot(normVec2)) > angle)
            return true;
        return false;
    }
    boolean chkParallelism(double[] line1 , double[] line2 , double angle)
    {
        Point vec1 = new Point(line1[2]-line1[0] , line1[3]-line1[1]);
        Point vec2 = new Point(line2[2]-line2[0] , line2[3]-line2[1]);

        Point normVec1 = normalize(vec1);
        Point normVec2 = normalize(vec2);

        if (Math.abs(normVec1.dot(normVec2)) > angle)
            return true;
        return false;
    }

    boolean registerPoint(double[] line , int index , int reg , boolean atteptRegistration)
    {
        Point pt = getPt(line , index);
        return registerPoint(pt  , reg , atteptRegistration);
    }
    boolean registerPoint(Point pt , int reg ,boolean atteptRegistration )
    {
        if (reg == PATH)
        {
            for (int i = 0; i < pathPtsCounter; i++) {
                if (path[i].x == pt.x && path[i].y == pt.y)
                    return true;
            }

            if (atteptRegistration) {
                path[pathPtsCounter] = pt;
                pathPtsCounter++;
            }
        }

        if (reg == FILTERED_PATH)
        {
            for (int i = 0; i < filteredPathPtsCounter; i++) {
                if (filteredPath[i].x == pt.x && filteredPath[i].y == pt.y)
                    return true;
            }

            if (atteptRegistration) {
                filteredPath[filteredPathPtsCounter] = pt;
                filteredPathPtsCounter++;
            }
        }
        return false;
    }

    //find beginning point and direction of path
    //if recursion is true then pathPt will be equal to what was last calculated
    void findPath()
    {
        if (lines.rows() == 0) return;

        int pathLineIndex = 0;
        if (!recursion)
        {
            for (int i = 0; i < lines.rows(); i++)
            {
                if (lines.get(i, 0)[1] > highestY)
                {
                    highestY = lines.get(i, 0)[1];
                    pathPt.x = lines.get(i, 0)[0];
                    pathPt.y = lines.get(i, 0)[1];
                    pathLineIndex = i;
                }

                if (lines.get(i, 0)[3] > highestY)
                {
                    highestY = lines.get(i, 0)[3];
                    pathPt.x = lines.get(i, 0)[2];
                    pathPt.y = lines.get(i, 0)[3];
                    pathLineIndex = i;
                }
            }
            //get direction at the beginning of the path and register the first line
            pathDir = getDirection(lines.get(pathLineIndex, 0)).y<0?getDirection(lines.get(pathLineIndex, 0)):timesConst(getDirection(lines.get(pathLineIndex, 0)),-1);
            filteredPathDir = pathDir;
            registerPoint(lines.get(pathLineIndex, 0) , 0 , PATH , true);
            registerPoint(lines.get(pathLineIndex, 0) , 1 , PATH , true);
        }
        //--------------------------------------//

        double temp = maxDstSqr;
        boolean foundNewPathPt = false;
        Point loopPt;
        Point loopPathDir;
        Point vector;
        Point newPathPt  = new Point();
        Point newPathDir = new Point();
        recursion = false;
        for (int i = 0; i < lines.rows(); i++)
        {
            loopPt = getPt(lines.get(i, 0), 0);
            vector = getVec(pathPt , loopPt);
            loopPathDir = normalize( getVec(loopPt , getPt(lines.get(i, 0), 1)) );
            if (!registerPoint(loopPt ,PATH , false) && ( sqrMag(vector) < temp) && pathDir.dot(vector) > 0
                    &&	chkParallelism(pathDir, loopPathDir, angleThreshold))
            {
                newPathPt = loopPt;
                newPathDir = loopPathDir;
                temp = sqrMag(vector);
                foundNewPathPt = true;
            }

            loopPt = getPt(lines.get(i, 0), 1);
            vector = getVec(pathPt , loopPt);
            loopPathDir = normalize( getVec(loopPt , getPt(lines.get(i, 0), 0)) );
            if (!registerPoint(loopPt , PATH , false) && ( sqrMag(vector) < temp) && pathDir.dot(vector) > 0
                    &&	chkParallelism(pathDir, loopPathDir, angleThreshold))
            {
                newPathPt = loopPt;
                newPathDir = loopPathDir;
                temp = sqrMag(vector);
                foundNewPathPt = true;
            }
        }

        if (foundNewPathPt && iterations > 0)
        {
            iterations--;
            pathPt = newPathPt;
            pathDir = newPathDir.dot(pathDir) > 0? newPathDir : timesConst(newPathDir, -1);
            registerPoint(pathPt , PATH , true);

            if (pathDir.dot(filteredPathDir) < angleThreshold)
            {
                filteredPathDir = pathDir;
                registerPoint(pathPt , FILTERED_PATH , true);
            }

            recursion = true;
            return;
        }

        if (!recursion)
            registerPoint(pathPt , FILTERED_PATH , true);


    }
}
