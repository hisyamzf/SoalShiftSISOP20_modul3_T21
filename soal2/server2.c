
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define PORT 8080



int main(){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);

    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);

    }



    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);

    }


    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);

    }


    char command[64] = {0};
    char iter[64];
    char username[64] = {0};
    char password[64] = {0};
    char message[64] = {0};
    int flag,i;


    FILE *file;
    

    while(1){

        file = fopen("akun.txt","a+");
        memset(command,0,sizeof(command));
        memset(username,0,sizeof(username));
        memset(password,0,sizeof(password));
        

        //terima inputan command

        read( new_socket , command, 64);
        

        //terima input username dan password

        read ( new_socket, username, 64);
        read ( new_socket, password, 64);
        username[strlen(username)]='\n';
        password[strlen(password)]='\n';
        

        //ketika command login

        if(tolower(command[0])=='l'){
            flag=0;

            //iterasi kedalam file akun.txt untuk multi autentikasi

            while(fgets(iter,64,file)){
                if(strcmp(iter,username)==0){
                    fgets(iter,64,file);
                    if(strcmp(iter,password)==0){
                        flag=1;
                        break;
                    }
                }
            }
            if(flag==1){
                printf("Auth success\n");
                send( new_socket, "login success",strlen("login success"),0);
            }
            else{
                printf("Auth failed\n");
                send( new_socket, "login failed",strlen("login failed"),0);
            }
            
        }

        //ketika command register

        else if(command[0]=='r'){
            i=0;

            //iterasi untuk mencetak username dan password yang telah ada

            while(fgets(iter,64,file)){
                printf("%d. username : %s",++i,iter);
                fgets(iter,64,file);
                printf("password : %s",iter);
            }

            //menuliskan username dan password baru

            fwrite(username,1,strlen(username),file);
            fwrite(password,1,strlen(password),file);
            
            
            send( new_socket, "register success",strlen("register success"),0);
        }

        //command untuk keluar dari game

        else{
            break;
        }
        
        fclose(file);
    }

    
    return 0;
}



