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
public class Engine implements MessageHandler {
    private MessageManager mh;
    private EntityManager em;
    private boolean run;
    
    private Engine() {
    }
    
    public void startup() {
        this.mh = new MessageManager();
        Messages.getInstance().RegisterMessages(this.mh);
        this.mh.registerHandler(Messages.getInstance().SHUTDOWN(), this);
        this.em = new EntityManager();
        this.run = true;
    }
    
    public void shutdown() {
        
    }
    
    public void doMainLoop() {
        while(this.run) {
            
        }
    }
    
    public MessageManager getMessageManager() {
        return this.mh;
    }
    
    public EntityManager getEntityManager() {
        return this.em;
    }
    
    public static Engine getInstance() {
        return EngineHolder.INSTANCE;
    }

    @Override
    public void handleMessage(Message msg) {
        if(msg.getType() == Messages.getInstance().SHUTDOWN()) {
            this.run = false;
        }
    }
    
    private static class EngineHolder {
        private static final Engine INSTANCE = new Engine();
    }
}
