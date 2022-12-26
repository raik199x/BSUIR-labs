package com.example.kpp.cache;

import org.springframework.context.annotation.Bean;
import org.springframework.stereotype.Component;

import java.util.HashMap;
import java.util.Map;

@Component
public class GetCache {
    static Map<String, String[][]> states = new HashMap<String, String[][]>();


    public String[][] Get(String line){
        String[][] reply = new String[3][2];
        if(states.containsKey(line)){
            return states.get(line);
        } else{
            reply[0][0] = "FALSE";
            return reply;
        }
    }

    public void Set(String line, String[][] result){
        states.put(line,result);
    }

}
