#include "ismatch.h"

/* read file */
ismatch *ismatch_read(const char *_fm, const char *_fw)
{
  ismatch *_ism;
  icsv *fm, *fw;
  int nm, nw, n, *r, i, j;

  /* init */
  fm = icsv_new_delimiter(_fm, " ");
  fw = icsv_new_delimiter(_fw, " ");
  nm = icsv_num_line(fm);
  nw = icsv_num_line(fw);
  if(nm > nw) n = nm;
  else        n = nw;
  r = (int *)iary_new(n, sizeof(int));
  _ism = ismatch_new(n);

  /* read men's ranking */
  for(i=0; i<icsv_num_line(fm) && i<n; i++){
    /* read ranking */
    for(j=0; j<n; j++) r[j] = -1;
    for(j=0; j<icsv_num_item(fm, i) && j<n; j++)
      r[j] = atoi( icsv_get(fm, i, j) );

    /* set ranking */
    ismatch_set_rm(_ism, i, r);
  }

  /* read women's ranking */
  for(i=0; i<icsv_num_line(fw) && i<n; i++){
    /* read ranking */
    for(j=0; j<n; j++) r[j] = -1;
    for(j=0; j<icsv_num_item(fw, i) && j<n; j++)
      r[j] = atoi( icsv_get(fw, i, j) );

    /* set ranking */
    ismatch_set_rw(_ism, i, r);
  }

  /* free */
  icsv_free(fm);
  icsv_free(fw);
  free(r);

  return _ism;
}

/* new n x n stable match */
ismatch *ismatch_new(int _n)
{
  ismatch *_ism;
  int i, j;

  _ism = (ismatch *)imalloc(sizeof(ismatch), "new");
  _ism->n = _n;

  /* ranking */
  _ism->rm = (int **)imatrix_new(_n, _n, sizeof(int));
  _ism->rw = (int **)imatrix_new(_n, _n, sizeof(int));
  for(i=0; i<_n; i++){
    for(j=0; j<_n; j++){
      _ism->rm[i][j] = -1;
      _ism->rw[i][j] = -1;
    }
  }

  /* matching */
  _ism->m = (int *)iary_new(_n, sizeof(int));
  for(i=0; i<_n; i++)
    _ism->m[i] = -1;

  return _ism;
}

/* free */
void ismatch_free(ismatch *_ism)
{
  imatrix_free(_ism->n, _ism->rm);
  imatrix_free(_ism->n, _ism->rw);
  free(_ism);
}

/* show */
void ismatch_show(FILE *_fp, ismatch *_ism)
{
  int m, w, i, n = _ism->n;;

  /* men */
  for(m=0; m<n; m++){
    fprintf(_fp, "  man %3d :", m);
    for(i=0; i<n; i++){
      fprintf(_fp, " %3d", _ism->rm[m][i]);
    }
    fprintf(_fp, "\n");
  }

  /* women */
  for(w=0; w<n; w++){
    fprintf(_fp, "woman %3d :", w);
    for(i=0; i<n; i++){
      fprintf(_fp, " %3d", _ism->rw[w][i]);
    }
    fprintf(_fp, "\n");
  }
}

/*------------------------------------*/
/* accessor */
/*------------------------------------*/
void ismatch_set_rm(ismatch *_ism, int _m, int *_r)
{
  int i, n = _ism->n;
  for(i=0; i<n; i++){
    if(0 <= _r[i] && _r[i] < n) _ism->rm[_m][i] = _r[i];
  }
}
void ismatch_set_rw(ismatch *_ism, int _w, int *_r)
{
  int i, n = _ism->n;
  for(i=0; i<n; i++){
    if(0 <= _r[i] && _r[i] < n) _ism->rw[_w][i] = _r[i];
  }
}

/*------------------------------------*/
/* match */
/*------------------------------------*/
void ismatch_match(ismatch *_ism)
{
  int m, w, f, i, *q, n = _ism->n;

  if(SHOW) ismatch_show(stdout, _ism);

  /*------------------------------------*/
  /* init */
  /*------------------------------------*/
  /* woman's preference */
  int **r = (int **)imatrix_new(n, n, sizeof(int));
  for(w=0; w<n; w++){
    for(m=0; m<n; m++){
      r[w][m] = -1;
    }
  }
  for(w=0; w<n; w++){
    for(i=0; i<n; i++){
      m = _ism->rw[w][i]; /* the i-th prefered man */
      if(m >= 0) r[w][m] = i;
    }
  }

  /* enguage lists */
  int *em = (int *)iary_new(n, sizeof(int));
  int *ew = (int *)iary_new(n, sizeof(int));
  for(m=0; m<n; m++) em[m] = -1;
  for(w=0; w<n; w++) ew[w] = -1;

  /* attackable list */
  ilist **a = (ilist **)iary_new(n, sizeof(ilist *));
  for(m=0; m<n; m++){
    a[m] = ilist_new();
    for(i=0; i<n; i++){
      w = _ism->rm[m][i];
      if(w >= 0)  ilist_push(a[m], int_new(w));
    }
  }

  /* single men list */
  ilist *s = ilist_new();
  for(m=0; m<n; m++){
    ilist_push(s, int_new(m));
  }

  /*------------------------------------*/
  /* matching */
  /*------------------------------------*/
  while((q = ilist_shift(s)) != NULL){
    m = *q; free(q); /* man */

    while((q = ilist_shift(a[m])) != NULL){
      w = *q; free(q); /* woman */

      /* propose m to w */
      if(SHOW) printf("%d proposes to %d: ", m, w);

      /* w is free */
      if(r[w][m] != -1 && ew[w] == -1){
        if(SHOW) printf("enguage (%d, %d)\n", m, w);
        em[m] = w;
        ew[w] = m;
      }

      /* w is enguage but m is better than the current fiancee f */
      else if(r[w][m] >= 0 &&  r[w][ (f = ew[w]) ] > r[w][m]){
        if(SHOW) printf("breakup (%d, %d) & enguage (%d, %d)\n", f, w, m, w);
        em[m] = w;
        em[w] = m;
        em[f] = -1;
        ilist_push(s, int_new(f));
      }

      /* */
      else{
        if(SHOW) printf("refused\n");
      }

      /* break if m is engaged */
      if(em[m] >= 0) break;
    }
  }

  /* result */
  for(m=0; m<n; m++){
    _ism->m[m] = em[m];
    if(em[m] >= 0){
      printf("%d %d\n", m, em[m]);
    }
  }

  /*------------------------------------*/
  /* free */
  /*------------------------------------*/
  for(m=0; m<n; m++)
    ilist_free_func(a[m], free);
  free(a);
  ilist_free(s);
  imatrix_free(n, r);
  free(em);
  free(ew);
}
