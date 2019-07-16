package org.komparator.mediator.ws;

import java.util.TimerTask;

public class MyTimerTask extends TimerTask {

    Object argument;

    public MyTimerTask(Object argument) {
        this.argument = argument;
    }

    @Override
    public void run() {
        System.out.println(this.getClass() + " running...");

        // read argument (any object)
        Integer arg = (Integer) this.argument;

        // Do something...
        System.out.println(arg);

    }

}
