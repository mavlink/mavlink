package pl.bezzalogowe.mavlink;

import android.Manifest;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import pl.bezzalogowe.PhoneUAV.Gravity;
import pl.bezzalogowe.PhoneUAV.Location;
import pl.bezzalogowe.PhoneUAV.Magnetometer;

public class MainActivity extends AppCompatActivity implements SensorEventListener {
    private final static String TAG = MainActivity.class.getName();

    final MainActivity main = this;
    public UpdateTextThread update;
    public UpdateProgressThread updateProgress;

    public TextView address, textFeedback, textButtons;
    public Gravity gravityObject = new Gravity(this);
    public Magnetometer magObject = new Magnetometer(this);
    public Location locObject = new Location(this);
    public MAVLink mavLink = new MAVLink(this);
    public Network networkObject = new Network();
    Button startHeartbeat, stopHeartbeat;
    SeekBar seekbar1, seekbar2, seekbar3, seekbar4;
    TextView dutyCycleTextX, dutyCycleTextY, dutyCycleTextZ, dutyCycleTextR;
    String groundStationIP;

    /** https://stackoverflow.com/questions/3291655/get-battery-level-and-state-in-android */
    private BroadcastReceiver mBatInfoReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context ctxt, Intent intent) {
            /** sending battery voltage info */
            int voltage = intent.getIntExtra(BatteryManager.EXTRA_VOLTAGE, 0); // Millivolts
            int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0); // percent
            mavLink.setBattery(voltage, level);
        }
    };

    public static boolean validate(final String ip) {
        final String PATTERN = "^(([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.){3}([01]?\\d\\d?|2[0-4]\\d|25[0-5])$";
        Pattern pattern = Pattern.compile(PATTERN);
        Matcher matcher = pattern.matcher(ip);
        return matcher.matches();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        seekbar1 = (SeekBar) findViewById(R.id.Seekbar1);
        dutyCycleTextX = (TextView) findViewById(R.id.DutyCycleTextX);

        seekbar2 = (SeekBar) findViewById(R.id.Seekbar2);
        dutyCycleTextY = (TextView) findViewById(R.id.DutyCycleTextY);

        seekbar3 = (SeekBar) findViewById(R.id.Seekbar3);
        dutyCycleTextZ = (TextView) findViewById(R.id.DutyCycleTextZ);

        seekbar4 = (SeekBar) findViewById(R.id.Seekbar4);
        dutyCycleTextR = (TextView) findViewById(R.id.DutyCycleTextR);

        if (Build.VERSION.SDK_INT >= 23) {
            askPermissions();
        }

        readPreferences();
        mavLink.classInit();

        address = findViewById(R.id.address);
        address.setText(networkObject.getLocalIpAddress());

        textFeedback = findViewById(R.id.text_feedback);
        textFeedback.setTextColor(Color.BLACK);
        textButtons = findViewById(R.id.text_buttons);
        textButtons.setTextColor(Color.BLACK);

        startHeartbeat = findViewById(R.id.start_heartbeat);
        stopHeartbeat = findViewById(R.id.stop_heartbeat);

        update = new UpdateTextThread();
        update.updateConversationHandler = new Handler();

        updateProgress = new UpdateProgressThread();
        updateProgress.updateConversationHandler = new Handler();

        gravityObject.startGravity();
        magObject.startMagnetometer();
        locObject.startLocation();

        this.registerReceiver(this.mBatInfoReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));

        startHeartbeat.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LOCKED);
/*
String ip = address.getText().toString();
if (validate(ip)) {
    mavLink.setGroundStationIP(ip);
    saveReceiverIP();
}
*/
                    mavLink.heartBeatInit();

                    /* doesn't allow starting thread twice */
                    startHeartbeat.setVisibility(View.GONE);
                    stopHeartbeat.setVisibility(View.VISIBLE);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LOCKED);
                main.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
        });

        stopHeartbeat.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mavLink.heartBeatStop();
                main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
                stopHeartbeat.setVisibility(View.GONE);
                startHeartbeat.setVisibility(View.VISIBLE);

                main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
                main.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
        });
    }

    @Override
    public void onBackPressed() {
        new AlertDialog.Builder(this).setIcon(android.R.drawable.ic_menu_close_clear_cancel)
                .setTitle("Close application?")
                .setMessage("Confirm closing application:")
                .setPositiveButton("yes", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        mavLink.heartBeatStop();
                        finish();
                        System.exit(0);
                    }
                }).setNegativeButton("no", null).show();
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        switch (event.sensor.getType()) {
            case Sensor.TYPE_ACCELEROMETER:
                //TODO: process accelerometer event
                break;
            case Sensor.TYPE_GRAVITY:
                gravityObject.processGravity(event);
                break;
            case Sensor.TYPE_MAGNETIC_FIELD:
                magObject.calculateMagnetometer(event);
                break;
            case Sensor.TYPE_PRESSURE:
                //TODO: process barometric pressure sensor event
                break;
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // do nothing
    }

    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main_menu, menu);
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        /* Handle menu item selection */
        switch (item.getItemId()) {
            case R.id.address:
                networkObject.showIP(this);
                break;
            case R.id.info:
                networkObject.showInfo(this);
                break;
            case R.id.other_apps:
                openOtherAppsPage();
                break;
            case R.id.source_code:
                openSourceCodePage();
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    private void readPreferences() {
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
        groundStationIP = settings.getString("ground-station-ip", "192.168.0.100");
        Log.d("preferences read", "ground-station-ip" + groundStationIP);
    }

    private void saveReceiverIP() {
        final EditText addressEditText = (EditText) this.findViewById(R.id.address);
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString("ground-station-ip", addressEditText.getText().toString());
        editor.commit();
    }

    protected void askPermissions() {
        String[] permissions = {
                Manifest.permission.ACCESS_COARSE_LOCATION,
                Manifest.permission.ACCESS_FINE_LOCATION,
                Manifest.permission.READ_PHONE_STATE
        };

        int requestCode = 200;
        ActivityCompat.requestPermissions(this, permissions, requestCode);
    }

    /* checks if device is online */
    private boolean isNetworkAvailable() {
        ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(main.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
        return activeNetworkInfo != null && activeNetworkInfo.isConnected();
    }

    public void openOtherAppsPage() {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/developer?id=bezzalogowe.pl"));
        startActivity(browserIntent);
    }

    public void openSourceCodePage() {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://github.com/mareksuma1985/MAVLink"));
        startActivity(browserIntent);
    }
}
