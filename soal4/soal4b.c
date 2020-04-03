
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





