package com.arkadeepde;

import java.io.File;
import java.util.*;

public class FCFSScheduler implements Scheduler {

    private ArrayList<Job> jobList;
    private Map<Integer, Integer> endingTimeMap;

    public FCFSScheduler(Scanner sc) {
        PriorityQueue<Job> priorityQueue = new PriorityQueue<>((o1, o2) -> {
            if (o1.jobID == o2.jobID)
                return 0;
            if (o1.arrivalTime < o2.arrivalTime) return 1;
            else if (o1.arrivalTime > o2.arrivalTime) return -1;
            else {
                if (o1.jobID < o2.jobID) return 1;
            }
            return -1;
        });
        endingTimeMap = new HashMap<>();
        jobList = new ArrayList<>();

        while (sc.hasNextInt()) {
            int jobID = sc.nextInt();
            int priority = sc.nextInt();
            int arrivalTime = sc.nextInt();
            LinkedList<Integer> burstTimes = new LinkedList<>();
            while (sc.hasNextInt()) {
                int burstTime = sc.nextInt();
                if(burstTime == -1) break;
                burstTimes.add(burstTime);
            }

            jobList.add(new Job(jobID, priority, arrivalTime, burstTimes));
            priorityQueue.addAll(jobList);
        }
        int time = 0;
        while(!priorityQueue.isEmpty()) {
            Job job = priorityQueue.poll();
            time = Math.max(time, job.arrivalTime);
            var burstTimes = job.getBurstTimes();
            int cpuBurst = burstTimes.removeFirst();
            if(burstTimes.isEmpty()) {
                endingTimeMap.put(job.jobID, time + cpuBurst);
            } else {
                int osBurst = burstTimes.removeFirst();
                priorityQueue.add(new Job(job.jobID, job.priority, time + cpuBurst + osBurst, burstTimes));
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
