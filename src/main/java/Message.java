/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class Message {
    protected int type;
    protected Object payload;

    public Message(int type, Object payload) {
        this.type = type;
        this.payload = payload;
    }

    public int getType() {
        return type;
    }

    public Object getPayload() {
        return payload;
    }
}
