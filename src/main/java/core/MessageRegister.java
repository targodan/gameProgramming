/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package core;

import java.lang.reflect.*;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public abstract class MessageRegister {
    public void RegisterMessages(MessageManager manager) {
        for(Field f : this.getClass().getDeclaredFields()) {
            if(f.getName().startsWith("MSG_")) {
                if(!(f.getType().equals(int.class) || f.getType().equals(Integer.class))) {
                    // not int => propably an error
                    throw new WTFException("The field \""+f.getName()+"\" in class \""+
                            this.getClass().getCanonicalName()+"\" is prefixed by \"MSG_\" but is not of type int."+
                            "If that field is not a Message please rename it. If it is a message please make it type int.");
                }
                try {
                    f.setInt(this, manager.registerMessageType(f.getName()));
                } catch(IllegalAccessException e) {
                    // not accessible => propably an error
                    throw new WTFException("The field \""+f.getName()+"\" in class \""+
                            this.getClass().getCanonicalName()+"\" is prefixed by \"MSG_\" but is not accessible."+
                            "If that field is not a Message please rename it. If it is a message please make it protected or more accessible.", e);
                } catch(IllegalArgumentException e) {
                    throw new WTFException("This really should never happen!", e);
                }
            }
        }
    }
}
