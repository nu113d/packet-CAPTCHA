//This is the worst code I ever wrote. If you are a future employer please ignore this.
//Run ONLY THIS program. Ignore server.exe
//gcc captcha.c -o captcha

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>

#define USER_BUFFER 256

//starts server.exe
void setup_server(int port, int secret) {
    char command[100];
    sprintf(command, "server.exe %d %d", port, secret);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,          
        command,                      
        NULL,                         
        NULL,                         
        FALSE,                        
        CREATE_NEW_CONSOLE,           
        NULL,                         
        NULL,                         
        &si,                          
        &pi)                          
    ) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    srand(time(NULL));
    int port = rand() % 10000;
    int secret = rand() % 100000000;
    char server_buffer[25];
    char user_input[USER_BUFFER];
    
    setup_server(port, secret);
    printf("CAPTCHA: Write the correct netcat command to connect to the local TCP server\n");
    printf("The port you need to connect is %d \n", port);
    printf("Also provide the following secret number as verification. %d \n", secret);
    printf("Use ONLY ONE command. To send the secret number use pipes or redirects to the netcat command\n");
    printf("You have 30 seconds\n"); //Well..no but I have to meet the requirements for the challenge
    printf("Here is some help for netcat\n");
    system("nc.exe -h");

    fgets(user_input, USER_BUFFER, stdin);
    system(user_input); //Scary

    //if everything is ok the netcat will terminate and we can proceed. Else we wait forever
    return 0;
}
