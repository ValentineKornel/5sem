package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

public class MainPage {
    WebDriver driver;

    private By messagingTab = By.cssSelector("a[href='https://www.linkedin.com/messaging/?']");

    public MainPage(WebDriver driver){
        this.driver = driver;
    }

    public void clickMessagingTab(){
        driver.findElement(messagingTab).click();
    }
}
