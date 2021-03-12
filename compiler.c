#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define VERSION "v1.0"
#define MEMORY_SIZE 1024
#define BREAK(a)                                                               \
  {                                                                            \
    a;                                                                         \
    break;                                                                     \
  }

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    perror("No script files");
  }
  printf("BrainFuck %s\n", VERSION);

  FILE *fd;
  struct stat statbuf;
  __int8_t *memory =
      (__int8_t *)malloc(MEMORY_SIZE * sizeof(__int8_t)); // 1K memory
  struct loop_stack {
    __int8_t *mem_rax;
    __uint32_t file_index;
  } loop_stack[36];
  __uint16_t mem_index = 0;
  __uint8_t loop_stack_size;

  stat(argv[1], &statbuf);
  __uint8_t *file = (__uint8_t *)malloc(statbuf.st_size + 1);
  assert(file != NULL);
  *(file + statbuf.st_size) = '\0';

  fd = fopen(argv[1], "r");
  fread(file, statbuf.st_size, 1, fd);
  assert(!fclose(fd));

  for (int i = 0; *(file + i) != '\0'; i++) {
    switch (*(file + i)) {
    case '>':
      BREAK(mem_index++);
    case '<':
      BREAK(mem_index--);
    case '+':
      BREAK((*(memory + mem_index))++);
      break;
    case '-':
      BREAK((*(memory + mem_index))--);
    case '[':
      loop_stack[loop_stack_size].mem_rax = memory + mem_index;
      loop_stack[loop_stack_size].file_index = i;
      BREAK(loop_stack_size++);
    case ']':
      if (loop_stack_size != 0) {
        // TODO ']'
      }
      if (*(loop_stack[loop_stack_size - 1].mem_rax) != 0) {
        i = loop_stack[loop_stack_size - 1].file_index;
        break;
      }
      BREAK(loop_stack_size--);
    case ',':
      memory[mem_index] = getchar();
      break;
    case '.':
      putchar(memory[mem_index]);
      break;
    default:
      // space: '#', ' ', '\n', '\t', '\r'
      for (; file[i] == ' ' || file[i] == '\n' || file[i] == '\t' ||
             file[i] == '\r';
           i++)
        ;
      if (file[i] == '#') {
        for (; file[i] != '\n'; i++)
          ;
      }
      i--;
    }
  }

  return 0;
}
