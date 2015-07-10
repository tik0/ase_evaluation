/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package collisions;
import static java.lang.Math.*;


/**
 *
 * @author Kosnoros
 */
public class LineSegment {

    Vector2 startpoint;
    Vector2 endpoint;
    Vector2 d;
    double m;
    

    public LineSegment(Vector2 a, Vector2 b) {
       
        this.startpoint = a;
        this.endpoint = b;
        

        this.m = (b.y - a.y) / (b.x - a.x);
//        System.out.println("Line:");
//        System.out.println("m: " + m );
        
        d= b.subtract(a);
//        System.out.println("direction: " + d);
        
    }

    public boolean intersectCirlce(Circle circle) {
           Vector2 diff = startpoint.subtract(circle.center);
           double p = (2*diff.x*d.x + 2*diff.y*d.y)/(pow(d.x,2)+ pow(d.y,2));
           double q = (pow(diff.x,2) + pow(diff.y,2) - pow(circle.r,2))/(pow(d.x,2)+ pow(d.y,2));
           double det = (pow(p,2)/4 - q);
            if (det < 0){
               return false;
           }
            
           double t1 = p/2 + sqrt(det);
           double t2 = p/2 - sqrt(det);
            
           System.out.println("t1: " + t1 + "  t2: " + t2);
           Vector2 schnittpunkt = new Vector2(0, 0);
           schnittpunkt = schnittpunkt.add(startpoint.add(d.mult(t1)));
           Vector2 schnittpunkt2 = new Vector2(0, 0);
           schnittpunkt2 = schnittpunkt2.add(startpoint.add(d.mult(t2)));
           System.out.println("Schnittpunkt1 : " + schnittpunkt );
           System.out.println("Schnittpunkt2 : " + schnittpunkt2 );
           
           if (liesIn(t1,0,1) || liesIn(t2,0,1)){
               return true;
           } else {
               return false;
           }
               
           
          
          
           
    }

    public Vector2 getStartpoint() {
        return startpoint;
    }

    public Vector2 getEndpoint() {
        return endpoint;
    }

    public double getM() {
        return m;
    }
    
    public boolean liesIn(double t, double a, double b){
        if (t>= a && t<=b){
            return true;
        } else{
            return false;
        }
    }
    
    @Override
    public String toString(){
        return "Line with m = " + m;
    }

}
