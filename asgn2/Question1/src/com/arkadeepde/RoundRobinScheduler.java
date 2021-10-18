package com.arkadeepde;

import java.util.*;

public class RoundRobinScheduler implements Scheduler {

    static final int TIME_QUANTUM = 25;
    private ArrayList<Job> jobList;
    private Map<Integer, Integer> endingTimeMap;


    public RoundRobinScheduler(ArrayList<Job> jobs) {
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

        Queue<Job> roundRobinQueue = new LinkedList<>();

        arrivalQueue.addAll(jobs);

        if (!arrivalQueue.isEmpty()) {
            int time = 0;

            do {

                if (roundRobinQueue.isEmpty()) {
                    time = Math.max(time, arrivalQueue.peek().arrivalTime);
                }

                while (!arrivalQueue.isEmpty() && arrivalQueue.peek().arrivalTime <= time) {
                    roundRobinQueue.add(arrivalQueue.poll());
                }


                Job job = roundRobinQueue.poll();

                var burstTimes = job.getBurstTimes();
                int cpuBurst = burstTimes.removeFirst();

                if (cpuBurst > TIME_QUANTUM) {
                    time += TIME_QUANTUM;
                    burstTimes.addFirst(cpuBurst - TIME_QUANTUM);
                    roundRobinQueue.add(new Job(job.jobID, job.priority, job.arrivalTime, burstTimes));
                } else if (burstTimes.isEmpty()) {
                    endingTimeMap.put(job.jobID, time + cpuBurst);
                    time += cpuBurst;
                } else {
                    int ioBurst = burstTimes.removeFirst();
                    arrivalQueue.add(new Job(job.jobID, job.priority, time + cpuBurst + ioBurst, burstTimes));
                    time += cpuBurst;
                }

            } while (!(arrivalQueue.isEmpty() && roundRobinQueue.isEmpty()));
        }
    }


    @Override
    public void turnaroundTime() {
        System.out.println("Job ID\tTAT");
        for (var job : jobList) {
            System.out.printf("%d\t\t%d\n", job.jobID, endingTimeMap.get(job.jobID) - job.arrivalTime);
        }
    }

    @Override
    public void completionTime() {
        System.out.println("Job ID\tET");
        for (var job : jobList) {
            System.out.printf("%d\t\t%d\n", job.jobID, endingTimeMap.get(job.jobID));
        }
    }

    @Override
    public void waitingTime() {
        System.out.println("Job ID\tWT");
        for (var job : jobList) {
            System.out.printf("%d\t\t%d\n", job.jobID, endingTimeMap.get(job.jobID) - job.arrivalTime - job.getBurstTimes().parallelStream().reduce(0, (a, b) -> a + b));
        }
    }
}
