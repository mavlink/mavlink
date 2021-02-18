package pl.bezzalogowe.mavlink;

import android.util.Log;

public class MAVLink {
    private final static String TAG = MAVLink.class.getName();

    /* Used to load the native library on application startup. */
    static {
        System.loadLibrary("mavlink_udp");
    }

    MainActivity main;

    public MAVLink(final MainActivity argActivity) {
        main = argActivity;
    }

    public static native void classInit();

    public static native void setGroundStationIP(String ip);

    public native int heartBeatInit();

    public native int heartBeatStop();

    /* Displays a string from C code */
    public native String stringFromJNI();

    public native int sendProtocol();

    public native int sendHello();

    public native void setHeadingDegrees(double hdg);

    public native void sendAttitude(float roll, float pitch/*, float heading*/);

    public native void setBattery(int voltage, int level);

    public native void sendGlobalPosition(double lat, double lon, double alt);

    /* Called from native code. This sets the content of the TextView from the UI thread. */
    private void setMessage(final String message, boolean blink) {
        main.update.updateConversationHandler.post(new UpdateTextThread(main.textFeedback, message, blink));
    }

    private void setButtons(final String message, boolean blink) {
        main.update.updateConversationHandler.post(new UpdateTextThread(main.textButtons, message, blink));
    }

    private void setLog(final String message) {
        Log.d(TAG, message);
        //System.out.println(message);
    }

    private void setProgress(short x, short y, short z, short r) {
        /* pitch, roll, thrust, yaw */
        main.update.updateConversationHandler.post(new UpdateProgressThread(main.seekbar1, x / 20 + 50));
        main.update.updateConversationHandler.post(new UpdateTextThread(main.dutyCycleTextX, Integer.toString(x), false));

        main.update.updateConversationHandler.post(new UpdateProgressThread(main.seekbar2, y / 20 + 50));
        main.update.updateConversationHandler.post(new UpdateTextThread(main.dutyCycleTextY, Integer.toString(y), false));

        main.update.updateConversationHandler.post(new UpdateProgressThread(main.seekbar3, z / 20 + 50));
        main.update.updateConversationHandler.post(new UpdateTextThread(main.dutyCycleTextZ, Integer.toString(z), false));

        main.update.updateConversationHandler.post(new UpdateProgressThread(main.seekbar4, r / 20 + 50));
        main.update.updateConversationHandler.post(new UpdateTextThread(main.dutyCycleTextR, Integer.toString(r), false));
    }
}
