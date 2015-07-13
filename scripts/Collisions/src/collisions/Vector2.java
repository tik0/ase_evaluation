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
public class Vector2 {
    
   public double x;
   public double y;
    public Vector2(double x, double y){
        this.x=x;
        this.y=y;
    }
    
    public void normalize(){
        double length = this.length();
        x/=length;
        y/=length;
    }
    
    public double length(){
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    
    public double scalarP(Vector2 v){
        return pow(x,2) + pow (y,2);
    }
    
    public void set(double x, double y){
        this.x = x;
        this.y = y;
    }

    public Vector2 subtract(Vector2 a){
        return new Vector2(x-a.x, y-a.y);
    }
    public Vector2 add(Vector2 a){
        return new Vector2(x+a.x, y+a.y);
    }
    
    public Vector2 mult(double a){
        return new Vector2(x*a,y*a);
    }
    
    @Override
    public String toString(){
        return "\nVector: \nx: " + x + "\n" + "y: " + y;
    }
}
