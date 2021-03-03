package pl.bezzalogowe.mavlink;

import android.widget.SeekBar;
import android.os.Handler;

class UpdateProgressThread implements Runnable {
    public Handler updateConversationHandler;
    private SeekBar view;
    private int pos;

    public UpdateProgressThread() {
    }

    public UpdateProgressThread(SeekBar element, int progress) {
        this.view = element;
        this.pos = progress;
    }

    @Override
    public void run() {
        view.setProgress(pos);
    }
}
