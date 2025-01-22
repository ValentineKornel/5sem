package org.example;

import io.github.bonigarcia.wdm.WebDriverManager;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.Select;
import org.openqa.selenium.support.ui.WebDriverWait;


import java.time.Duration;
import java.util.List;

public class SeleniumTester {





    private WebDriver driver;
    WebDriverWait wait;
    private boolean authorized = false;

    public SeleniumTester(){
        WebDriverManager.chromedriver().setup();
        this.driver = new ChromeDriver();
        driver.manage().timeouts().implicitlyWait(Duration.ofSeconds(20));
        this.wait = new WebDriverWait(driver, Duration.ofSeconds(20));
    }

    public void authorize(){

        driver.get("https://www.linkedin.com/login");

        WebElement emailField = driver.findElement(By.id("username"));
        emailField.sendKeys("valentine.korneliuk17@gmail.com");

        WebElement passwordField = driver.findElement(By.id("password"));
        passwordField.sendKeys("password");

        WebElement loginButton = driver.findElement(By.xpath("//button[@type='submit']"));
        loginButton.click();

        WebElement profileIcon = driver.findElement(By.cssSelector("img[alt*='Valentine Korneliuk']"));
        if (profileIcon.isDisplayed()) {
            System.out.println("Успешный вход в LinkedIn!");
            this.authorized = true;
        } else {
            System.out.println("Не удалось войти в LinkedIn.");
        }
    }

    public void sendMessage() throws InterruptedException {
        if(!this.authorized){
            authorize();
        }

        wait.until(ExpectedConditions.presenceOfElementLocated(By.cssSelector("a[href='https://www.linkedin.com/messaging/?']")));

        WebElement messagingTab = driver.findElement(By.cssSelector("a[href='https://www.linkedin.com/messaging/?']"));
        messagingTab.click();

        wait.until(ExpectedConditions.presenceOfElementLocated(By.cssSelector("ul.msg-conversations-container__conversations-list")));

        WebElement firstChat = driver.findElement(By.cssSelector("ul.list-style-none.msg-conversations-container__conversations-list li:nth-child(2)"));
        firstChat.click();

        wait.until(ExpectedConditions.presenceOfElementLocated(By.cssSelector("div.msg-form__contenteditable")));

        WebElement messageBox = driver.findElement(By.cssSelector("div.msg-form__contenteditable"));
        messageBox.sendKeys("Hello!");

        wait.until(ExpectedConditions.elementToBeClickable(By.cssSelector("button.msg-form__send-button")));
        WebElement sendButton = driver.findElement(By.cssSelector("button.msg-form__send-button"));
        sendButton.click();

        System.out.println("Сообщение успешно отправлено в первый чат.");

        Thread.sleep(5000);
    }

    public void changeGender() throws InterruptedException {
        if(!this.authorized){
            authorize();
        }

        driver.get("https://www.linkedin.com/mypreferences/d/demographic-info-copy");

        wait.until(ExpectedConditions.presenceOfElementLocated(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select")));
        WebElement dropdown = driver.findElement(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select"));

        Select select = new Select(dropdown);

        //String gender = "Man";
        String gender = "Woman";
        select.selectByVisibleText(gender);

        WebElement saveButton = driver.findElement(By.xpath("/html/body/div[7]/div/div/main/section/div/button[2]"));
        saveButton.click();

        Thread.sleep(2000);

        driver.get("https://www.linkedin.com/mypreferences/d/demographic-info-copy");

        wait.until(ExpectedConditions.presenceOfElementLocated(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select")));
        dropdown = driver.findElement(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select"));

        select = new Select(dropdown);
        if(select.getFirstSelectedOption().getText().equals(gender)){
            System.out.println("гендер успешно изменен");
            Thread.sleep(2000);
        }else{
            System.out.println("не удалось изменить гендер");
        }
    }

    public void checkSubscribtion() throws InterruptedException {
        if(!this.authorized){
            authorize();
        }
        driver.get("https://www.linkedin.com/school/belarusian-state-university/");

        WebElement followButton = driver.findElement(By.cssSelector("button.follow.org-company-follow-button.org-top-card-primary-actions__action.artdeco-button.artdeco-button--primary"));
        followButton.click();

        driver.get("https://www.linkedin.com/mynetwork/network-manager/company/");

        List<WebElement> subscriptions = driver.findElements(By.xpath("//a[text()='Belarusian State University']"));

        if (subscriptions.size() > 0) {
            System.out.println("Подписка на Belarusian State University успешно добавлена.");
        } else {
            System.out.println("Ошибка: Belarusian State University не найден в подписках.");
        }
        Thread.sleep(2000);
    }

    public void logOut(){
        driver.quit();
    }

}
