package core;


import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.concurrent.LinkedBlockingQueue;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class MessageManager {
    protected Set<String> messages;
    protected Set<Integer> hashes;
    protected Map<Integer, Set<Handler>> handlers;
    protected Queue<Message> queue;
    
    public MessageManager() {
        this.messages = new HashSet<>();
        this.handlers = new HashMap<>();
        // TODO: does this need to be thread safe?
        this.queue = new LinkedBlockingQueue<>();
    }
    
    public int registerMessageType(String name) {
        if(!this.messages.add(name)) {
            throw new IllegalArgumentException("Message name \""+name+"\" is already registered!");
        }
        int hash = this.getHash(name);
        this.hashes.add(hash);
        this.handlers.put(hash, new HashSet<>());
        return hash;
    }
    
    public int getHash(String name) {
        return name.hashCode();
    }
    
    public void registerHandler(int msg, Handler h) {
        if(!this.hashes.contains(msg)) {
            throw new IllegalArgumentException("Message type unknown.");
        }
        this.handlers.get(msg).add(h);
    }
    
    public void registerHandler(Collection<Integer> msges, Handler h) {
        msges.stream().forEach(msg -> {
            this.registerHandler(msg, h);
        });
    }
    
    // TODO: where does the queue come in?
    // *when* are messages handled?
    public void dispatch(Message msg) {
        this.handlers.get(msg.getType()).stream().forEach(h -> {
            h.handle(msg);
        });
    }
    
    public void sendMessage(Message msg) {
        this.queue.add(msg);
    }
    
    public Message recvMessage() {
        return this.queue.remove();
    }
    
    public boolean hasMessages() {
        return !this.queue.isEmpty();
    }
}
