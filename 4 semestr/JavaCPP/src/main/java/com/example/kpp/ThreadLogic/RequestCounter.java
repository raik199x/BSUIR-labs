package com.example.kpp.ThreadLogic;

public class RequestCounter {
    static int CountRequests = 0;

    public synchronized static void IncCounter(){
        CountRequests++;
    }
    public synchronized static int AskCounter(){
        return CountRequests;
    }
}
