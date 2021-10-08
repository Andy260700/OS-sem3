package com.arkadeepde;

import java.io.File;
import java.util.*;

public class FCFSScheduler implements Scheduler {

    private PriorityQueue<Job> priorityQueue;
    private Map<Integer, Integer> endingTimeMap;
    private Map<Integer, Integer> currentBurstPosition;

    public FCFSScheduler(Scanner sc) {
        priorityQueue = new PriorityQueue<>(new Comparator<Job>() {
            @Override
            public int compare(Job o1, Job o2) {
                if (o1.jobID == o2.jobID)
                    return 0;
                if (o1.arrivalTime < o2.arrivalTime) return 1;
                else if (o1.arrivalTime > o2.arrivalTime) return -1;
                if (o1.arrivalTime == o2.arrivalTime) {
                    if (o1.jobID < o2.jobID) return 1;
                }
                return -1;
            }
        });
        endingTimeMap = new HashMap<>();
        currentBurstPosition = new HashMap<>();

        while(sc.hasNextInt())
        {
            int jobID = sc.nextInt();
            int priority = sc.nextInt();
            ArrayList<Integer> burstTimes = new ArrayList<>();
        }
    }

    @Override
    public void turnaroundTime() {
        System.out.println(endingTimeMap);
    }

    @Override
    public void completionTime() {

    }

    @Override
    public void waitingTime() {

    }

}
