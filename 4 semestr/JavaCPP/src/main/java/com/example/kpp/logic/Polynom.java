package com.example.kpp.logic;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import com.example.kpp.cache.GetCache;
import com.example.kpp.exceptionhandling.MyThrowable;


public class Polynom {

    static Logger MyLogger = LoggerFactory.getLogger(Polynom.class);

    public static String[][] is_polynom(String test) throws MyThrowable{
        if (test.equals("empty")) {
            MyLogger.warn("Get request without params");
            throw new MyThrowable(400);
        }
        String[][] reply = new String[3][2];

        reply[0][0] = "Word: ";
        reply[0][1] = test;
        reply[1][0] = "Size: ";
        reply[1][1] = Integer.toString(test.length());
        reply[2][0] = "Is Polynom: ";

        MyLogger.info("Successfully send answer");
        for (int i = 0; i < test.length(); i++)
            if (test.charAt(i) != test.charAt(test.length() - i - 1)) {
                reply[2][1] = "false";
                return reply;
            }
        reply[2][1] = "true";
        return reply;
    }

}
