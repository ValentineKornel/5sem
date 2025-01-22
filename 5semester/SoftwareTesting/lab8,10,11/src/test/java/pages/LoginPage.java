package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

public class LoginPage {
    private WebDriver driver;

    private By emailField = By.id("username");
    private By passwordField = By.id("password");
    private By loginButton = By.xpath("//button[@type='submit']");
    private By profileIcon = By.cssSelector("img[alt*='Valentine Korneliuk']");




    public  LoginPage(WebDriver webDriver){
        this.driver = webDriver;
    }

    public void enterEmail(String email){
        driver.findElement(emailField).sendKeys(email);
    }

    public void enterPassword(String password){
        driver.findElement(passwordField).sendKeys(password);
    }

    public void clickLoginButton(){
        driver.findElement(loginButton).click();
    }

    public Boolean isProfilePictureFound() {
        return driver.findElement(profileIcon) != null;
    }

}
