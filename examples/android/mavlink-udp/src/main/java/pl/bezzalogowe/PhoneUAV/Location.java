package pl.bezzalogowe.PhoneUAV;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.Log;
import androidx.core.content.ContextCompat;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.LinkedList;

import pl.bezzalogowe.mavlink.MainActivity;
import pl.bezzalogowe.mavlink.UpdateTextThread;

public class Location extends Thread {
    private static final String TAG = "location";
    LocationManager locationManager;
    android.location.Location recentLocation;
    android.location.Location waypointNext;
    LinkedList<Waypoint> route = new LinkedList<Waypoint>();
    String deviceIdentifier = null;
    MainActivity main;
    double[] altitudesample = new double[10];

    public Location(final MainActivity argActivity) {
        main = argActivity;
    }

    /** converts one double to an array of 8 bytes */
    public static final byte[] double2Bytes(double inData) {
        long bits = Double.doubleToLongBits(inData);
        byte[] buffer = {(byte) (bits & 0xff),
                (byte) ((bits >> 8) & 0xff),
                (byte) ((bits >> 16) & 0xff),
                (byte) ((bits >> 24) & 0xff),
                (byte) ((bits >> 32) & 0xff),
                (byte) ((bits >> 40) & 0xff),
                (byte) ((bits >> 48) & 0xff),
                (byte) ((bits >> 56) & 0xff)};
        return buffer;
    }

    /** converts an array of doubles to an array of bytes */
    public static final byte[] doubleArray2Bytes(double[] inArray) {
        int j = 0;
        int length = inArray.length;
        byte[] out = new byte[length * Double.BYTES];
        for (int i = 0; i < length; i++) {
            long bits = Double.doubleToLongBits(inArray[i]);
            out[j++] = (byte) (bits & 0xff);
            out[j++] = (byte) ((bits >> 8) & 0xff);
            out[j++] = (byte) ((bits >> 16) & 0xff);
            out[j++] = (byte) ((bits >> 24) & 0xff);
            out[j++] = (byte) ((bits >> 32) & 0xff);
            out[j++] = (byte) ((bits >> 40) & 0xff);
            out[j++] = (byte) ((bits >> 48) & 0xff);
            out[j++] = (byte) ((bits >> 56) & 0xff);
        }
        return out;
    }

    /** converts an array of bytes to a double */
    public static final double bytesArray2Double(byte[] input) {
        System.out.println(Arrays.toString(input) + "\n");
        byte[] reverse = new byte[]{input[7], input[6], input[5], input[4], input[3], input[2], input[1], input[0]};
        return ByteBuffer.wrap(reverse).getDouble();
    }

    public void processLocation(android.location.Location location) {
        recentLocation = location;

        Log.d("location", "Lat: " + recentLocation.getLatitude() + " Lon: " + recentLocation.getLongitude() + " alt: " + recentLocation.getAltitude() + " hdg: " + main.magObject.heading + " acc: " + recentLocation.getAccuracy());
        main.update.updateConversationHandler.post(new UpdateTextThread(main.textFeedback, "Lat: " + recentLocation.getLatitude() + " Lon: " + recentLocation.getLongitude(), false));
/*
        8 x 3 = 24
        8 x 4 = 32
*/

/*
        byte[] temp = doubleArray2Bytes(data);
        String tempString = "";
        for (short i = 0; i < temp.length; i++)
        {
            tempString += "["+temp[i]+"]";
            if (i!=temp.length-1)
            {tempString += " ";}
        }
        System.out.println(tempString);
*/

/*
        double[] data = {recentLocation.getLatitude(), recentLocation.getLongitude(), recentLocation.getAltitude()};
        String base64string = android.util.Base64.encodeToString(doubleArray2Bytes(data), Base64.URL_SAFE | Base64.NO_WRAP);
        Log.d("location", "base64: " + base64string);
*/
        main.mavLink.sendGlobalPosition(recentLocation.getLatitude(), recentLocation.getLongitude(), recentLocation.getAltitude());
    }

    public void startLocation() {
        locationManager = (LocationManager) main.getSystemService(Context.LOCATION_SERVICE);
        TelephonyManager telemamanger = (TelephonyManager) main.getSystemService(main.TELEPHONY_SERVICE);

/*
        try {
            deviceIdentifier = telemamanger.getLine1Number();
        } catch (Exception e) {
            e.printStackTrace();
        }
*/
        try {
            deviceIdentifier = telemamanger.getDeviceId();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (deviceIdentifier == null) {
            deviceIdentifier = "N/A";
        }

        LocationListener locationListener = new LocationListener() {
            @Override
            public void onLocationChanged(android.location.Location location) {
                processLocation(location);
            }

            public void onStatusChanged(String provider, int status, Bundle extras) {
            }

            public void onProviderEnabled(String provider) {
            }

            public void onProviderDisabled(String provider) {
            }
        };

        if (ContextCompat.checkSelfPermission(main, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000, 1, locationListener);
        }
    }

    public void printFlightpath() {
        try {
            Log.d(TAG, "waypoints read: " + route.size());
            for (Waypoint item : route) {
                Log.d(TAG, item.lat + "\u00B0 " + item.lon + "\u00B0 " + item.ele + "m");
            }
        } catch (Exception e) {
            Log.d(TAG, e.toString());
        }
    }

    class Waypoint {
        public double lat;
        public double lon;
        public double ele;

        // constructor with latitude and longitude
        public Waypoint(double argLat, double argLon) {
            super();
            this.lat = argLat;
            this.lon = argLon;
        }

        // constructor with latitude, longitude and elevation
        public Waypoint(double argLat, double argLon, double argElevation) {
            super();
            this.lat = argLat;
            this.lon = argLon;
            this.ele = argElevation;
        }
    }

    class Wrap implements Runnable {
        @Override
        public void run() {
            double[] coordinates = {recentLocation.getLatitude(), recentLocation.getLongitude()};
            try {
                //main.sendTelemetry(2, coordinates);
            } catch (Exception e) {
                Log.d("Barometer", "error: " + e);
                //main.logObject.saveComment("error: " + e.toString());
            }
        }
    }
}
