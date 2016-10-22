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
    protected final Set<String> messages;
    protected final Set<Integer> hashes;
    protected final Map<Integer, Set<MessageHandler>> handlers;
    protected final Queue<Message> queue;
    
    public MessageManager() {
        this.messages = new HashSet<>();
        this.hashes = new HashSet<>();
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
    
    public boolean isRegistered(String name) {
        return this.messages.contains(name);
    }
    
    public void registerHandler(MessageHandler h, int msg) {
        if(!this.hashes.contains(msg)) {
            throw new IllegalArgumentException("Message type unknown.");
        }
        this.handlers.get(msg).add(h);
    }
    
    public void registerHandler(MessageHandler h, Collection<Integer> msges) {
        msges.stream().forEach(msg -> {
            this.registerHandler(h, msg);
        });
    }
    
    public void registerHandler(MessageHandler h, int... msges) {
        for(int msg : msges) {
            this.registerHandler(h, msg);
        }
    }
    
    // TODO: where does the queue come in?
    // *when* are messages handled?
    public void dispatch(Message msg) {
        this.handlers.get(msg.getType()).stream().forEach(h -> {
            h.handleMessage(msg);
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
