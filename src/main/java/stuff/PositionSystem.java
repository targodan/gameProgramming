/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package stuff;

import core.EntityManager;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class PositionSystem implements core.System {

    @Override
    public void process(double dT, EntityManager em) {
        em.getComponentsOfType(PositionComponent.class).stream()
                .map(o -> (PositionComponent)o)
                .forEach(pos -> {
            pos.position.add(Math3D.mul(pos.direction, dT));
        });
    }
}
