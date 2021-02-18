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
    LocationManager locationManager;
    android.location.Location recentLocation;
    String deviceIdentifier = null;
    MainActivity main;

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

        Log.d("location", "lat: " + recentLocation.getLatitude() + " lon: " + recentLocation.getLongitude() + " alt: " + recentLocation.getAltitude() + " hdg: " + main.magObject.heading + " acc: " + recentLocation.getAccuracy());
        main.update.updateConversationHandler.post(new UpdateTextThread(main.textFeedback, "lat: " + recentLocation.getLatitude() + " lon: " + recentLocation.getLongitude(), false));
        main.mavLink.sendGlobalPosition(recentLocation.getLatitude(), recentLocation.getLongitude(), recentLocation.getAltitude());
    }

    public void startLocation() {
        locationManager = (LocationManager) main.getSystemService(Context.LOCATION_SERVICE);
        TelephonyManager telemamanger = (TelephonyManager) main.getSystemService(main.TELEPHONY_SERVICE);

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
}
