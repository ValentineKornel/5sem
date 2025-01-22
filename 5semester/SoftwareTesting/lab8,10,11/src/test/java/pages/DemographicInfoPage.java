package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.ui.Select;

public class DemographicInfoPage {
    private WebDriver driver;

    private By dropdown = By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select");
    private By saveButton = By.xpath("/html/body/div[7]/div/div/main/section/div/button[2]");

    public DemographicInfoPage(WebDriver driver){
        this.driver = driver;
    }

    public void setNewGender(String gender){
        Select select = new Select(driver.findElement(dropdown));
        select.selectByVisibleText(gender);
    }

    public void clickSaveButton(){
        driver.findElement(saveButton).click();
    }

    public String getCurrentGender(){
        Select select = new Select(driver.findElement(dropdown));
        return select.getFirstSelectedOption().getText();
    }
}
