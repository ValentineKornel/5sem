package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

public class BSUPage {
    private WebDriver driver;

    private By followButton = By.cssSelector("button.follow.org-company-follow-button.org-top-card-primary-actions__action.artdeco-button.artdeco-button--primary");

    public BSUPage(WebDriver driver){
        this.driver = driver;
    }

    public void clickFollowButton(){
        driver.findElement(followButton).click();
    }
}
