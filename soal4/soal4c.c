
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




