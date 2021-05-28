#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#if _WIN64 || __amd64__
#define CAST (unsigned long int)
#define PRINTHEX "0x%016lX"
#else
#define CAST (unsigned  int)
#define PRINTHEX "0x%08X"
#endif

int main(int argc, char **argv)
{
  void *handle;
  void (*func)(int, char **);
  int i = 0;
  char *new_argv[100]; // fsck malloc

  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s path-to-lib func/symbol [args to func] [...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  printf("++ GOING TO START !!\n");
  fflush(stdout);
  handle = dlopen(argv[1], RTLD_LAZY);
  printf("> LIB HANDLE : " PRINTHEX "\n", CAST(handle));
  if (handle == NULL)
  {
    printf("!! THAT IS NULL\n");
  }
  else
  {
    *(void **)(&func) = dlsym(handle, argv[2]);
    if (func == NULL)
    {
      printf("!! THAT IS NULL \n");
    }
    else
    {
      printf("> FUNC PTR : " PRINTHEX "\n", CAST((void *)func));

      new_argv[0] = argv[1];
      // argv[2] == func-name
      for (i = 3; i < argc; i++)
      {
        printf(" ~ rewrite argv %d as %d (%s)\n", i, i - 2, argv[i]);
        new_argv[i - 2] = argv[i];
      }
      printf("\n==============================\n");
      func(argc - 2, new_argv);
    }
    dlclose(handle);
  }

  return EXIT_SUCCESS;
}
