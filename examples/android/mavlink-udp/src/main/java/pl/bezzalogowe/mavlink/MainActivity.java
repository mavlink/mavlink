package pl.bezzalogowe.mavlink;

import android.Manifest;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
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
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
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

    public TextView address_host, address_gcs, textFeedback, textButtons;
    public Gravity gravityObject = new Gravity(this);
    public Magnetometer magObject = new Magnetometer(this);
    public Location locObject = new Location(this);
    public MAVLink mavLink = new MAVLink(this);
    public Network networkObject = new Network();
    Button startServer, stopServer;
    SeekBar seekbar1, seekbar2, seekbar3, seekbar4;
    TextView dutyCycleTextX, dutyCycleTextY, dutyCycleTextZ, dutyCycleTextR;
    String groundStationIP;

    /* https://stackoverflow.com/questions/3291655/get-battery-level-and-state-in-android */
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

        //readPreferences();
        mavLink.classInit();

        address_host = findViewById(R.id.host_address);
        address_host.setText(networkObject.getLocalIpAddress());

        address_gcs = findViewById(R.id.gcs_address);

        textFeedback = findViewById(R.id.text_feedback);
        textButtons = findViewById(R.id.text_buttons);

        startServer = findViewById(R.id.start_server);
        stopServer = findViewById(R.id.stop_server);

        update = new UpdateTextThread();
        update.updateConversationHandler = new Handler();

        updateProgress = new UpdateProgressThread();
        updateProgress.updateConversationHandler = new Handler();

        gravityObject.startGravity();
        magObject.startMagnetometer();
        locObject.startLocation();

        this.registerReceiver(this.mBatInfoReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));

        startServer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LOCKED);
/*
String ip = address_gcs.getText().toString();
if (validate(ip)) {
    mavLink.setGroundStationIP(ip);
    saveReceiverIP();
}
*/
                    mavLink.receiveInit();
                    mavLink.heartBeatInit();
                    /* doesn't allow starting thread twice */
                    startServer.setVisibility(View.GONE);
                    stopServer.setVisibility(View.VISIBLE);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LOCKED);
                main.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
        });

        stopServer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mavLink.heartBeatStop();
                mavLink.receiveStop();
                main.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
                stopServer.setVisibility(View.GONE);
                startServer.setVisibility(View.VISIBLE);

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
                        exit();
                    }
                }).setNegativeButton("no", null).show();
    }

    private void exit() {
        mavLink.heartBeatStop();
        mavLink.receiveStop();
        finish();
        System.exit(0);
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
            /*
            case R.id.address:
                networkObject.showIP(this);
                break;
             */
            case R.id.info:
                networkObject.showInfo(this);
                break;
            case R.id.source_code:
                openSourceCodePage();
                break;
            case R.id.action_exit: {
                exit();
            }
            break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

/* reads remote address from preferences */
/*
    private void readPreferences() {
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
        groundStationIP = settings.getString("ground-station-ip", "192.168.0.100");
        Log.d("preferences read", "ground-station-ip" + groundStationIP);
    }
*/

/* saves remote address in preferences */
/*
    private void saveReceiverIP() {
        final EditText addressEditText = (EditText) this.findViewById(R.id.gcs_address);
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString("ground-station-ip", addressEditText.getText().toString());
        editor.commit();
    }
*/
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

    public void openSourceCodePage() {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://github.com/mavlink/mavlink/tree/master/examples/android"));
        startActivity(browserIntent);
    }
}
