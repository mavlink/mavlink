package pl.bezzalogowe.PhoneUAV;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;
import android.util.Log;

import pl.bezzalogowe.mavlink.MainActivity;

public class Gravity {
    public Sensor sensorInstanceGrav;
    MainActivity main;
    SensorManager gravitySensorManager;
    double angle_roll, angle_pitch;
    double angle_roll_rads, angle_pitch_rads;
    float[] gravityVectorDevice = new float[3];
    float[] gravityVectorVehicle = new float[3];

    public Gravity(MainActivity argActivity) {
        main = argActivity;
    }

    public void processGravity(SensorEvent event) {
        /** Processes data from gravity sensor event. */

        float deltaX = Math.abs(gravityVectorDevice[0] - event.values[0]);
        float deltaY = Math.abs(gravityVectorDevice[1] - event.values[1]);
        float deltaZ = Math.abs(gravityVectorDevice[2] - event.values[2]);

        if (deltaX < 0.01)
            deltaX = 0;
        if (deltaY < 0.01)
            deltaY = 0;
        if (deltaZ < 0.01)
            deltaZ = 0;

        gravityVectorDevice[0] = event.values[0];
        gravityVectorDevice[1] = event.values[1];
        gravityVectorDevice[2] = event.values[2];

        /** rotates the vector by angle between device and vehicle orientation */
        //TODO: Here you can set the angle at which the Android device is mounted in the vehicle
        //gravityVectorVehicle = rotateAroundZ(gravityVectorDevice, main.device_orientation[2]);
        gravityVectorVehicle = gravityVectorDevice;

        if (deltaX != 0 || deltaY != 0 || deltaZ != 0) {

            /** in radians */
            angle_roll_rads = Math.atan2((double) gravityVectorVehicle[0], (double) gravityVectorVehicle[2]);
            angle_pitch_rads = Math.atan2((double) gravityVectorVehicle[1], (double) gravityVectorVehicle[2]);

            /** in degrees */
            /*
            angle_roll = Math.toDegrees(Math.atan2((double) gravityVectorVehicle[0], (double) gravityVectorVehicle[2]));
            angle_pitch = Math.toDegrees(Math.atan2((double) gravityVectorVehicle[1], (double) gravityVectorVehicle[2]));
            */
            main.mavLink.sendAttitude((float) -angle_roll_rads, (float) angle_pitch_rads);
        }
    }

    public void startGravity() {
        gravitySensorManager = (SensorManager) main.getSystemService(Context.SENSOR_SERVICE);
        if (gravitySensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY) != null) {
            sensorInstanceGrav = gravitySensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
            //SENSOR_DELAY_NORMAL, SENSOR_DELAY_UI, SENSOR_DELAY_GAME, SENSOR_DELAY_FASTEST
            gravitySensorManager.registerListener(main, sensorInstanceGrav, SensorManager.SENSOR_DELAY_NORMAL);
        } else {
            Log.d("onCreate", "Gravity sensor error!");
        }
    }

    public float[] rotateAroundZ(float[] input, double angleDegrees) {
        double angleRadians = Math.toRadians(angleDegrees);
        float[] output = new float[3];
        if (angleDegrees == 180) {
            output[0] = -input[0];
            output[1] = -input[1];
            output[2] = input[2];
        } else {
            output[0] = (float) (Math.cos(angleRadians) * input[0] - Math.sin(angleRadians) * input[1]);
            output[1] = (float) (Math.sin(angleRadians) * input[0] + Math.cos(angleRadians) * input[1]);
            output[2] = input[2];
        }
        return output;
    }
}
