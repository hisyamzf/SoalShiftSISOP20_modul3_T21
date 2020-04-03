
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<dirent.h>
#include <sys/types.h>
#include<sys/stat.h>
#include<ctype.h>




void *hutan(void *filename){

    char* file=(char*)filename;

    //mendapatkan ekstensi

    char* split= strrchr(filename,'.');
    char dirname[200];

    //ketika fle memiliki ekstensi

    if(split){
        strcpy(dirname,split+1);
        int i;
        for(i=0;i<strlen(dirname);i++){
            dirname[i]=tolower(dirname[i]);
        }
    }

    //ketika file tidak punya ekstensi

    else{
        strcpy(dirname,"Unknown");
    }
    struct stat sb;

    //mengecek apakah directory dengan nama sesuai ekstensi telah ada

    if(!(stat(dirname, &sb)==0 &&S_ISDIR(sb.st_mode))){
        mkdir(dirname,0755);
    }

    //mendapatkan nama file

    char* only_file= strrchr(filename,'/');
    char new[200]="/home/osboxes/modul3/";
    strcat(new,dirname);
    strcat(new,only_file);

    //memindahkan file dari directory lama kedalam directory yang sesuai ekstensinya

    if(rename(filename,new)==0);
}

int main(int argc,char *argv[]){
    int i;
    DIR *dir;
    char dirname[64];

    //Ketika mode -f

    if(strcmp(argv[1],"-f")==0){

        //menjalankan pengelompokan dengan fungsi hutan dengan thread sebanyak file yang diinput

        for(i=2;i<argc;i++){
            pthread_t thread;
            if(pthread_create(&thread,NULL,hutan,(void*)argv[i])==1){
                printf("Thread Error\n");
            }
            pthread_join(thread,NULL);
        }
    }

    else{

        //ketika mode *

        if(argv[1][0]=='*'){

            //membuka working directory sekarang

            dir = opendir("/home/osboxes/modul3/");
            strcpy(dirname,"/home/osboxes/modul3/");
        }

        //ketika mode -d

        else if(strcmp(argv[1],"-d")==0){

            //membuka directory sesuai input

            dir=opendir(argv[2]);
            strcpy(dirname,argv[2]);
        }
        struct dirent *drnt;

        //directory berhasil dibuka

        if(dir){
            char check[200];

            //traverse kedalam directory yang dibuka

            while(drnt = readdir(dir)){
                strcpy(check,dirname);
                strcat(check,"/");
                strcat(check,drnt->d_name);
                struct stat sb;

                //ketika ditemukan file

                if(stat(check, &sb)==0 &&S_ISREG(sb.st_mode)){

                    //membuat thread untuk mengkategorikan file nya dengan fungsi hutan

                    pthread_t thread;
                    if(pthread_create(&thread,NULL,hutan,(void*)check)==1){
                        printf("Thread Error\n");
                    }
                    pthread_join(thread,NULL);
                }

            }
            closedir(dir);
        }
        else{
            puts("cant open directory");
        }
    }
}


