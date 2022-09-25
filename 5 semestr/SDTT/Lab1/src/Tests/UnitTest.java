package Tests;
import demo.parallel.Complex;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;

public class UnitTest {
    public static final String ANSI_RESET = "\u001B[0m";

    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";

    @Test
    public void testSub(){
        Complex a = new Complex(1.5,0.32);
        Complex b = new Complex(1, 0.22);
        a = a.sub(b);
        assertEquals(0.5, a.GetRe(a));
        assertEquals(0.1, a.GetIm(a));
    }
    @Test
    public void testDivide(){
        Complex a = new Complex(8,16);
        Complex b = new Complex(2, 4);
        a = a.divide(b);
        assertEquals(4, a.GetRe(a));
        assertEquals(0.8, a.GetIm(a));
    }
}
