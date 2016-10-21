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
public class Matrix3x3 {
    protected double[] elems;
    
    public Matrix3x3() {
        this.elems = new double[9];
    }
    
    public Matrix3x3(double[] elems) {
        if(elems.length != 9) {
            throw new IllegalArgumentException("A 3x3 matrix has to consist of 9 entries.");
        }
        this.elems = elems.clone();
    }

    public double get(int row, int col) {
        return this.elems[col+row*3];
    }
    
    public Matrix3x3 set(int row, int col, double val) {
        this.elems[col+row*3] = val;
        return this;
    }
    
    public double determinant() {
        return this.get(0, 0) * this.get(1, 1) * this.get(2, 2)
                + this.get(1, 0) * this.get(2, 1) * this.get(0, 2)
                + this.get(0, 2) * this.get(0, 1) * this.get(1, 2)
                - this.get(2, 0) * this.get(1, 1) * this.get(0, 2)
                - this.get(1, 0) * this.get(0, 1) * this.get(2, 2)
                - this.get(0, 0) * this.get(2, 1) * this.get(1, 2);
    }
    
    public Matrix3x3 mul(double d) {
        this.elems[0] *= d;
        this.elems[1] *= d;
        this.elems[2] *= d;
        this.elems[3] *= d;
        this.elems[4] *= d;
        this.elems[5] *= d;
        this.elems[6] *= d;
        this.elems[7] *= d;
        this.elems[8] *= d;
        this.elems[9] *= d;
        
        return this;
    }
    
    public Matrix3x3 div(double d) {
        this.elems[0] /= d;
        this.elems[1] /= d;
        this.elems[2] /= d;
        this.elems[3] /= d;
        this.elems[4] /= d;
        this.elems[5] /= d;
        this.elems[6] /= d;
        this.elems[7] /= d;
        this.elems[8] /= d;
        this.elems[9] /= d;
        
        return this;
    }
}
