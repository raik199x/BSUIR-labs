package com.example.kpp.exceptionhandling;

import com.example.kpp.logic.Polynom;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;

@ControllerAdvice
public class Exceptions {
    static Logger MyLogger = LoggerFactory.getLogger(Exceptions.class);
    @ExceptionHandler
    public ResponseEntity<Object> exception(MyThrowable exception) {
        if (exception.status_code == 400)
            return new ResponseEntity<>("Empty line was send", HttpStatus.BAD_REQUEST);
        else
            return new ResponseEntity<>("Unexpected Error", HttpStatus.INTERNAL_SERVER_ERROR);
    }

    @ExceptionHandler(value = Exception.class)
    public ResponseEntity<String> DefaultExceptionHandler(Exception exception) {
        MyLogger.error("500 error");
        return new ResponseEntity<>("Unexpected Error", HttpStatus.INTERNAL_SERVER_ERROR);
    }

}