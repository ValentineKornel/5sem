#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

void ListDirectoryContents(const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("Error: Directory not found or access denied");
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем текущий и родительский каталоги (".", "..")
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Определяем, является ли текущая запись каталогом или файлом
        const char* type = (entry->d_type == DT_DIR) ? "Directory" : "File";
        printf("%-12s %s/%s\n", type, path, entry->d_name);

        // Если это каталог, вызываем функцию рекурсивно
        if (entry->d_type == DT_DIR) {
            char subPath[PATH_MAX];
            snprintf(subPath, PATH_MAX, "%s/%s", path, entry->d_name);
            ListDirectoryContents(subPath);
        }
    }

    closedir(dir);
}

void WatchDirectory(const char* path) {
    int fd = inotify_init();
    if (fd == -1) {
        perror("Error: Could not initialize inotify");
        exit(1);
    }

    int wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
    if (wd == -1) {
        perror("Error: Could not add watch to directory");
        close(fd);
        exit(1);
    }

    char buffer[1024];
    while (1) {
        ssize_t length = read(fd, buffer, sizeof(buffer));
        if (length == -1) {
            perror("Error: Could not read directory changes");
            break;
        }

        for (char* ptr = buffer; ptr < buffer + length; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;

            if (event->mask & IN_ACCESS) {
                printf("File was accessed: %s\n", event->name);
            }
            if (event->mask & IN_MODIFY) {
                printf("File was modified: %s\n", event->name);
            }
            if (event->mask & IN_ATTRIB) {
                printf("Metadata changed: %s\n", event->name);
            }
            if (event->mask & IN_CLOSE_WRITE) {
                printf("Writtable file was closed: %s\n", event->name);
            }
            if (event->mask & IN_OPEN) {
                printf("File was opened: %s\n", event->name);
            }
            if (event->mask & IN_MOVED_FROM) {
                printf("File was moved from: %s\n", event->name);
            }
            if (event->mask & IN_MOVED_TO) {
                printf("File was moved to: %s\n", event->name);
            }
            if (event->mask & IN_CREATE) {
                printf("Subfile was created: %s\n", event->name);
            }
            if (event->mask & IN_DELETE) {
                printf("Subfile was deleted: %s\n", event->name);
            }
            if (event->mask & IN_DELETE_SELF) {
                printf("Self was deleted: %s\n", event->name);
            }
            if (event->mask & IN_MOVE_SELF) {
                printf("Self was moved: %s\n", event->name);
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    const char* directoryPath = argv[1];

    ListDirectoryContents(directoryPath);

    printf("\nWatching directory for changes...\n");
    WatchDirectory(directoryPath);

    return 0;
}
