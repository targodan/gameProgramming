package core;


import java.util.*;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class EntityManager {
    // Invariant: All component lists are sorted ascending by time of adding the
    //            *entity*.
    private final Map<Class<?>, List<Component>> components;
    private int nextEntity;
    
    public EntityManager() {
        this.components = new HashMap<>();
        this.nextEntity = 0;
    }
    
    public void registerComponent(Class<?> comp) {
        if(this.components.containsKey(comp)) {
            throw new WTFException("The component is already registered.");
        }
        this.components.put(comp, new ArrayList<>());
    }
    
    public int addEntity(Collection<Component> comps) {
        int id = this.nextEntity;
        ++this.nextEntity;
        comps.stream().forEach(comp -> {
            comp.setEntityID(id);
            this.components.get(comp.getClass()).add(comp);
        });
        return id;
    }
    
    public void addComponent(int entityID, Component comp) {
        comp.setEntityID(entityID);
        List<Component> list = this.components.get(comp.getClass());
        list.add(comp);
        // Keep the invariant valid.
        list.sort((a, b) -> b.getEntityID() - a.getEntityID());
        Engine.getInstance().getMessageManager().sendMessage(new Message(Messages.getInstance().ENTITY_ADD_COMPONENT(), entityID));
    }
    
    public List<Component> getComponentsOfType(Class<?> type) {
        return this.components.get(type);
    }
}
