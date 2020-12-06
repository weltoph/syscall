#include<sys/syscall.h> /* contains the SYS_xxx constants to systemcalls */
#include<unistd.h> /* necessary for type definitions of ssize_t, size_t, ... */

void end_program(int exit_code) {
  int exit_syscall = SYS_exit;

  __asm__(
      "mov %[syscall], %%eax;"
      "mov %[exit_code], %%edi;"
      "syscall"
      :
      :[syscall]"r"(exit_syscall),[exit_code]"r"(exit_code)
      :"%eax","%edi");
}

ssize_t write_syscall(int filedes, const void *buf, size_t count) {
  int syscall = SYS_write;
  ssize_t written_bytes;


  __asm__(
      "mov %[syscall], %%eax;"
      "mov %[filedes], %%edi;"
      "mov %[msg], %%rsi;"
      "mov %[size], %%rdx;"
      "syscall;"
      "mov %%rax, %0"
      :"=r"(written_bytes)
      :[filedes]"r"(filedes),[syscall]"r"(syscall),[msg]"r"(buf),[size]"r"(count)
      :"%eax","%edi","%rsi","%rdx","%rax");

  return written_bytes;
}

size_t strlen(char *msg) {
  size_t ret = 0;
  char *curr = msg;
  while(*curr != 0) {
    ret += 1;
    curr += 1;
  }
  return ret;
}

int entry(int argc, char *argv[]) {
  /* echo command line args */
  for(int i = 1; i < argc; i++) {
    write_syscall(1, argv[i], strlen(argv[i]));
    write_syscall(1, " ", 1);
  }
  write_syscall(1, "\n", 1);
  end_program(0);
}

/* entry point to the program */

__asm__(
    ".global _start;"
    "_start:"

    /* calling convention says we should put zeros in this register */
    "mov $0, %rbp;"

    /* get argc */
    "mov (%rsp), %edi;"

    /* get argv */
    /* computing $rsp + 8 would be
    "mov %rsp, %rax;"
    "add $8, %rax;"
    "mov %rax, %rsi;"
    however, this is equivalent to:
    */
    "lea 8(%rsp), %rsi;"

    /* jump into our main function */
    "call entry;"
    );
