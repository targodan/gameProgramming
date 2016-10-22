/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package core;

import org.junit.Test;
import org.junit.runner.RunWith;
import static org.junit.Assert.assertThat;
import static org.hamcrest.Matchers.*;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;
import static org.mockito.Mockito.*;

/**
 *
 * @author Luca Corbatto {@literal <luca@corbatto.de>}
 */
public class ReflectionMSGRegistererTest {
    
    public ReflectionMSGRegistererTest() {
    }

    /**
     * Test of RegisterMessages method, of class MessageRegister.
     */
    @Test
    public void testRegisterMessages_Ok() {
        java.lang.System.out.println("RegisterMessages");
        MessageManager manager = new MessageManager();
        ReflectionMSGRegisterer instance = new MessageRegisterImplOk();
        instance.RegisterMessages(manager);
        assertThat(manager.isRegistered("MSG_SOME_MESSAGE"), equalTo(true));
        assertThat(manager.isRegistered("MSG_SOME_MESSAGE2"), equalTo(true));
        assertThat(manager.isRegistered("notAMessage"), equalTo(false));
        assertThat(manager.isRegistered("notAMessage2"), equalTo(false));
    }

    /**
     * Test of RegisterMessages method, of class MessageRegister.
     */
    @Test(expected = WTFException.class)
    public void testRegisterMessages_WrongType() {
        java.lang.System.out.println("RegisterMessages");
        MessageManager manager = new MessageManager();
        ReflectionMSGRegisterer instance = new MessageRegisterImplWrongType();
        instance.RegisterMessages(manager);
    }

    /**
     * Test of RegisterMessages method, of class MessageRegister.
     */
    @Test(expected = WTFException.class)
    public void testRegisterMessages_WrongAccess() {
        java.lang.System.out.println("RegisterMessages");
        MessageManager manager = new MessageManager();
        ReflectionMSGRegisterer instance = new MessageRegisterImplWrongAccess();
        instance.RegisterMessages(manager);
    }

    public class MessageRegisterImplOk extends ReflectionMSGRegisterer {
        public int MSG_SOME_MESSAGE;
        public int MSG_SOME_MESSAGE2;
        protected int notAMessage;
        protected int notAMessage2;
    }
    
    public class MessageRegisterImplWrongType extends ReflectionMSGRegisterer {
        protected boolean MSG_NO_MESSAGE;
        protected int MSG_SOME_MESSAGE;
        protected int MSG_SOME_MESSAGE2;
        protected int notAMessage;
        protected int notAMessage2;
    }
    
    public class MessageRegisterImplWrongAccess extends ReflectionMSGRegisterer {
        private int MSG_NO_MESSAGE;
        protected int MSG_SOME_MESSAGE;
        protected int MSG_SOME_MESSAGE2;
        protected int notAMessage;
        protected int notAMessage2;
    }
}
