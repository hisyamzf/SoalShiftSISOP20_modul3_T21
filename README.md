# SoalShiftSISOP20_modul3_T21


Soal Shift Sistem Operasi 2020


Hisyam Zulkarnain F             05311840000019\
Bayu Trianayasa                 05311840000038
## #2 &ndash; Client Side
> Source code: [client2.c](https://github.com/hisyamzf/SoalShiftSISOP20_modul3_T21/blob/master/soal2/client2.c)

---


```

#include <stdio.h>
#include <ctype.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#define PORT 8080



int my_health;
int enemy_health;
int before;

int getch(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int kbhit(void){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
    ungetc(ch, stdin);
    return 1;
    }

    return 0;
}


void *langit(void *args){

    //generate tap persecond secara random antara 10-15 tap

    double time=1/(double)((random()%10)+5);
    clock_t t=clock();
    before=my_health+1;
    while(my_health>0&&enemy_health>0){

        //ketika waktu yang dilampaui telah lebih/sama dengan tap/second

        if(((double)(clock()-t)/CLOCKS_PER_SEC)>=time){
            my_health-=1;
            t=clock();
        }
    }
}


void *gurun(void *arg){
    int flag=1,i;
    char command[64];
    

    //ketika belum logout

    while(flag){
        puts("1. Find Match\n2. Logout");
        printf("%s","Choices : ");
        

        //mendapatkan input

        gets(command);
        
        //input find match

        if(tolower(command[0])=='f'){

            //waiting time didapatkan dari angka random 1-10 detik

            int time=random()%10;
            while(time>0){
                puts("Waiting For Player ....");
                sleep(1);
                time--;
            }
            my_health=100;
            enemy_health=100;
            int status;
            if(fork()==0){
                execlp("clear","clear",(char *)NULL);
            }
            while((wait(&status)>0));
            
            //Game dimulai dengan menjalankan fungsi langit dengan thread untuk enemy

            puts("game dimulai silahkan taptap secepat mungkin !!!!");
            pthread_t trd2;
            if(pthread_create(&(trd2),NULL,&langit,NULL)==1){
                printf("Thread %d Error\n",i);
            }
            while(my_health>0&&enemy_health>0){

                //ketika tidak tap

                if(!kbhit()){

                    //ketika darah kita berkurang

                    if(before-my_health==2){
                        before-=1;
                        printf("\t\t\t\t\t\t\t%d\n",my_health);
                    }
                }

                //ketika tap

                else if(getch()==32){
                    if(before-my_health==2){
                        before-=1;
                    }
                    printf("HIT!!\t\t\t\t\t\t\t%d\n",my_health);
                    enemy_health--;
                }
            }
            printf("%d\n",my_health);

            //pengecekan pemenang

            if(my_health>0){
                puts("Game berakhir kamu menang");
            }
            else{
                puts("Game berakhir kamu kalah");
            }
            pthread_join(trd2,NULL);
            sleep(4);
        }
        
        //input logout

        else{
            flag=0;
        }
        int status;
        if(fork()==0){
            execlp("clear","clear",(char *)NULL);
        }
        while((wait(&status)>0));
    }
    
}

int main() {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    int status;
    if(fork()==0){
        execlp("clear","clear",(char *)NULL);
    }
    while((wait(&status)>0));

    char command[64];
    char username[64];
    char password[64];
    char message[64] = {0};
    srandom(time(NULL));

    while(1){
        puts("1. Login\n2. Register");
        printf("%s","Choices : ");
        
        //mendapatkan input command

        gets(command);

        send(sock,command,strlen(command),0);
        if(command[0]=='q')break;
        
        //mendapatkan username

        gets(username);
        send(sock,username,strlen(username),0);
        
        //mendapatkan password

        gets(password);
        send(sock,password,strlen(password),0);

        //mendapatkan feedback dari server

        read(sock,message,64);

        int status;
        if(fork()==0){
            execlp("clear","clear",(char *)NULL);
        }
        while((wait(&status)>0));
        puts(message);
        
        //ketika login success

        if(strcmp(message,"login success")==0){

            //menjalankan fungsi gurun dengan thread untuk screen 2

            pthread_t trd1;
            if(pthread_create(&trd1,NULL,&gurun,NULL)==1){
                printf("Thread Error\n");
            }
            pthread_join(trd1,NULL);
        }
        
        memset(message,0,64);
    }
    return 0;    
}

 ```
 
 ## #2 &ndash; Server Side

> Source code: [server2.c](https://github.com/hisyamzf/SoalShiftSISOP20_modul3_T21/blob/master/soal2/server2.c)

---

```

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


```

 ## #3 &ndash; Kategori File
> Source code: [soal3.c](https://github.com/hisyamzf/SoalShiftSISOP20_modul3_T21/blob/master/soal3/soal3.c)

---

```

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

```


 ## #4 &ndash; Batu Pertama
> Source code: [soal4a.c](https://github.com/hisyamzf/SoalShiftSISOP20_modul3_T21/blob/master/soal4/soal4a.c)

---

```

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>


int mx1[5][5], mx2[5][5];
int (*ans)[10];
int i,j;
int b1=4, k1=2;//matriks 4x2
int b2=2, k2=5;//matriks 2x5
int stat;

//fungsi untuk mengisi matriks1 dan matriks2 dan perkalian matriks
void *kali_mx(void *);
void *isi_mx1(void *);
void *isi_mx2(void *);

void *isi_mx1(void *arg){
	stat=0;
    mx1[0][0]=1;
    mx1[0][1]=2;
    mx1[1][0]=3;
    mx1[1][1]=4;
    mx1[2][0]=5;
    mx1[2][1]=6;
    mx1[3][0]=7;
    mx1[3][1]=8;
	stat=1;
	return NULL;
}

void *isi_mx2(void *arg){
	while(stat!=1){
	}
    mx2[0][0]=1;
    mx2[0][1]=2;
    mx2[0][2]=3;
    mx2[0][3]=4;
    mx2[0][4]=5;
    mx2[1][0]=6;
    mx2[1][1]=7;
    mx2[1][2]=8;
    mx2[1][3]=9;
    mx2[1][4]=10;

	stat=2;
	return NULL;
}

void *kali_mx(void *arg)
{
	while(stat!=2)	{
	}
	int i, j, k;
	for(i=0;i<b1;i++){
		for(j=0;j<k2;j++){
			for(k=0;k<k1;k++)
				ans[i][j]+=mx1[i][k] * mx2[k][j];
		}
	}
	return NULL;
}

//fungsi untuk meng output hasil
void print(){
	printf("Hasil dari perkalian matriks: \n");
	for(i=0;i<b1;i++){
		for(j=0;j<k2;j++)
		{
			printf("%d\t", ans[i][j]);
		}
		printf("\n");
	}
}

pthread_t t1, t2, t3;

int main(){
	int i, j;
	key_t key = 1234;
        int shmid = shmget(key, sizeof(int[10][10]), IPC_CREAT | 0666);
        ans = shmat(shmid, 0, 0);

	pthread_create(&t1, NULL, isi_mx1, NULL);
	pthread_join(t1,NULL);

	pthread_create(&t2, NULL, isi_mx2, NULL);
	pthread_join(t2,NULL);

	for(i=0;i<b1;i++)
	{
		for(j=0;j<k2;j++)
		{
			ans[i][j]=0;
		}
		pthread_create(&t3, NULL, kali_mx, NULL);
		pthread_join(t3,NULL);
	}

	print();
	return 0;
}



```
 ## #4 &ndash; Batu Kedua
> Source code: [soal4b.c](https://github.com/hisyamzf/SoalShiftSISOP20_modul3_T21/blob/master/soal4/soal4b.c)

---

```

#include <stdio.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


unsigned long long num;
int b=4, k=5;
int i,j;
int (*ans)[10];



unsigned long long mfaktorial(unsigned long long x){
    if(x==0 || x==1) return 1;
    else return x+ mfaktorial(x-1);

}


//fungsi untuk faktorial penjumlahan matriks
void *faktorial(void *arg)
{
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    ans = shmat(shmid, 0, 0);
   	printf("Hasil faktorial penjumlahan matriks \n");
   	for(int i=0;i<b;i++){
 		for(int j=0;j<k;j++){
           	num=ans[i][j];
   			printf("%llu\t", mfaktorial(num));
  			}
  		printf("\n");
 	}
    pthread_exit(0);

}


//fungsi untuk output program sebelumnya yaitu perkalian matriks

void print_kali(){
	printf("Hasil dari perkalian matriks \n");
    for(i=0;i< 4;i++){
        for(j=0;j<5;j++){
        printf("%d\t", ans[i][j]);
        }
        printf("\n");
    }	
}


void main()

{	

    pthread_t td;
    key_t key = 1234;
    int (*ans)[10];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    ans = shmat(shmid, 0, 0);
	
	print_kali();
	
    pthread_create(&td, NULL, faktorial, NULL);
    pthread_join(td,NULL);

}




```

 ## #4 &ndash; Batu Ketiga
> Source code: [soal4c.c](https://github.com/hisyamzf/SoalShiftSISOP20_modul3_T21/blob/master/soal4/soal4c.c)

---

```

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>



int main(int argc, char **argv)

{

  int pipex[2];
  int p;

  char *wc_args[] = {"wc", "-l", NULL};
  char *ls_args[] = {"ls", NULL};

  //membuat pipe

  pipe(pipex);
  p = fork();

  if (p == 0)

    {

      //mengganti standard input dengan input bagian dari pipe			

      dup2(pipex[0], 0);
      close(pipex[1]);
      execvp("wc", wc_args);

    }

  else

    {

      // mengganti standard output dengan output bagian dari pipe

      dup2(pipex[1], 1);
      close(pipex[0]);
      execvp("ls", ls_args);

    }

}



```
