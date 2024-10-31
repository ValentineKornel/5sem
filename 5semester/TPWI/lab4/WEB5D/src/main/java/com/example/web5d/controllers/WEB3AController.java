package com.example.web5d.controllers;

import com.example.web5d.model.CounterRequestModel;
import com.example.web5d.model.CounterResponseModel;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
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
                                                      HttpSession session,
                                                      HttpServletRequest request,
                                                      HttpServletResponse response){

        Integer x = (Integer) session.getAttribute("x");
        Integer y = (Integer) session.getAttribute("y");
        Integer requestCount = (Integer) session.getAttribute("requestCount");

        if(x == null) x = 0;
        if(y == null) y = 0;
        if(requestCount == null) requestCount = 0;

        System.out.println(x);
        System.out.println(y);
        System.out.println(requestCount);


        if((requestCount % 5) == 0){
            x = 0;
            y = 0;
        }

        Integer newX = x + body.getX();
        Integer newY = y + body.getY();

        session.setAttribute("x", newX);
        session.setAttribute("y", newY);
        session.setAttribute("requestCount", requestCount + 1);

        CounterResponseModel resp = new CounterResponseModel();
        resp.setSx(newX);
        resp.setSy(newY);
        return ResponseEntity.status(HttpStatus.OK).body(resp);
    }
}
