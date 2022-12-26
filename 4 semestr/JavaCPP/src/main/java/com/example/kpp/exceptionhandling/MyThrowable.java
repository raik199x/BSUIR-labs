package com.example.kpp.exceptionhandling;

public class MyThrowable extends Throwable {
    public final int status_code;

    public MyThrowable(int status_code) {
        this.status_code = status_code;
    }

}
