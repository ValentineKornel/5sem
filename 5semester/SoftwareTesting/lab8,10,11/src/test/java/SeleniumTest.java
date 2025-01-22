import io.github.bonigarcia.wdm.WebDriverManager;
import org.junit.jupiter.api.*;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import org.openqa.selenium.*;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.chrome.ChromeOptions;
import org.openqa.selenium.io.FileHandler;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.Select;
import org.openqa.selenium.support.ui.WebDriverWait;
import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.IOException;
import java.time.Duration;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Set;

@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
public class SeleniumTest {
    private static WebDriver driver;
    static WebDriverWait wait;
    private static boolean authorized;

    @BeforeAll
    public static void setUp(){
        ChromeOptions options = new ChromeOptions();
        options.addArguments("--start-maximized");

        WebDriverManager.chromedriver().setup();
        driver = new ChromeDriver(options);
        driver.manage().timeouts().implicitlyWait(Duration.ofSeconds(20));
        wait = new WebDriverWait(driver, Duration.ofSeconds(20));
        authorized = false;
    }

    @Test
    @Order(1)
    @Tag("critical")
    public void authorize(){

        driver.get("https://www.linkedin.com/login");

        WebElement emailField = driver.findElement(By.id("username"));
        emailField.sendKeys("valentine.korneliuk17@gmail.com");

        WebElement passwordField = driver.findElement(By.id("password"));
        passwordField.sendKeys("valentine5264552");

        WebElement loginButton = driver.findElement(By.xpath("//button[@type='submit']"));
        loginButton.click();

        WebElement profileIcon = driver.findElement(By.cssSelector("img[alt*='Valentine Korneliuk']"));

        assertNotNull(profileIcon, "не удалось войти в учетную запись LinkedIn");
        authorized = true;
        System.out.println("Successful login to LinedIn!");
        takeScreenshot("MainPage");

        Set<Cookie> cookies = driver.manage().getCookies();
        System.out.println("All cookies:");
        for (Cookie cookie : cookies) {
            System.out.println(cookie);
        }

    }


    @Test
    @Disabled
    @Tag("expectedFailure")
    @Order(2)
    public void sendMessage() throws InterruptedException {
        if(!authorized){
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
        messageBox.sendKeys("Hi!");

        wait.until(ExpectedConditions.elementToBeClickable(By.cssSelector("button.msg-form__send-button")));
        WebElement sendButton = driver.findElement(By.cssSelector("button.msg-form__send-button"));
        sendButton.click();


        WebElement lastMessage = driver.findElement(By.cssSelector("#message-list-ember70 > ul > li.msg-s-message-list__event.clearfix.msg-s-message-list__last-msg-ember110 > div > div.msg-s-event-listitem__message-bubble.msg-s-event-listitem__message-bubble--msg-fwd-enabled > div > div > p"));

        System.out.println(lastMessage.getText());
        assertEquals("Hi!", lastMessage.getText(), "Последнее соббщение не соответствует отправленному");
        System.out.println("Сообщение успешно отправлено в первый чат.");

        Thread.sleep(5000);
    }

    @ParameterizedTest
    @Order(3)
    @Tag("critical")
    @ValueSource(strings = {"Man", "Woman"})
    public void changeGender(String gender) throws InterruptedException {
        if(!authorized){
            authorize();
        }

        driver.get("https://www.linkedin.com/mypreferences/d/demographic-info-copy");

        wait.until(ExpectedConditions.presenceOfElementLocated(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select")));
        WebElement dropdown = driver.findElement(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select"));

        Select select = new Select(dropdown);

        select.selectByVisibleText(gender);

        WebElement saveButton = driver.findElement(By.xpath("/html/body/div[7]/div/div/main/section/div/button[2]"));
        saveButton.click();

        Thread.sleep(2000);

        driver.get("https://www.linkedin.com/mypreferences/d/demographic-info-copy");

        wait.until(ExpectedConditions.presenceOfElementLocated(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select")));
        dropdown = driver.findElement(By.xpath("/html/body/div[7]/div/div/main/section/div/div[1]/div/select"));

        select = new Select(dropdown);

        assertEquals(gender, select.getFirstSelectedOption().getText(), "не удалось изменить гендер");
        System.out.println("Gender successfully changed");
    }

    @Test
    @Disabled
    @Order(4)
    public void checkSubscribtion() throws InterruptedException {
        if(!authorized){
            authorize();
        }
        driver.get("https://www.linkedin.com/school/belarusian-state-university/");

        WebElement followButton = driver.findElement(By.cssSelector("button.follow.org-company-follow-button.org-top-card-primary-actions__action.artdeco-button.artdeco-button--primary"));
        followButton.click();

        driver.get("https://www.linkedin.com/mynetwork/network-manager/company/");

        List<WebElement> subscriptions = driver.findElements(By.xpath("//a[text()='Belarusian State University']"));

        assertTrue(subscriptions.size() > 0, "Ошибка: Belarusian State University не найден в подписках.");
        System.out.println("Subscription to Belarusian State University successfully added.");

        Thread.sleep(2000);
    }

    @AfterAll
    public static void logOut(){
        driver.quit();
    }

    public static void takeScreenshot(String screenshotName) {
        File screenshot = ((TakesScreenshot) driver).getScreenshotAs(OutputType.FILE);

        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyyMMdd_HHmmss");
        String timestamp = LocalDateTime.now().format(formatter);

        File destinationFile = new File("D:\\Study\\university\\5semester\\TPO\\lab8,10\\screenshots\\" + screenshotName + "_" + timestamp + ".png");

        try {
            FileHandler.copy(screenshot, destinationFile);
            System.out.println("screenshot saved: " + destinationFile.getAbsolutePath());
        } catch (IOException e) {
            System.err.println("Не удалось сохранить скриншот: " + e.getMessage());
        }
    }

}
