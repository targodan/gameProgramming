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
public class Math3D {
    public static Vector3D mul(Vector3D v, double f) {
        Vector3D ret = v.clone();
        ret.mul(f);
        return ret;
    }
    
    public static Vector3D mul(Matrix3x3 m, Vector3D v) {
        Vector3D ret = new Vector3D();
        
        int row = 0;
        ret.set(row, m.get(row, 0) * v.get(0) + m.get(row, 1) * v.get(1) + m.get(row, 2) * v.get(2));
        row = 1;
        ret.set(row, m.get(row, 0) * v.get(0) + m.get(row, 1) * v.get(1) + m.get(row, 2) * v.get(2));
        row = 2;
        ret.set(row, m.get(row, 0) * v.get(0) + m.get(row, 1) * v.get(1) + m.get(row, 2) * v.get(2));
        
        return ret;
    }
    
    public static Matrix3x3 mul(Matrix3x3 a, Matrix3x3 b) {
        Matrix3x3 ret = new Matrix3x3();
        
        int row = 0;
        int col = 0;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 1;
        col = 0;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 2;
        col = 0;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 0;
        col = 1;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 1;
        col = 1;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 2;
        col = 1;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 0;
        col = 2;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 1;
        col = 2;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        row = 2;
        col = 2;
        ret.set(row, col, a.get(row, 0) * b.get(0, col) + a.get(row, 1) * b.get(1, col) + a.get(row, 2) * b.get(2, col));
        
        return ret;
    }
    
    public static Matrix3x3 transpose(Matrix3x3 m) {
        Matrix3x3 ret = new Matrix3x3(m.elems);
        
        int row = 0;
        int col = 1;
        ret.set(row, col, m.get(col, row));
        row = 0;
        col = 2;
        ret.set(row, col, m.get(col, row));
        row = 1;
        col = 0;
        ret.set(row, col, m.get(col, row));
        row = 1;
        col = 2;
        ret.set(row, col, m.get(col, row));
        row = 2;
        col = 0;
        ret.set(row, col, m.get(col, row));
        row = 2;
        col = 1;
        ret.set(row, col, m.get(col, row));
        
        return ret;
    }
}
