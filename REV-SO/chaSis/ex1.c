char msg[]="Aula de SO\n";
int cnt, len=12;
int main(){

/*Protótipo de uma função para a escrita de caracteres

ssize_t write(int fd, const void *buf, size_t count);

cujo uso poderia ser: write (1, msg, len);*/


__asm__ ("movl len, %edx  #define o tamanho da mensagem \n"
         "movl $msg, %ecx #ponteiro para a mensagem \n"
         "movl $1, %ebx   #descritor utilizado \n"
         "movl $4, %eax   #identificador da chamada (sys_write) \n"
         "int  $0x80      #mudança para modo supervisor \n"
         "movl %eax, cnt  #retorna o no. de bytes transferidos ou -1 em caso de erro \n"
         );
  return 0;
          }
