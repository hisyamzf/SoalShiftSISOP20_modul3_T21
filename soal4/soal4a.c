
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







