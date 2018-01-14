#ifndef _TBF_H
#define _TBF_H

#define TBFMAX	1024

/*初始化令牌桶-----open()*/
int tbf_init(int cps, int burst);

/*在指定的令牌桶取令牌-----read()*/
int tbf_fetchtoken(int td, int ntoken);

/*向指定的令牌桶还令牌*/
int tbf_returntoken(int td, int ntoken);

/*close()*/
int tbf_close(int td);

void tbf_destroyall(void);

#endif

