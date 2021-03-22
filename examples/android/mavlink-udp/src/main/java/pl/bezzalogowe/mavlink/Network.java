package pl.bezzalogowe.mavlink;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.net.ConnectivityManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Network {
    MainActivity main;
    /* must be declared here, otherwise won't be accessible in GetHostNameThread */
    String hostname, hostnameCanonical;

    public Network(MainActivity argActivity) {
        main = argActivity;
    }

    public static InetAddress getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements(); ) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf
                        .getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress()
                            && inetAddress instanceof Inet4Address) {
                        return inetAddress;
                    }
                }
            }
        } catch (SocketException ex) {
            ex.printStackTrace();
        }
        return null;
    }

    public static String getLocalIpAddressString() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements(); ) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf
                        .getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress()
                            && inetAddress instanceof Inet4Address) {
                        return inetAddress.getHostAddress();
                    }
                }
            }
        } catch (SocketException ex) {
            ex.printStackTrace();
        }
        return null;
    }

    public void showIP(MainActivity arg) {
        AlertDialog.Builder helpBuilder = new AlertDialog.Builder(arg);
        helpBuilder.setIcon(android.R.drawable.ic_dialog_info)
                .setTitle(arg.getResources().getString(R.string.show_ip)).setMessage("IP address: " + getLocalIpAddressString())
                .setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                });
        AlertDialog helpDialog = helpBuilder.create();
        helpDialog.show();
    }

    public void showNetworkInfo(MainActivity arg) {
        /* general network information */
        AlertDialog.Builder helpBuilder = new AlertDialog.Builder(main);
        helpBuilder.setIcon(android.R.drawable.ic_dialog_info);

        /** Checking host names must be done on separate thread */
        Thread delayed = new Thread(new GetHostNameThread());
        delayed.start();
        try {
            delayed.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        helpBuilder.setTitle(main.getResources().getString(R.string.show_network_info));

        try {
            helpBuilder.setMessage("hostname: " + hostname +
                    "\r\ncanonical: " + hostnameCanonical);
            helpBuilder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                }
            });
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }

        AlertDialog helpDialog = helpBuilder.create();
        helpDialog.show();
    }

    public void showWiFiInfo(MainActivity arg) {
        /* WiFi information */
        AlertDialog.Builder helpBuilder = new AlertDialog.Builder(main);
        helpBuilder.setIcon(android.R.drawable.ic_dialog_info);
        WifiManager wifi;
        WifiInfo info;

        String frequency, pattern;

        int channelWLAN;

        /** Checking host names must be done on separate thread */
        Thread delayed = new Thread(new GetHostNameThread());
        delayed.start();
        try {
            delayed.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        helpBuilder.setTitle(main.getResources().getString(R.string.show_wifi_info));

        try {
            wifi = (WifiManager) main.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            info = wifi.getConnectionInfo();

            if (Build.VERSION.SDK_INT <= 20) {
                pattern = "Frequency:\\s*(.*)";
                Pattern r = Pattern.compile(pattern);
                Matcher m = r.matcher(info.toString());
                if (m.find()) {
                    frequency = m.group(1);
                } else {
                    frequency = "";
                }

                /** http://en.wikipedia.org/wiki/List_of_WLAN_channels */
                channelWLAN = ((Integer.valueOf(frequency) - 2412) / 5) + 1;

                if (channelWLAN < 0)
                    channelWLAN = 0;
                helpBuilder.setMessage("hostname: " + hostname +
                        "\r\ncanonical: " + hostnameCanonical +
                        "\r\nWi-Fi channel: " + channelWLAN + info.toString().replace(", ", "\n"));
            } else {
                helpBuilder.setMessage("hostname: " + hostname +
                        "\r\ncanonical: " + hostnameCanonical +
                        "\r\nWi-Fi frequency: " + info.getFrequency() + " " + info.FREQUENCY_UNITS +
                        "\r\nstrength: " + info.getRssi() + " dBm" +
                        "\r\nSSID: " + info.getSSID());
            }

            helpBuilder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                }
            });
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }

        AlertDialog helpDialog = helpBuilder.create();
        helpDialog.show();

    }

    public int connected() {
        /** https://stackoverflow.com/questions/5027890/check-gprs-connection */
        int connected = 0;

        final ConnectivityManager connMgr = (ConnectivityManager) main.getSystemService(Context.CONNECTIVITY_SERVICE);

        final WifiManager wifiManager = (WifiManager) main.getApplicationContext().getSystemService(Context.WIFI_SERVICE);

        if (wifiManager.isWifiEnabled()) {
            connected += 2;
        }

        final android.net.NetworkInfo mobile = connMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);

        if (mobile.isAvailable()) {
            connected += 1;
        }

        return connected;
    }

    class GetHostNameThread implements Runnable {
        public void run() {
            InetAddress ia = getLocalIpAddress();
            hostname = ia.getHostName();
            hostnameCanonical = ia.getCanonicalHostName();
        }
    }
}
