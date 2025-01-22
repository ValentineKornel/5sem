package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;

import java.util.List;

public class SubscriptionsPage {
    WebDriver driver;

    private By subscriptions = By.xpath("//a[text()='Belarusian State University']");

    public SubscriptionsPage(WebDriver driver){
        this.driver = driver;
    }

    public List<WebElement> getSubscriptions(){
        return driver.findElements(subscriptions);
    }
}
