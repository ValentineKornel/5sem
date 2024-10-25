package org.example;

import io.github.bonigarcia.wdm.WebDriverManager;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.support.ui.WebDriverWait;

import java.time.Duration;
import java.util.List;

public class SeleniumTest {

    public static void main(String[] args) {
        WebDriverManager.chromedriver().setup();

        WebDriver driver = new ChromeDriver();
        driver.get("https://mvnrepository.com/artifact/org.seleniumhq.selenium/selenium-java");

        List<WebElement> cssSelector1 = driver.findElements(By.cssSelector("div.gridcontainer th"));
        System.out.println("CSS Selector 1 (Заголовки в таблице с версиями модуля): " + cssSelector1.size() + " элементов найдено");
        cssSelector1.forEach(el -> System.out.println(el.getText()));
        System.out.println();

        List<WebElement> cssSelector2 = driver.findElements(By.cssSelector("#snippets ul.tabs li a"));
        System.out.println("CSS Selector 2 (Табки расположения репозиториев): " + cssSelector2.size() + " элементов найдено");
        cssSelector2.forEach(el -> System.out.println(el.getText()));
        System.out.println();

        List<WebElement> cssSelector3 = driver.findElements(By.cssSelector("div.tab_container div.tab_content.active div.gridcontainer table.grid.versions td.date"));
        System.out.println("CSS Selector 3 (Даты артефактов): " + cssSelector3.size() + " элементов найдено");
        cssSelector3.forEach(el -> System.out.println(el.getText()));
        System.out.println();

        List<WebElement> xpathSelector1 = driver.findElements(By.xpath("//table[@class='grid']//tbody//tr//th"));
        System.out.println("XPath 1 (Заголовки таблицы информации о модуле): " + xpathSelector1.size() + " элементов найдено");
        xpathSelector1.forEach(el -> System.out.println(el.getText()));

        List<WebElement> xpathSelector2 = driver.findElements(By.xpath("(//table[@class='grid']//tbody//tr)[3]//td//a"));
        System.out.println("XPath 2 (теги связанные с модулем): " + xpathSelector2.size() + " элементов найдено");
        xpathSelector2.forEach(el -> System.out.println(el.getText()));
        System.out.println();

        List<WebElement> xpathSelector3 = driver.findElements(By.xpath("//div[@class='version-section']//div//table[@class='grid']//tbody//tr//td//a"));
        System.out.println("XPath 3 (related книги): " + xpathSelector3.size() + " элементов найдено");
        xpathSelector3.forEach(el -> System.out.println(el.getText()));
        System.out.println();

        List<WebElement> tags = driver.findElements(By.tagName("h3"));
        System.out.println("Поиск по тегу <h3>: " + tags.size() + " элементов найдено");
        tags.forEach(el -> System.out.println(el.getText()));
        System.out.println();

        WebElement link = driver.findElement(By.partialLinkText("Selenium"));
        System.out.println("Ссылка с частичным текстом 'Selenium': " + link.getText());

        driver.quit();
    }
}
