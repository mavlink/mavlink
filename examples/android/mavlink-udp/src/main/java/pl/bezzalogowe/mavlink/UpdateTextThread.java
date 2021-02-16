package pl.bezzalogowe.mavlink;

import android.animation.ArgbEvaluator;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.graphics.Color;
import android.os.Handler;
import android.util.Log;
import android.widget.TextView;

public class UpdateTextThread implements Runnable {
    private final static String TAG = UpdateTextThread.class.getName();
    public Handler updateConversationHandler;
    private TextView view;
    private String msg;
    private boolean fancy;

    public UpdateTextThread() {
    }

    public UpdateTextThread(TextView element, String str, boolean blink) {
        this.view = element;
        this.msg = str;
        this.fancy = blink;
    }

    @Override
    public void run() {
        try {
            view.setText(msg);
            if (fancy) {
                manageBlinkEffect();
            }
            Log.d(TAG, msg);
        } catch (Exception e) {
            e.printStackTrace();
            Log.d(TAG, "Error: " + e);
        }
    }

    /** https://medium.com/@ssaurel/create-a-blink-effect-on-android-3c76b5e0e36b */
    private void manageBlinkEffect() {
/* Ground station sends messages with the same content,
   a blink emphasizes that it's a new message. */
        ObjectAnimator anim = ObjectAnimator.ofInt(view, "textColor", Color.BLACK, Color.BLUE);
        anim.setDuration(500);
        anim.setEvaluator(new ArgbEvaluator());
        anim.setRepeatMode(ValueAnimator.REVERSE);
        anim.setRepeatCount(1);
        anim.start();
    }
}
