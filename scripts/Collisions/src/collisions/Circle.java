/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package collisions;


/**
 *
 * @author Kosnoros
 */
public class Circle {
    
   public Vector2 center;
   public double r;
    
    public Circle(Vector2 c, double r){
        this.center = c;
        this.r = r;
        
    }
    
    public boolean intersectCircle(Circle circle){
       return circle.center.subtract(center).length() < r+circle.r;
            
        
    }
    
}
