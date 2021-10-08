package com.arkadeepde;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        try(Scanner sc = new Scanner(new File("files/input.txt"))) {
            Scheduler scheduler = new FCFSScheduler(sc);
            scheduler.waitingTime();
            scheduler.completionTime();
            scheduler.turnaroundTime();
        } catch (IOException e){
            System.out.println("Issue with file");
        }

    }
}
