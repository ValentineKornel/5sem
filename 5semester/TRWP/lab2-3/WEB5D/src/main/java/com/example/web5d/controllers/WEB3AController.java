package com.example.web5d.controllers;

import com.example.web5d.model.CounterRequestModel;
import com.example.web5d.model.CounterResponseModel;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import java.util.HashMap;
import java.util.Map;

@CrossOrigin("*")
@RestController
public class WEB3AController {

    @PostMapping("/count")
    public ResponseEntity<CounterResponseModel> count(@RequestBody CounterRequestModel body,
                                                     HttpServletRequest request,
                                                     HttpServletResponse response){


        Map<String, String> cookieMap = new HashMap<>();
        Cookie[] cookies = request.getCookies();

        if (cookies != null) {
            for (Cookie cookie : cookies) {
                cookieMap.put(cookie.getName(), cookie.getValue());
            }
        }

        int x = Integer.parseInt(cookieMap.getOrDefault("x", "0"));
        int y = Integer.parseInt(cookieMap.getOrDefault("y", "0"));
        int requestCount = Integer.parseInt(cookieMap.getOrDefault("requestCount", "0"));

        if((requestCount % 5) == 0){
            x = 0;
            y = 0;
        }

        String newCounter = "requestCount=" + String.valueOf(requestCount+1) + ";Max-Age=" + (24 * 60 * 60) + "; Path=/";
        String newX = "x=" + String.valueOf(x + body.getX()) + ";Max-Age=" + (24 * 60 * 60) + "; Path=/";
        String newY = "y=" + String.valueOf(y + body.getY()) + ";Max-Age=" + (24 * 60 * 60) + "; Path=/";


        HttpHeaders newCookies = new HttpHeaders();
        newCookies.add(HttpHeaders.SET_COOKIE, newCounter);
        newCookies.add(HttpHeaders.SET_COOKIE, newX);
        newCookies.add(HttpHeaders.SET_COOKIE, newY);


        CounterResponseModel resp = new CounterResponseModel();
        resp.setSx(x + body.getX());
        resp.setSy(y + body.getY());
        return ResponseEntity.status(HttpStatus.OK).headers(newCookies).body(resp);
    }
}
