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
public class Messages extends ReflectionMSGRegisterer {
    private int MSG_SHUTDOWN;
    private int MSG_ENTITY_ADD_COMPONENT;
    private int MSG_ENTITY_REMOVE_COMPONENT;
    
    private Messages() {
    }

    public int ENTITY_ADD_COMPONENT() {
        return MSG_ENTITY_ADD_COMPONENT;
    }

    public int ENTITY_REMOVE_COMPONENT() {
        return MSG_ENTITY_REMOVE_COMPONENT;
    }

    public int SHUTDOWN() {
        return MSG_SHUTDOWN;
    }
    
    public static Messages getInstance() {
        return MessagesHolder.INSTANCE;
    }
    
    private static class MessagesHolder {
        private static final Messages INSTANCE = new Messages();
    }
}
