package com.example.bleadfast.smartcaropencv;

import android.content.res.Resources;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.util.TypedValue;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.Button;
import android.graphics.Color;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Toast;
import android.view.View;
import android.annotation.SuppressLint;
import java.io.IOException;
import java.util.Set;
import java.util.UUID;
import android.widget.TextView;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import org.opencv.android.JavaCameraView;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.core.Point;
import org.opencv.imgproc.Imgproc;
import org.opencv.core.Scalar;
import org.opencv.core.Size;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2{

    //region VARIABLES
    BaseLoaderCallback baseLoaderCallback;
    CameraBridgeViewBase cameraBridgeViewBase;
    Mat mat1, mat2 ,mat3 , lookUpTable ,lines;
    String Tag = "sdsds";


    boolean bSelectTarget = false ,bSelectRear = false ,bSelectFront = false;

    //Bluetooth
    String address = null , name=null;

    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    Set<BluetoothDevice> pairedDevices;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    TextView t1;


    //activity palletes
    Button selectTarget , selectCarRear , selectCarFront , connect;



    //Recognition Objects
    PathDetection pd;
    SignDetection signDetect;
    DynamicDetection carRearDet , carFrontDet , targetDet;
    CarDetection cd;


    //Thresshold varaible
    int colorReduction = 30;
    //ThressholdData td;

    SeekBar highHue;
    SeekBar lowHue;

    SeekBar highSat;
    SeekBar lowSat;

    SeekBar highVal;
    SeekBar lowVal;

    //variables that concern showing red dot on touched point
    Point touchPoint;
    boolean touched = false;
    //endregion

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //set activity to full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.activity_main);

        //region Pallete Listeners
        selectTarget = (Button)findViewById(R.id.target);
        selectTarget.setOnClickListener(
                new Button.OnClickListener(){
                    public void onClick(View v)
                    {
                        bSelectTarget = true;
                    }
                }
        );

        selectCarRear = (Button)findViewById(R.id.CarRear);
        selectCarRear.setOnClickListener(
                new Button.OnClickListener(){
                    public void onClick(View v)
                    {
                        bSelectRear = true;
                    }
                }
        );

        selectCarFront = (Button)findViewById(R.id.CarFront);
        selectCarFront.setOnClickListener(
                new Button.OnClickListener(){
                    public void onClick(View v)
                    {
                        bSelectFront = true;
                    }
                }
        );

        connect = (Button)findViewById(R.id.Connect);
        connect.setOnClickListener(
                new Button.OnClickListener(){
                    public void onClick(View v)
                    {
                        try {bluetooth_connect_device();} catch (Exception e) {}
                    }
                }
        );

        //endregion

        //region cameraBridge
        cameraBridgeViewBase = (JavaCameraView)findViewById(R.id.myCameraView);
        cameraBridgeViewBase.setVisibility(SurfaceView.VISIBLE);
        cameraBridgeViewBase.setCvCameraViewListener(this);

        baseLoaderCallback = new BaseLoaderCallback(this) {
            @Override
            public void onManagerConnected(int status) {

                switch (status)
                {
                    case BaseLoaderCallback.SUCCESS:
                        cameraBridgeViewBase.enableView();


                        break;
                    default:
                        super.onManagerConnected(status);
                        break;
                }
            }
        };


        cameraBridgeViewBase.setOnTouchListener(
                new CameraBridgeViewBase.OnTouchListener() {
                    @Override
                    public boolean onTouch(View view, MotionEvent motionEvent) {
                        if (motionEvent.getAction() == MotionEvent.ACTION_DOWN){
                            touched = true;
                            Resources r = getResources();

                            float x = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, motionEvent.getX(), r.getDisplayMetrics());
                            float y = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, motionEvent.getY(), r.getDisplayMetrics());
                            x -= (TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, view.getWidth(), r.getDisplayMetrics()) - mat1.cols())/2;
                            y -= (TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, view.getHeight(), r.getDisplayMetrics()) - mat1.rows())/2;

                            touchPoint.x = x;
                            touchPoint.y = y;
                        }
                        return true;
                    }
                }
        );
        //endregion

    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        mat1 = inputFrame.rgba();

        if(touched)
        {
            touched = false;
            if(bSelectTarget) targetDet.setScanPoint(touchPoint);
            if(bSelectRear)   carRearDet.setScanPoint(touchPoint);
            if(bSelectFront)  carFrontDet.setScanPoint(touchPoint);
            bSelectTarget = false; bSelectRear = false; bSelectFront = false;
            Imgproc.circle(mat1 , touchPoint , 5 , new Scalar(255,0,0) , 5);
        }

        if(targetDet.detectObject(mat1))   targetDet.drawObject();
        if(carRearDet.detectObject(mat1))  carRearDet.drawObject();
        if(carFrontDet.detectObject(mat1)) carFrontDet.drawObject();




     /*   cd.setImage(mat1);
        if (cd.detectCar())
        {
            cd.drawBackFrame();
            cd.drawFrontFrame();
        }

        //if (dynamicDetection.detectObject(mat1))
        //    dynamicDetection.drawObject();


        signDetect = new SignDetection(mat1);

        if(signDetect.detectSign())
        {
            //Log.d(Tag, "detected sign!");
            signDetect.drawSignFrame();
            signDetect.drawSignText();
            //return signDetect.getOutput();
        }

/*
        Imgproc.cvtColor(mat1, mat2, Imgproc.COLOR_BGR2HSV);
        Core.inRange(mat2 ,new Scalar(0,0,0) ,new Scalar(360,100,100) , mat2);
        Imgproc.Canny(mat2 , mat2 , 3 , 100);
        Imgproc.HoughLinesP(mat2, lines, 1, Math.PI/180, 50, 10, 10 );
        pd = new PathDetection(lines , 150 , 0.8 , 25);

        pd.findPath();
        while (pd.recursion)
            pd.findPath();

        for (int i = 0; i < pd.filteredPathPtsCounter-1; i++)
        {
            Imgproc.circle(mat1, pd.filteredPath[i], 5, new Scalar(255 , 20, 50) , 2);

            //Imgproc.line(mat1, pd.filteredPath[i], pd.filteredPath[i+1], new Scalar(255 , 20, 20));
        }*/


    return mat1;
    }

    //region UNIMPORTANT
    @Override
    public void onCameraViewStarted(int width, int height) {
        mat1 = new Mat(width , height , CvType.CV_8UC3);
        mat2 = new Mat(width , height , CvType.CV_8UC3);
        mat3 = new Mat(width , height , CvType.CV_8UC3);
        lines = new Mat(width , height , CvType.CV_8UC3);

        carRearDet  = new DynamicDetection();
        carFrontDet = new DynamicDetection();
        targetDet   = new DynamicDetection();

        cd = new CarDetection();
        touchPoint = new Point();
        //fill lookup table
        lookUpTable = new Mat(1, 256, CvType.CV_8UC1);
        double[] l = new double[256];

        for (int i =0; i<256; i++)
            l[i] = ( (int)(i/colorReduction) ) * colorReduction;
        lookUpTable.put(0,0 , l);
    }


    @Override
    public void onCameraViewStopped() {
        mat1.release();
        mat2.release();
        mat3.release();

    }

    @Override
    protected void onPause() {
        super.onPause();
        if (cameraBridgeViewBase != null)
            cameraBridgeViewBase.disableView();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onResume()
    {
        super.onResume();



        if (!OpenCVLoader.initDebug())
            Toast.makeText(getApplicationContext(), "There is a problem in opencv", Toast.LENGTH_SHORT).show();
        else {
            Toast.makeText(getApplicationContext(), "opencv loaded", Toast.LENGTH_SHORT).show();
            baseLoaderCallback.onManagerConnected(BaseLoaderCallback.SUCCESS);
        }
    }


    private void bluetooth_connect_device() throws IOException
    {
        Toast.makeText(getApplicationContext(), "Connecting", Toast.LENGTH_LONG).show();
        try
        {

            t1=(TextView)findViewById(R.id.textView1);
            myBluetooth = BluetoothAdapter.getDefaultAdapter();
            address = myBluetooth.getAddress();
            pairedDevices = myBluetooth.getBondedDevices();
            if (pairedDevices.size()>0)
            {
                for(BluetoothDevice bt : pairedDevices)
                {
                    address=bt.getAddress().toString();name = bt.getName().toString();
                    Toast.makeText(getApplicationContext(),"Connected", Toast.LENGTH_SHORT).show();

                }
            }

        }
        catch(Exception we){}
        myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
        BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
        btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
        btSocket.connect();
        try { t1.setText("BT Name: "+name+"\nBT Address: "+address); }
        catch(Exception e){}
    }


    private void sendData(String i)
    {
        try
        {
            if (btSocket!=null)
            {

                btSocket.getOutputStream().write(i.toString().getBytes());
            }

        }
        catch (Exception e)
        {
            Toast.makeText(getApplicationContext(),e.getMessage(), Toast.LENGTH_SHORT).show();

        }

    }

//endregion



}
