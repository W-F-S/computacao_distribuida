
    #include <syscall.h>

    char msg[]="Aula de SO\n";
    int cnt, len=13;

    int main(){

             syscall(SYS_write, 1, msg, len);

    return 0;
    }






    