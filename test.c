#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096

char *getPRocesses()
{
    DWORD processIds[1024];
    DWORD bytesReturned;
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned))
    {
        fprintf(stderr, "Error: EnumProcesses failed with error %d\n", GetLastError());
        return NULL;
    }
    int count = bytesReturned / sizeof(DWORD);

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: Out of memory\n");
        return NULL;
    }
    buffer[0] = '\0';

    for (int i = 0; i < count; i++)
    {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
        if (processHandle != NULL)
        {
            char processName[MAX_PATH];
            if (GetModuleFileNameExA(processHandle, NULL, processName, MAX_PATH))
            {
                char processIdString[16];
                snprintf(processIdString, sizeof(processIdString), "%d", processIds[i]);
                strncat(buffer, processIdString, BUFFER_SIZE - strlen(buffer) - 1);
                strncat(buffer, ":", BUFFER_SIZE - strlen(buffer) - 1);
                strncat(buffer, processName, BUFFER_SIZE - strlen(buffer) - 1);
                strncat(buffer, "\n", BUFFER_SIZE - strlen(buffer) - 1);
            }
            CloseHandle(processHandle);
        }
    }

    return buffer;
}

int main()
{
    char *processListString = getPRocesses();
    if (processListString != NULL)
    {
        printf("%s\n", processListString);
        printf("%d\n", strlen(processListString));
        free(processListString);
    }
    return 0;
}