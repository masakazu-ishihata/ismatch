#include "main.h"

int main(int argc, char *argv[])
{
  ismatch *m;

  /*------------------------------------*/
  /* arguments */
  /*------------------------------------*/
  /* options */
  char fm[256] = "men.txt";
  char fw[256] = "women.txt";

  /* optarg */
  int opt = 0, opt_idx = 0;
  char *opts = "m:w:Sh";
  struct option long_opts[] = {
    {"men",     1, NULL, 'm'},
    {"women",   1, NULL, 'w'},
    {"show",    0, NULL, 'S'},
    {"help",    0, NULL, 'h'},
    {0, 0, 0, 0}
  };

  /* parsing */
  while((opt = getopt_long(argc, argv, opts, long_opts, &opt_idx)) != EOF){
    switch(opt){
    case 'm': sprintf(fm, "%s", optarg); break;
    case 'w': sprintf(fw, "%s", optarg); break;
    case 'S': SHOW = 1; break;
    case 'h':
    default:
      printf("Usage : ./iflow [options]\n");
      printf("    --men,      -m [string] : men's ranking file\n");
      printf("    --women,    -w [string] : women's ranking file\n");
      printf("    --show,     -S          : show log\n");
      printf("    --help,     -h          : show this message\n");
      exit(1);
    }
  }

  /* match */
  m = ismatch_read(fm, fw);
  ismatch_match(m);
  ismatch_free(m);

  return 0;
}
