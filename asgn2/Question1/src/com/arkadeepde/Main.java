package com.arkadeepde;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;

public class Main {

    private static ArrayList<Job> parseJobs(Scanner sc) {
        ArrayList<Job> jobs = new ArrayList<>();

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

            jobs.add(new Job(jobID, priority, arrivalTime, burstTimes));
        }
        return jobs;
    }

    public static void main(String[] args) {
        try(Scanner sc = new Scanner(new File("files/input.txt"))) {
            ArrayList<Job> jobs = parseJobs(sc);
//            System.out.println(jobs);
            Scheduler scheduler = new RoundRobinScheduler(jobs);
            scheduler.waitingTime();
            scheduler.completionTime();
            scheduler.turnaroundTime();
        } catch (IOException e){
            System.out.println("Issue with file");
        }

    }
}
