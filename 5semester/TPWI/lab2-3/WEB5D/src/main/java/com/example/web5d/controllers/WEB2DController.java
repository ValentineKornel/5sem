package com.example.web5d.controllers;

import jakarta.servlet.http.HttpServletResponse;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.Random;

@CrossOrigin("*")
@RestController
public class WEB2DController {

    @PostMapping("/add")
    public void getZ(@RequestHeader("X-Value-x") String xValue,
                     @RequestHeader("X-Value-y") String yValue,
                     HttpServletResponse response) throws InterruptedException {

        int x = Integer.parseInt(xValue);
        int y = Integer.parseInt(yValue);
        int z = x + y;
        //Thread.sleep(3000);
        response.setHeader("X-Value-z", String.valueOf(z));
        response.setHeader("Access-Control-Expose-Headers", "X-Value-z");
    }

    @PostMapping("/getRandomRow")
    public ResponseEntity<String> getRandomRow(@RequestHeader("X-Rand-N") String nValue) throws InterruptedException {

        ArrayList<Integer> res = new ArrayList<>();
        Random random = new Random();

        int max = Integer.parseInt(nValue);
        int min = -max;
        int iter = random.nextInt(10 - 4) + 5;
        for(int i = 0; i < iter; i++){
            res.add(random.nextInt(max - min + 1) + min);
        }
        //Thread.sleep(1000);
        return ResponseEntity.status(HttpStatus.OK).body(res.toString());
    }
}
