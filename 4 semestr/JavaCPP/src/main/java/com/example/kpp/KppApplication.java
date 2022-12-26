package com.example.kpp;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cache.annotation.EnableCaching;

@SpringBootApplication
public class KppApplication {

    public static void main(String[] args) {
        SpringApplication.run(KppApplication.class, args);
    }

}
