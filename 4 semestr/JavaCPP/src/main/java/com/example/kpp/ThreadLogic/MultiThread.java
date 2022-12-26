package com.example.kpp.ThreadLogic;



public class MultiThread extends Thread{
    @Override
    public synchronized void run(){ //Is it should work?
        RequestCounter.IncCounter();
    }
}
