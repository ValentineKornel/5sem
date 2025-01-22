package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

public class MessagingPage {
    private WebDriver driver;


    private By firstChat = By.cssSelector("ul.list-style-none.msg-conversations-container__conversations-list li:nth-child(2)");
    private By messageBox = By.cssSelector("div.msg-form__contenteditable");
    private By sendButton = By.cssSelector("button.msg-form__send-button");
    private By lastMessage = By.cssSelector("#message-list-ember70 > ul > li.msg-s-message-list__event.clearfix.msg-s-message-list__last-msg-ember110 > div > div.msg-s-event-listitem__message-bubble.msg-s-event-listitem__message-bubble--msg-fwd-enabled > div > div > p");

    public MessagingPage(WebDriver driver){
        this.driver = driver;
    }

    public void clickFirstChat(){
        driver.findElement(firstChat).click();
    }

    public void enterMessage(String message){
        driver.findElement(messageBox).sendKeys(message);
    }

    public void clickSendButton(){
        driver.findElement(sendButton).click();
    }

    public String getLastMessageTest(){
        return driver.findElement(lastMessage).getText();
    }
}
