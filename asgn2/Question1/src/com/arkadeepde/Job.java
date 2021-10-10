package com.arkadeepde;


import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;


public class Job {
    public final int jobID;
    public final int arrivalTime;
    public final int priority;
    private LinkedList<Integer> burstTimes;

    public Job(int jobID, int priority, int arrivalTime, LinkedList<Integer> burstTimes) {
        this.jobID = jobID;
        this.arrivalTime = arrivalTime;
        this.priority = priority;
        this.burstTimes = burstTimes;
    }

    public LinkedList<Integer> getBurstTimes() {
        return (LinkedList<Integer>) burstTimes.clone();
    }

    @Override
    public String toString() {
        return "Job{" +
                "jobID=" + jobID +
                ", arrivalTime=" + arrivalTime +
                ", priority=" + priority +
                ", burstTimes=" + burstTimes +
                '}';
    }
}
