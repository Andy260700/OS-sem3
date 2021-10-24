package com.arkadeepde;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;

public class PriorityScheduler implements Scheduler {

    private ArrayList<Job> jobList;
    private Map<Integer, Integer> endingTimeMap;

    public PriorityScheduler(ArrayList<Job> jobs) {
        jobList = jobs;
        endingTimeMap = new HashMap<>();
        PriorityQueue<Job> arrivalQueue = new PriorityQueue<>((job1, job2) -> {
            if (job1.jobID == job2.jobID)
                return 0;
            if (job1.arrivalTime < job2.arrivalTime) return -1;
            else if (job1.arrivalTime > job2.arrivalTime) return 1;
            else {
                if (job1.jobID < job2.jobID) return -1;
            }
            return 1;
        });

        PriorityQueue<Job> priorityQueue = new PriorityQueue<>((job1 , job2) -> {
            if(job1.priority == job2.priority) {
                if (job1.jobID == job2.jobID)
                    return 0;
                if (job1.arrivalTime < job2.arrivalTime) return -1;
                else if (job1.arrivalTime > job2.arrivalTime) return 1;
                else {
                    if (job1.jobID < job2.jobID) return -1;
                }
                return 1;
            }
            if(job1.priority < job2.priority) return -1;
            return 1;
        });
        arrivalQueue.addAll(jobList);
        if(!arrivalQueue.isEmpty()) {
            int time = 0;

            do {
                if(priorityQueue.isEmpty()) {
                    time = Math.max(time, arrivalQueue.peek().arrivalTime);
                }

                while (!arrivalQueue.isEmpty() && arrivalQueue.peek().arrivalTime <= time) {
                    priorityQueue.add(arrivalQueue.poll());
                }
                Job job = priorityQueue.poll();


                var burstTimes = job.getBurstTimes();
                int cpuBurst = burstTimes.removeFirst();

                if(burstTimes.isEmpty()) {
                    endingTimeMap.put(job.jobID, time + cpuBurst);
                } else {
                    int ioBurst = burstTimes.removeFirst();
                    arrivalQueue.add(new Job(job.jobID, job.priority, time + cpuBurst + ioBurst, burstTimes));
                }
                time += cpuBurst;

            } while (!(arrivalQueue.isEmpty() && priorityQueue.isEmpty()));
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

    @Override
    public void printTable(){
        System.out.println("Job ID\tEnd Time\tWaiting Time\tTurnaround Time");
        for(var job : jobList){
            System.out.printf("%02d\t\t%05d\t\t%05d\t\t\t%05d\n", job.jobID,
                    endingTimeMap.get(job.jobID),
                    endingTimeMap.get(job.jobID) - job.arrivalTime - job.getBurstTimes().parallelStream().reduce(0, (a, b) -> a + b),
                    endingTimeMap.get(job.jobID) - job.arrivalTime);
        }
    }
}
