package core;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public abstract class Component {
    protected int entityID;
    
    public void setEntityID(int id) {
        this.entityID = id;
    }
    
    public int getEntityID() {
        return this.entityID;
    }
}
