package org.example;

import io.github.bonigarcia.wdm.WebDriverManager;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;

public class MainClass {

    public static void main(String[] args) throws InterruptedException {
        SeleniumTester tester = new SeleniumTester();

        try{
            tester.authorize();
            tester.sendMessage();
            tester.changeGender();
            tester.checkSubscribtion();
            tester.logOut();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
