#include<stdio.h>
#include<fcntl.h>
#include<stdint.h>
#include<asm/unistd.h>
#include<unistd.h>

void write_hello(int filedes_arg) {
  int filedes = filedes_arg;
  int syscall = __NR_write;
  ssize_t written_bytes;
  char msg[] = "Hello there.\n";
  size_t size = sizeof(msg);


  __asm__(
      "mov %[syscall], %%eax;"
      "mov %[filedes], %%edi;"
      "mov %[msg], %%rsi;"
      "mov %[size], %%rdx;"
      "syscall;"
      "mov %%rax, %0"
      :"=r"(written_bytes)
      :[filedes]"r"(filedes),[syscall]"r"(syscall),[msg]"r"(msg),[size]"r"(size)
      :"%eax","%edi","%rsi","%rdx","%rax");
  printf("Written %i bytes of %i bytes.\n", written_bytes, size);
}

int main(int argc, char *argv[]) {
  int fdes = open("test.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if(fdes < 0) {
    printf("Something went wrong!\n");
  } else {
    write_hello(fdes);
    close(fdes);
  }
}
