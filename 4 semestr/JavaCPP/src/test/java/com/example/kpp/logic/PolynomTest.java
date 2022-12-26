package com.example.kpp.logic;

import com.example.kpp.exceptionhandling.MyThrowable;
import org.junit.jupiter.api.Test;

import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.assertj.core.api.Assertions.catchThrowableOfType;
import static org.junit.jupiter.api.Assertions.*;
//letter big file
class PolynomTest {

    @Test
    void ReturnTruePolynom() throws MyThrowable {
        String[][] reply = new String[3][2];
        Polynom test = new Polynom();
        reply = Polynom.is_polynom("abba");
        assertEquals(reply[2][1], "true");
        assertEquals(reply[1][1], "4");
    }
    @Test
    void ReturnFalsePolynom() throws MyThrowable{
        String[][] reply = new String[3][2];
        Polynom test = new Polynom();
        reply = Polynom.is_polynom("not_polynom");
        assertEquals(reply[2][1], "false");
        assertEquals(reply[1][1], "11");
    }
    @Test
    void ReturnExceptionEmptyPath() throws MyThrowable{
        String[][] reply = new String[3][2];
        Polynom test = new Polynom();
        MyThrowable throwable = catchThrowableOfType(()-> Polynom.is_polynom("empty"), MyThrowable.class);
        assertEquals(throwable.status_code, 400);
    }

}