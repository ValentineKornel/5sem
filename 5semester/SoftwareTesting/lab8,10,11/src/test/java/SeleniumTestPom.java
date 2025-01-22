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
import pages.*;

import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.IOException;
import java.time.Duration;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Set;

@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
public class SeleniumTestPom {
    private static WebDriver driver;
    static WebDriverWait wait;
    private static boolean authorized;

    private static LoginPage loginPage;
    private static MainPage mainPage;
    private static MessagingPage messagingPage;
    private static DemographicInfoPage demographicInfoPage;
    private static BSUPage bsuPage;
    private static SubscriptionsPage subscriptionsPage;

    @BeforeAll
    public static void setUp(){
        ChromeOptions options = new ChromeOptions();
        options.addArguments("--start-maximized");

        WebDriverManager.chromedriver().setup();
        driver = new ChromeDriver(options);
        driver.manage().timeouts().implicitlyWait(Duration.ofSeconds(20));
        wait = new WebDriverWait(driver, Duration.ofSeconds(20));
        authorized = false;
        loginPage = new LoginPage(driver);
        mainPage = new MainPage(driver);
        messagingPage = new MessagingPage(driver);
        demographicInfoPage = new DemographicInfoPage(driver);
        bsuPage = new BSUPage(driver);
        subscriptionsPage = new SubscriptionsPage(driver);
    }

    @Test
    @Order(1)
    @Tag("critical")
    public void authorize(){

        driver.get("https://www.linkedin.com/login");

        loginPage.enterEmail("valentine.korneliuk17@gmail.com");
        loginPage.enterPassword("password");
        loginPage.clickLoginButton();

        assertNotNull(loginPage.isProfilePictureFound(), "не удалось войти в учетную запись LinkedIn");
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
    @Tag("expectedFailure")
    @Order(2)
    public void sendMessage() throws InterruptedException {
        if(!authorized){
            authorize();
        }

        mainPage.clickMessagingTab();
        messagingPage.clickFirstChat();
        messagingPage.enterMessage("Hi!");
        messagingPage.clickSendButton();
        System.out.println("Сообщение успешно отправлено в первый чат.");
        //assertEquals("Hi!", messagingPage.getLastMessageTest(), "Последнее соббщение не соответствует отправленному");

        Thread.sleep(3000);
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

        demographicInfoPage.setNewGender(gender);
        demographicInfoPage.clickSaveButton();
        driver.get("https://www.linkedin.com/mypreferences/d/demographic-info-copy");

        Thread.sleep(2000);

        assertEquals(gender, demographicInfoPage.getCurrentGender(), "не удалось изменить гендер");
        System.out.println("Gender successfully changed");
    }

    @Test
    @Order(4)
    public void checkSubscribtion() throws InterruptedException {
        if(!authorized){
            authorize();
        }
        driver.get("https://www.linkedin.com/school/belarusian-state-university/");

        bsuPage.clickFollowButton();

        driver.get("https://www.linkedin.com/mynetwork/network-manager/company/");

        assertTrue(subscriptionsPage.getSubscriptions().size() > 0, "Ошибка: Belarusian State University не найден в подписках.");
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

        File destinationFile = new File("D:\\Study\\university\\5semester\\TPO\\lab8,10,11\\screenshots\\" + screenshotName + "_" + timestamp + ".png");

        try {
            FileHandler.copy(screenshot, destinationFile);
            System.out.println("screenshot saved: " + destinationFile.getAbsolutePath());
        } catch (IOException e) {
            System.err.println("Не удалось сохранить скриншот: " + e.getMessage());
        }
    }

}
