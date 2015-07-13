/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package collisions;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;

/**
 *
 * @author Kosnoros
 */
public class Collisions {

    public Collisions() {

        LineSegment line = new LineSegment(new Vector2(1, 1), new Vector2(10, 10));

        Circle circle1 = new Circle(new Vector2(2, 1), 3);

        ArrayList<LineSegment> lines = new ArrayList<>();
        ArrayList<Circle> circles = new ArrayList<>();
        ArrayList<Double> data = new ArrayList<>();

        double d;
        BufferedReader reader;

        try {
            reader = new BufferedReader(new FileReader("test.txt"));
            String linex = reader.readLine();
            while (linex != null) {

                // get data
                String sub = "";
                int i = 0;
                int j=0;
                String tmp = "";
                while (i < linex.length()) {
                    while (linex.charAt(i) != ' ' && i < linex.length()) {
                        System.out.println("i = " + i);
                        tmp += linex.charAt(i);
                        System.out.println("blbu");
                        i++;
                        System.out.println("i = " + i);
                    }
                   i++;
                    System.out.println("i= " + i);
                  //  data.add(Double.parseDouble(tmp));
                    
                    System.out.println(linex.length());
                    tmp="";
                }

//                System.out.println(linex);
                linex = reader.readLine();
            }

        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }

//        Scanner scanner;
//        File file = new File("test.txt");
//        try {
//            scanner = new Scanner(file);
//            while (scanner.hasNextDouble()) {
//                System.out.println(scanner.nextDouble());
//                
//            }
//
//        } catch (FileNotFoundException e1) {
//            e1.printStackTrace();
//        }
    }

}
