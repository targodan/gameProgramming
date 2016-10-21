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
    private final Map<Class<?>, List<Component>> components;
    private int nextEntity;
    
    public EntityManager() {
        this.components = new HashMap<>();
        this.nextEntity = 0;
    }
    
    public void registerComponent(Class<?> comp) {
        if(this.components.containsKey(comp)) {
            throw new IllegalArgumentException("The component is already registered.");
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
}
