/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package core;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;
import org.junit.Test;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

/**
 *
 * @author corbatto
 */
public class EngineTest {
    
    public EngineTest() {
    }

    /**
     * Test of computeModuleOrder method, of class Engine.
     */
    @Test
    public void testComputeModuleOrder() {
        java.lang.System.out.println("computeModuleOrder");
        Engine instance = new Engine();
        instance.registerModule(ModuleImpl4.class);
        instance.computeModuleOrder();
        assertThat(
                instance.moduleOrder.stream()
                        .map(e -> e.getClass())
                        .collect(Collectors.toList()),
                contains(
                        ModuleImpl1.class,
                        ModuleImpl2.class,
                        ModuleImpl3.class,
                        ModuleImpl4.class
                ));
    }

    /**
     * Test of isModuleSatisfied method, of class Engine.
     */
    @Test
    public void testIsModuleSatisfied() {
        java.lang.System.out.println("isModuleSatisfied");
        Set<Class<? extends Module>> alreadyOrdered = new HashSet<>();
        Module m;
        boolean expResult, result;
        
        m = new ModuleImpl1();
        alreadyOrdered.clear();
        expResult = true;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
        
        m = new ModuleImpl1();
        alreadyOrdered.clear();
        alreadyOrdered.add(ModuleImpl3.class);
        expResult = true;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
        
        m = new ModuleImpl2();
        alreadyOrdered.clear();
        alreadyOrdered.add(ModuleImpl1.class);
        expResult = true;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
        
        m = new ModuleImpl2();
        alreadyOrdered.clear();
        alreadyOrdered.add(ModuleImpl3.class);
        expResult = false;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
        
        m = new ModuleImpl3();
        alreadyOrdered.clear();
        alreadyOrdered.add(ModuleImpl1.class);
        alreadyOrdered.add(ModuleImpl2.class);
        expResult = true;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
        
        m = new ModuleImpl3();
        alreadyOrdered.clear();
        alreadyOrdered.add(ModuleImpl2.class);
        expResult = false;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
        
        m = new ModuleImpl3();
        alreadyOrdered.clear();
        alreadyOrdered.add(ModuleImpl1.class);
        expResult = false;
        result = Engine.isModuleSatisfied(alreadyOrdered, m);
        assertThat(result, equalTo(expResult));
    }
    
    public static class ModuleImpl1 implements Module {
        public ModuleImpl1() {}
        @Override
        public Collection<Class<? extends Module>> dependencies() {
            return null;
        }
        @Override
        public void startup() {}
        @Override
        public void shutdown() {}
        
    }
    
    public static class ModuleImpl2 implements Module {
        public ModuleImpl2() {}
        @Override
        public Collection<Class<? extends Module>> dependencies() {
            Collection<Class<? extends Module>> ret = new ArrayList<>();
            ret.add(ModuleImpl1.class);
            return ret;
        }
        @Override
        public void startup() {}
        @Override
        public void shutdown() {}
    }
    
    public static class ModuleImpl3 implements Module {
        public ModuleImpl3() {}
        @Override
        public Collection<Class<? extends Module>> dependencies() {
            Collection<Class<? extends Module>> ret = new ArrayList<>();
            ret.add(ModuleImpl1.class);
            ret.add(ModuleImpl2.class);
            return ret;
        }
        @Override
        public void startup() {}
        @Override
        public void shutdown() {}
    }
    
    public static class ModuleImpl4 implements Module {
        public ModuleImpl4() {}
        @Override
        public Collection<Class<? extends Module>> dependencies() {
            Collection<Class<? extends Module>> ret = new ArrayList<>();
            ret.add(ModuleImpl3.class);
            return ret;
        }
        @Override
        public void startup() {}
        @Override
        public void shutdown() {}
    }
}
