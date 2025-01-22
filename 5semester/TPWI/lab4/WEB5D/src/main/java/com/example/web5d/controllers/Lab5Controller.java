package com.example.web5d.controllers;

import org.springframework.core.io.ClassPathResource;
import org.springframework.core.io.Resource;
import org.springframework.http.CacheControl;
import org.springframework.http.HttpHeaders;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.io.IOException;
import java.io.InputStream;

@CrossOrigin("*")
@Controller
public class Lab5Controller {

    @GetMapping("/home")
    public String getMainPage(){
        return "page";
    }

    @GetMapping("/image")
    public ResponseEntity<byte[]> getImage(@RequestParam(required = false, defaultValue = "cache") String cache_parm) throws IOException{

        System.out.println("new request: " + cache_parm);
        InputStream imageStream = getClass().getResourceAsStream("/static/image.png");
        byte[] imageBytes = imageStream.readAllBytes();

        HttpHeaders headers = new HttpHeaders();

        headers.set("Content-Type", "image/png");
        switch (cache_parm) {
            case "no_store" -> headers.setCacheControl(CacheControl.noStore());
            case "max-age" -> headers.set("Cache-Control", "max-age=1800");
            case "last-modified" -> headers.setCacheControl(String.valueOf(System.currentTimeMillis()));
            case "etag" -> headers.setETag("\"image-etag-" + System.currentTimeMillis() + "\"");
            case "expired" -> headers.set("Expires", "Wed, 27 Nov 2024 07:28:00 GMT");
            default -> headers.setCacheControl(CacheControl.noCache());
        }

        return ResponseEntity.ok()
                .headers(headers)
                .body(imageBytes);
    }

    @GetMapping("/css")
    public ResponseEntity<Resource> getCss(@RequestParam(required = false, defaultValue = "cache") String cache_parm) throws IOException{

        System.out.println("new request: " + cache_parm);
        Resource resource = new ClassPathResource("static/style.css");

        if (!resource.exists()) {
            throw new IOException("CSS file not found");
        }
        HttpHeaders headers = new HttpHeaders();

        headers.set("Content-Type", "style/css");
        switch (cache_parm) {
            case "no_store" -> headers.setCacheControl(CacheControl.noStore());
            case "max-age" -> headers.set("Cache-Control", "max-age=1800");
            case "last-modified" -> headers.setCacheControl(String.valueOf(System.currentTimeMillis()));
            case "etag" -> headers.setETag("\"image-etag-" + System.currentTimeMillis() + "\"");
            case "expired" -> headers.set("Expires", "Wed, 27 Nov 2024 07:28:00 GMT");
            default -> headers.setCacheControl(CacheControl.noCache());
        }

        return ResponseEntity.ok()
                .headers(headers)
                .body(resource);
    }

    @GetMapping("/script")
    public ResponseEntity<Resource> getScript(@RequestParam(required = false, defaultValue = "cache") String cache_parm) throws IOException{

        System.out.println("new request: " + cache_parm);
        Resource resource = new ClassPathResource("static/script.js");

        if (!resource.exists()) {
            throw new IOException("CSS file not found");
        }
        HttpHeaders headers = new HttpHeaders();

        headers.set("Content-Type", "text/css");
        switch (cache_parm) {
            case "no_store" -> headers.setCacheControl(CacheControl.noStore());
            case "max-age" -> headers.set("Cache-Control", "max-age=1800");
            case "last-modified" -> headers.setCacheControl(String.valueOf(System.currentTimeMillis()));
            case "etag" -> headers.setETag("\"image-etag-" + System.currentTimeMillis() + "\"");
            case "expired" -> headers.set("Expires", "Wed, 27 Nov 2024 07:28:00 GMT");
            default -> headers.setCacheControl(CacheControl.noCache());
        }

        return ResponseEntity.ok()
                .headers(headers)
                .body(resource);
    }
}
