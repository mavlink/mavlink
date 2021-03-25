package pl.bezzalogowe.mavlink;

import android.Manifest;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
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
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import pl.bezzalogowe.PhoneUAV.Gravity;
import pl.bezzalogowe.PhoneUAV.Location;
import pl.bezzalogowe.PhoneUAV.Magnetometer;

import static android.text.InputType.TYPE_CLASS_NUMBER;

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
    public Network networkObject = new Network(this);
    SharedPreferences settings;
    Button startServer, stopServer;
    SeekBar seekbar1, seekbar2, seekbar3, seekbar4;
    TextView dutyCycleTextX, dutyCycleTextY, dutyCycleTextZ, dutyCycleTextR, system_id_value, component_id_value;
    TableRow systemIDrow, componentIDrow;
    String groundStationIP;
    int system_id, component_id;

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

        mavLink.classInit();

        address_host = findViewById(R.id.host_address);
        if (networkObject.connected() >= 1) {
            address_host.setText(networkObject.getLocalIpAddressString());
        }

        address_gcs = findViewById(R.id.gcs_address);

        systemIDrow = findViewById(R.id.system_id_row);
        componentIDrow = findViewById(R.id.component_id_row);

        system_id_value = findViewById(R.id.system_id_value);
        component_id_value = findViewById(R.id.component_id_value);

        textFeedback = findViewById(R.id.text_feedback);
        textButtons = findViewById(R.id.text_buttons);

        startServer = findViewById(R.id.start_server);
        stopServer = findViewById(R.id.stop_server);

        readPreferences();

        update = new UpdateTextThread();
        update.updateConversationHandler = new Handler();

        updateProgress = new UpdateProgressThread();
        updateProgress.updateConversationHandler = new Handler();

        final Network networkObject = new Network(this);

        gravityObject.startGravity();
        magObject.startMagnetometer();
        locObject.startLocation();

        this.registerReceiver(this.mBatInfoReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));

        systemIDrow.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                popUpSetSystemID();
            }
        });

        componentIDrow.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                popUpSetComponentID();
            }
        });

        startServer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if (networkObject.connected() >= 1) {
                    /** Wi-Fi or GPRS connection active */
                    try {
                        main.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                        mavLink.receiveInit();
                        mavLink.heartBeatInit();

                        /* doesn't allow starting thread twice */
                        startServer.setVisibility(View.GONE);
                        stopServer.setVisibility(View.VISIBLE);

                        /** displays IP number*/
                        address_host.setText(networkObject.getLocalIpAddressString());

                        /** displays hostname */
                        /*try {
                            InetAddress ia = InetAddress.getByName(networkObject.getLocalIpAddress());
                            address_host.setText(ia.getHostName());
                        } catch (UnknownHostException e) {
                            e.printStackTrace();
                        }*/

                        /** canonical hostname */
                        /*try {
                            InetAddress ia = InetAddress.getByName(networkObject.getLocalIpAddress());
                            address_host.setText(ia.getCanonicalHostName());
                        } catch (UnknownHostException e) {
                            e.printStackTrace();
                        }*/

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } else {
                    /** no connection available */
                    Toast.makeText(getApplicationContext(), "No Wi-Fi nor cellular connection.", Toast.LENGTH_SHORT).show();

                    address_host.setText("-.-.-.-");
                    address_host.setGravity(0x00800005);
                }
            }
        });

        stopServer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                main.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                mavLink.heartBeatStop();
                mavLink.receiveStop();

                stopServer.setVisibility(View.GONE);
                startServer.setVisibility(View.VISIBLE);
            }
        });

        /** start UDP server immediately if device is connected */
        if (networkObject.connected() >= 1) {
            main.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            mavLink.receiveInit();
            mavLink.setIDs((byte) 1, (byte) 1);
            mavLink.heartBeatInit();

            stopServer.setVisibility(View.VISIBLE);
        } else {
            startServer.setVisibility(View.VISIBLE);
        }
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
            case R.id.info:
                if (networkObject.connected() >= 2) {
                    /** Wi-Fi connection active */
                    networkObject.showWiFiInfo(this);
                } else {
                    /** cellular modem connection active */
                    if (networkObject.connected() == 1) {
                        networkObject.showNetworkInfo(this);
                    } else {
                        /** no connection available */
                        Toast.makeText(getApplicationContext(), "No Wi-Fi nor cellular connection.", Toast.LENGTH_SHORT).show();
                    }
                }
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

    /* reads system_id and component_id from preferences */
    private void readPreferences() {
        settings = PreferenceManager.getDefaultSharedPreferences(this);
        system_id = (settings.getInt("system_id", 1)) % 256;
        component_id = (settings.getInt("component_id", 1)) % 256;
        Log.d("preferences read", "system_id: " + system_id + "; " + "component_id: " + component_id);

        system_id_value.setText(Integer.toString(system_id));
        component_id_value.setText(Integer.toString(component_id));
    }

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

    public void openSourceCodePage() {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://github.com/mavlink/mavlink/tree/master/examples/android"));
        startActivity(browserIntent);
    }

    private void popUpSetSystemID() {
        AlertDialog.Builder helpBuilder = new AlertDialog.Builder(this);
        helpBuilder.setTitle(getResources().getString(R.string.system_id));
        helpBuilder.setMessage(getResources().getString(R.string.new_value) + " " + getResources().getString(R.string.system_id) + ":");

        final EditText input = new EditText(this);
        input.setSingleLine();
        input.setInputType(TYPE_CLASS_NUMBER);
        input.setText(Integer.toString(settings.getInt("system_id", 1)));
        helpBuilder.setView(input);
        helpBuilder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {

            public void onClick(DialogInterface dialog, int which) {
                try {
                    system_id = (Integer.valueOf(input.getText().toString())) % 256;
                    SharedPreferences.Editor editor = settings.edit();
                    editor.putInt("system_id", system_id);
                    editor.commit();

                    mavLink.setIDs((byte) system_id, (byte) component_id);
                    system_id_value.setText(Integer.toString(system_id));

                } catch (NumberFormatException e) {
                    Log.d("NumberFormatException", "Error: " + e);
                    e.printStackTrace();
                }
            }
        });

        AlertDialog helpDialog = helpBuilder.create();
        helpDialog.show();
    }

    private void popUpSetComponentID() {
        AlertDialog.Builder helpBuilder = new AlertDialog.Builder(this);
        helpBuilder.setTitle(getResources().getString(R.string.component_id));
        helpBuilder.setMessage(getResources().getString(R.string.new_value) + " " + getResources().getString(R.string.component_id) + ":");

        final EditText input = new EditText(this);
        input.setSingleLine();
        input.setInputType(TYPE_CLASS_NUMBER);
        input.setText(Integer.toString(settings.getInt("component_id", 1)));
        helpBuilder.setView(input);
        helpBuilder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {

            public void onClick(DialogInterface dialog, int which) {
                try {
                    component_id = (Integer.valueOf(input.getText().toString())) % 256;
                    SharedPreferences.Editor editor = settings.edit();
                    editor.putInt("component_id", component_id);
                    editor.commit();

                    mavLink.setIDs((byte) system_id, (byte) component_id);
                    component_id_value.setText(Integer.toString(component_id));

                } catch (NumberFormatException e) {
                    Log.d("NumberFormatException", "Error: " + e);
                    e.printStackTrace();
                }
            }
        });

        AlertDialog helpDialog = helpBuilder.create();
        helpDialog.show();
    }

    /** https://developer.android.com/guide/topics/resources/runtime-changes.html */
    /* prevents app from crashing when screen is rotated */
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        /** Checks the orientation of the screen */
        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
            //Toast.makeText(this, "landscape", Toast.LENGTH_SHORT).show();
        } else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
            //Toast.makeText(this, "portrait", Toast.LENGTH_SHORT).show();
        }
    }
}
