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
public class WTFException extends RuntimeException {
    public WTFException() {
    }

    public WTFException(String message) {
        super(message);
    }

    public WTFException(String message, Throwable cause) {
        super(message, cause);
    }

    public WTFException(Throwable cause) {
        super(cause);
    }

    public WTFException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
