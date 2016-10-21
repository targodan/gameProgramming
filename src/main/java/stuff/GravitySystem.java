/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package stuff;

import core.Component;
import core.EntityManager;
import core.WTFException;
import java.util.List;
import java.util.stream.Stream;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class GravitySystem implements core.System {

    @Override
    public void process(double dT, EntityManager em) {
        int posIndex = 0;
        List<Component> gravity = em.getComponentsOfType(GravityComponent.class);
        List<Component> position = em.getComponentsOfType(PositionComponent.class);
        for(int gravIndex = 0; gravIndex < gravity.size(); ++gravIndex) {
            GravityComponent grav = (GravityComponent)gravity.get(gravIndex);
            PositionComponent pos = null;
            for(; posIndex < position.size(); ++posIndex) {
                if(grav.getEntityID() == position.get(posIndex).getEntityID()) {
                    pos = (PositionComponent)position.get(posIndex);
                    break;
                }
            }
            if(pos == null) {
                throw new WTFException("The entity "+grav.getEntityID()+" has component gravity but is missing component position.");
            }
            
            pos.direction.add(Math3D.mul(grav.gravity, dT));
        }
    }
    
}
