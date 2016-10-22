/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package core;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Stream;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class Engine implements MessageHandler, Module {
    protected MessageManager mh;
    protected EntityManager em;
    protected boolean run;
    protected final Map<Class<? extends Module>, Module> registeredModules;
    protected final List<Module> moduleOrder;
    
    protected Engine() {
        this.registeredModules = new HashMap<>();
        this.moduleOrder = new ArrayList<>();
    }
    
    public Engine registerModule(Class<? extends Module> moduleClass) {
        if(this.registeredModules.containsKey(moduleClass)
                || moduleClass.equals(this.getClass())) {
            // Module already registered.
            return this;
        }
        try {
            Module module;
            try {
                module = moduleClass.newInstance();
            } catch(IllegalAccessException | InstantiationException e) {
                throw new WTFException("A Module must provide a public constructor without any parameters.", e);
            }
            if(module.dependencies() != null) {
                for(Class<? extends Module> m : module.dependencies()) {
                    this.registerModule(m);
                }
            }
            this.registeredModules.put(moduleClass, module);
        } catch(StackOverflowError e) {
            throw new WTFException("Stackoverflow. There propably is a circular dependency in your Modules.", e);
        }
        return this;
    }
    
    public void startup() {
        this.mh = new MessageManager();
        Messages.getInstance().RegisterMessages(this.mh);
        this.mh.registerHandler(this, Messages.getInstance().SHUTDOWN());
        this.em = new EntityManager();
        this.run = true;
        
        this.computeModuleOrder();
        
        this.moduleOrder.forEach((module) -> {
            module.startup();
        });
    }
    
    public void shutdown() {
        this.moduleOrder.forEach((module) -> {
            module.shutdown();
        });
    }
    
    public void doMainLoop() {
        while(this.run) {
            
        }
    }
    
    protected void computeModuleOrder() {
        this.moduleOrder.clear();
        Set<Class<? extends Module>> alreadyOrdered = new HashSet<>();
        
        List<Module> toBeOrdered = new LinkedList<>(this.registeredModules.values());
        while(toBeOrdered.size() > 0) {
            for(int i = 0; i < toBeOrdered.size(); ++i) {
                if(Engine.isModuleSatisfied(alreadyOrdered, toBeOrdered.get(i))) {
                    this.moduleOrder.add(toBeOrdered.get(i));
                    alreadyOrdered.add(toBeOrdered.get(i).getClass());
                    toBeOrdered.remove(i);
                    --i;
                }
            }
        }
    }
    
    protected static boolean isModuleSatisfied(Set<Class<? extends Module>> alreadyOrdered, Module m) {
        if(m.dependencies() == null || m.dependencies().isEmpty()) {
            return true;
        }
        return m.dependencies().stream().map(d -> alreadyOrdered.contains(d)).reduce(true, (a, b) -> a && b);
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

    @Override
    public Collection<Class<? extends Module>> dependencies() {
        return null;
    }
    
    private static class EngineHolder {
        private static final Engine INSTANCE = new Engine();
    }
}
