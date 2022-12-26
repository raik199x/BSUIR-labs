package com.example.kpp;

import com.example.kpp.ThreadLogic.MultiThread;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicInteger;

import com.example.kpp.ThreadLogic.RequestCounter;
import com.example.kpp.cache.GetCache;
import com.example.kpp.exceptionhandling.MyThrowable;
import com.example.kpp.logic.Polynom;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
public class ServerController {

    static Logger MyLogger = LoggerFactory.getLogger(Polynom.class);
    private GetCache getCache;

    @Autowired
    public void SetCache(GetCache getCache){
        this.getCache = getCache;
    }

    @GetMapping(path = "/polynom")
    public String[][] WelcomMessage(@RequestParam(name="MyPath",defaultValue = "empty") String test) throws MyThrowable {

        String[][] reply = new String[3][2];
        MultiThread MyThread = new MultiThread();
        MyThread.start();
        //checking cache
        reply = getCache.Get(test);
        if(!reply[0][0].equals("FALSE")) {
            MyLogger.info("Send info from cache");
            return reply;
        }
        reply = Polynom.is_polynom(test);
        getCache.Set(test,reply);
        return reply;
    }
    @GetMapping(path = "/Calls")
    public int ShowCalls(){
        return RequestCounter.AskCounter();
    }
    @PostMapping(path = "/polynom")
    public ResponseEntity<?> ManyData(@RequestBody ArrayList<String> data){
        ArrayList<String> Results = new ArrayList<>();
        MyLogger.info("Post request is in task");
        AtomicInteger TruePolynoms = new AtomicInteger();
        AtomicInteger FalsePolynoms = new AtomicInteger();
        data.forEach((currentElement) -> {
            try {
                String result = Polynom.is_polynom(currentElement)[2][1];
                Results.add(result);
                if(result == "true")
                    TruePolynoms.getAndIncrement();
                else
                    FalsePolynoms.getAndIncrement();
            } catch (MyThrowable e) {
                throw new RuntimeException(e);
            }
        });
        Results.add("True: " + TruePolynoms);
        Results.add("False: "+ FalsePolynoms);
        return new ResponseEntity<>(Results, HttpStatus.OK);
    }

}
