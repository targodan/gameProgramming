/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package core;

/**
 *
 * @author corbatto
 */
public interface Module {
    public Class<? extends Module>[] dependencies();
    public void startup();
    public void shutdown();
}
