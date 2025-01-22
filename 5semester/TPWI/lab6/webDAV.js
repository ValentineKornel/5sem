import { createClient } from 'webdav';

import fs from "fs/promises";

const username = "valentine.korneliuk17@gmail.com";
const password = "dwmf6ra4lau3tvbh";
const koofrUrl = `https://app.koofr.net/dav`;
const client = createClient(koofrUrl, { username, password });

async function createFolder(folderPath) {
    try {
        await client.createDirectory(folderPath);
        console.log(`Папка '${folderPath}' создана.`);
    } catch (err) {
        console.error(`Ошибка создания папки: ${err.message}`);
    }
}

async function uploadFile(localPath, remotePath) {
    try {
        const fileContent = await fs.readFile(localPath);
        await client.putFileContents(remotePath, fileContent);
        console.log(`Файл '${localPath}' загружен в '${remotePath}'.`);
    } catch (err) {
        console.error(`Ошибка выгрузки файла: ${err.message}`);
    }
}

async function downloadFile(remotePath, localPath) {
    try {
        const fileContent = await client.getFileContents(remotePath);
        await fs.writeFile(localPath, fileContent);
        console.log(`Файл '${remotePath}' загружен как '${localPath}'.`);
    } catch (err) {
        console.error(`Ошибка загрузки файла: ${err.message}`);
    }
}

async function copyFile(sourcePath, destinationPath) {
    try {
        await client.copyFile(sourcePath, destinationPath);
        console.log(`Файл скопирован из '${sourcePath}' в '${destinationPath}'.`);
    } catch (err) {
        console.error(`Ошибка копирования файла: ${err.message}`);
    }
}

async function deleteFile(filePath) {
    try {
        await client.deleteFile(filePath);
        console.log(`Файл '${filePath}' удалён.`);
    } catch (err) {
        console.error(`Ошибка удаления файла: ${err.message}`);
    }
}

async function deleteFolder(folderPath) {
    try {
        await client.deleteFile(folderPath);
        console.log(`Папка '${folderPath}' удалена.`);
    } catch (err) {
        console.error(`Ошибка удаления папки: ${err.message}`);
    }
}

(async () => {
    const folderPath = "koofr/test-folder";
    const localFilePath = "./test.txt";
    const remoteFilePath = `/${folderPath}/test.txt`;
    const copyDestinationPath = `/${folderPath}/copy-test.txt`;

    // Проверка наличия локального файла
    try {
        await fs.access(localFilePath);
    } catch {
        console.error(`Локальный файл '${localFilePath}' не существует. Создайте его для тестирования.`);
        return;
    }

    await createFolder(folderPath);
    await uploadFile(localFilePath, remoteFilePath);
    const downloadDestination = "./downloaded-test.txt";
    await downloadFile(remoteFilePath, downloadDestination);
    await copyFile(remoteFilePath, copyDestinationPath);
    await deleteFile(copyDestinationPath);
    await deleteFolder(folderPath);
})();
