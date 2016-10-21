/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package stuff;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class Vector3D {
    protected double[] coords;
    
    public Vector3D() {
        this.coords = new double[3];
    }
    
    public Vector3D(double[] coords) {
        if(coords.length != 3) {
            throw new IllegalArgumentException("A 3D vector has to consist of 3 entries.");
        }
        this.coords = coords.clone();
    }
    
    public double get(int i) {
        return this.coords[i];
    }
    
    public double getX() {
        return this.coords[0];
    }
    
    public double getY() {
        return this.coords[1];
    }
    
    public double getZ() {
        return this.coords[2];
    }
    
    public Vector3D set(int i, double v) {
        this.coords[i] = v;
        
        return this;
    }
    
    public Vector3D setX(double x) {
        this.coords[0] = x;
        
        return this;
    }
    
    public Vector3D setY(double y) {
        this.coords[1] = y;
        
        return this;
    }
    
    public Vector3D setZ(double z) {
        this.coords[2] = z;
        
        return this;
    }
    
    public Vector3D add(Vector3D vec) {
        this.coords[0] += vec.coords[0];
        this.coords[1] += vec.coords[1];
        this.coords[2] += vec.coords[2];
        
        return this;
    }
    
    public Vector3D sub(Vector3D vec) {
        this.coords[0] -= vec.coords[0];
        this.coords[1] -= vec.coords[1];
        this.coords[2] -= vec.coords[2];
        
        return this;
    }
    
    public Vector3D mul(Vector3D vec) {
        this.coords[0] *= vec.coords[0];
        this.coords[1] *= vec.coords[1];
        this.coords[2] *= vec.coords[2];
        
        return this;
    }
    
    public Vector3D div(Vector3D vec) {
        this.coords[0] /= vec.coords[0];
        this.coords[1] /= vec.coords[1];
        this.coords[2] /= vec.coords[2];
        
        return this;
    }
    
    public Vector3D mul(double d) {
        this.coords[0] *= d;
        this.coords[1] *= d;
        this.coords[2] *= d;
        
        return this;
    }
    
    public Vector3D div(double d) {
        this.coords[0] /= d;
        this.coords[1] /= d;
        this.coords[2] /= d;
        
        return this;
    }
    
    public double lengthSquare() {
        return this.coords[0] * this.coords[0] + this.coords[1] * this.coords[1] + this.coords[2] * this.coords[2];
    }
    
    public double length() {
        return Math.sqrt(this.lengthSquare());
    }
    
    public Vector3D normailze() {
        double len = this.length();
        this.coords[0] /= len;
        this.coords[1] /= len;
        this.coords[2] /= len;
        
        return this;
    }
    
    public Vector3D clone() {
        return new Vector3D(this.coords);
    }
}
