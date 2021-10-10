package com.arkadeepde;

import java.io.File;
import java.util.*;

public class FCFSScheduler implements Scheduler {

    private ArrayList<Job> jobList;
    private Map<Integer, Integer> endingTimeMap;

    public FCFSScheduler(ArrayList<Job> jobs) {
        PriorityQueue<Job> priorityQueue = new PriorityQueue<>((o1, o2) -> {
            if (o1.jobID == o2.jobID)
                return 0;
            if (o1.arrivalTime < o2.arrivalTime) return -1;
            else if (o1.arrivalTime > o2.arrivalTime) return 1;
            else {
                if (o1.jobID < o2.jobID) return -1;
            }
            return 1;
        });
        endingTimeMap = new HashMap<>();
        jobList = jobs;

        priorityQueue.addAll(jobList);

        int time = 0;
        while(!priorityQueue.isEmpty()) {
            Job job = priorityQueue.poll();
//            System.out.println(job);
            time = Math.max(time, job.arrivalTime);
            var burstTimes = job.getBurstTimes();
            int cpuBurst = burstTimes.removeFirst();
            if(burstTimes.isEmpty()) {
                endingTimeMap.put(job.jobID, time + cpuBurst);
            } else {
                int ioBurst = burstTimes.removeFirst();
                priorityQueue.add(new Job(job.jobID, job.priority, time + cpuBurst + ioBurst, burstTimes));
            }
            time += cpuBurst;
        }
    }

    @Override
    public void turnaroundTime() {
        System.out.println("Job ID\tTAT");
        for(var job : jobList)
        {
            System.out.printf("%d\t\t%d\n", job.jobID, endingTimeMap.get(job.jobID) - job.arrivalTime);
        }
    }

    @Override
    public void completionTime() {
        System.out.println("Job ID\tET");
        for(var job : jobList)
        {
            System.out.printf("%d\t\t%d\n", job.jobID, endingTimeMap.get(job.jobID));
        }
    }

    @Override
    public void waitingTime() {
        System.out.println("Job ID\tWT");
        for(var job : jobList)
        {
            System.out.printf("%d\t\t%d\n", job.jobID, endingTimeMap.get(job.jobID) - job.arrivalTime - job.getBurstTimes().parallelStream().reduce(0, (a, b) -> a + b));
        }
    }

}
