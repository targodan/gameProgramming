/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package core;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class Program {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Engine eng = Engine.getInstance();
        eng.Startup();
        eng.doMainLoop();
        eng.Shutdown();
    }
    
}
