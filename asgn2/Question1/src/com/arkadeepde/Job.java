package com.arkadeepde;

import java.util.ArrayList;

public class Job {
    public final int jobID;
    public final int arrivalTime;
    public final int priority;
    private ArrayList<Integer> burstTimes;

    public Job(int jobID, int arrivalTime, int priority, ArrayList<Integer> burstTimes) {
        this.jobID = jobID;
        this.arrivalTime = arrivalTime;
        this.priority = priority;
        this.burstTimes = burstTimes;
    }

    public ArrayList<Integer> getBurstTimes() {
        return burstTimes;
    }
}
