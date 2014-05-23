#ifndef _ISMATCH_H_
#define _ISMATCH_H_

/*------------------------------------*/
/* include */
/*------------------------------------*/
#include <iutil.h>
#include <ilist.h>
#include <icsv.h>

/*------------------------------------*/
/* constant */
/*------------------------------------*/
extern int SHOW;

/*------------------------------------*/
/* ismatch */
/*------------------------------------*/
typedef struct ISMATCH
{
  int n;    /* # guys (girls) */
  int **rm; /* men's ranking matrix*/
  int **rw; /* women's ranking matrix */
  int *m;   /* maching */
} ismatch;

/* new, free, show  */
ismatch *ismatch_read(const char *_fm, const char *_fw);
ismatch *ismatch_new(int _n);
void ismatch_free(ismatch *_ism);
void ismatch_show(FILE *_fp, ismatch *_ism);

/* accessor */
void ismatch_set_rm(ismatch *_ism, int _m, int *_r);
void ismatch_set_rw(ismatch *_ism, int _w, int *_r);

/* match */
void ismatch_match(ismatch *_ism);

#endif
