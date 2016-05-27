
/***************************************************************************
                                                                           *
Copyright 2013 CertiVox UK Ltd.                                           *
                                                                           *
This file is part of CertiVox MIRACL Crypto SDK.                           *
                                                                           *
The CertiVox MIRACL Crypto SDK provides developers with an                 *
extensive and efficient set of cryptographic functions.                    *
For further information about its features and functionalities please      *
refer to http://www.certivox.com                                           *
                                                                           *
* The CertiVox MIRACL Crypto SDK is free software: you can                 *
  redistribute it and/or modify it under the terms of the                  *
  GNU Affero General Public License as published by the                    *
  Free Software Foundation, either version 3 of the License,               *
  or (at your option) any later version.                                   *
                                                                           *
* The CertiVox MIRACL Crypto SDK is distributed in the hope                *
  that it will be useful, but WITHOUT ANY WARRANTY; without even the       *
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. *
  See the GNU Affero General Public License for more details.              *
                                                                           *
* You should have received a copy of the GNU Affero General Public         *
  License along with CertiVox MIRACL Crypto SDK.                           *
  If not, see <http://www.gnu.org/licenses/>.                              *
                                                                           *
You can be released from the requirements of the license by purchasing     *
a commercial license. Buying such a license is mandatory as soon as you    *
develop commercial activities involving the CertiVox MIRACL Crypto SDK     *
without disclosing the source code of your own applications, or shipping   *
the CertiVox MIRACL Crypto SDK with a closed source product.               *
                                                                           *
***************************************************************************/
/*
 *   MIRACL flash number builder: uses generator of
 *   regular continued fraction expansion to create
 *   a flash number, rounded if necessary.
 *   mrbuild.c
 */

#include "miracl.h"

#ifdef MR_FLASH

void build(_MIPD_ flash x,int (*gen)(_MIPT_ big,int))
{ /* Build x from its regular c.f. *
   * generated by gen()            */
    mr_small ex1,ex2,ex,st,sr;
    int a,b,c,d,rm,q,n,prc,lw2,lw4,lz;
    BOOL finoff,last;
    big t;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return;

    MR_IN(48)

    zero(mr_mip->w1);
    convert(_MIPP_ 1,mr_mip->w2);
    convert(_MIPP_ 1,mr_mip->w3);
    zero(mr_mip->w4);
    finoff=FALSE;
    last=FALSE;
    n=0;
    q=(*gen)(_MIPP_ x,n);   /* Note - first quotient may be zero */
    ex=mr_mip->base-1;
    if (mr_mip->nib==mr_mip->workprec) prc=mr_mip->nib;
    else prc=mr_mip->workprec+1;
    while (!mr_mip->ERNUM && q>=0)
    {
        if (q==MR_TOOBIG || n==0 || finoff)
        {
            if (q!=MR_TOOBIG) convert(_MIPP_ q,x);
            else last=FALSE;
            mr_mip->check=OFF;
            multiply(_MIPP_ mr_mip->w2,x,mr_mip->w0);
            subtract(_MIPP_ mr_mip->w1,mr_mip->w0,mr_mip->w7);
            mr_mip->check=ON;
            if ((int)(mr_mip->w7->len&MR_OBITS)>mr_mip->nib) break;
            copy(mr_mip->w7,mr_mip->w1);
            t=mr_mip->w1,mr_mip->w1=mr_mip->w2,mr_mip->w2=t;   /* swap(w1,w2) */
            mr_mip->check=OFF;
            multiply(_MIPP_ mr_mip->w4,x,mr_mip->w0);
            subtract(_MIPP_ mr_mip->w3,mr_mip->w0,mr_mip->w7);
            mr_mip->check=ON;
            if ((int)(mr_mip->w7->len&MR_OBITS)>mr_mip->nib)
            { /* oops! */
                fpack(_MIPP_ mr_mip->w1,mr_mip->w4,x);
                negify(x,x);
                mr_mip->EXACT=FALSE;
                MR_OUT
                return;
            }
            copy(mr_mip->w7,mr_mip->w3);
            t=mr_mip->w3,mr_mip->w3=mr_mip->w4,mr_mip->w4=t;   /* swap(w3,w4) */
            n++;
        }
        lw2=(int)(mr_mip->w2->len&MR_OBITS);
        lw4=(int)(mr_mip->w4->len&MR_OBITS);
        lz=lw2+lw4;
        if (lz > prc) break;  /* too big - exit */
        if (last)
        {
            if (finoff) break;
            finoff=TRUE;
            q=(*gen)(_MIPP_ x,n);
            continue;
        }
        if (lz>=prc-1)
        { /* nearly finished - so be careful not to overshoot */
            if (mr_mip->base==0)
            {
#ifndef MR_NOFULLWIDTH
                st=mr_mip->w2->w[lw2-1]+1;
                if (st==0) ex1=1;
                else ex1=muldvm((mr_small)1,(mr_small)0,st,&sr);
                st=mr_mip->w4->w[lw4-1]+1;
                if (st==0) ex2=1;
                else ex2=muldvm((mr_small)1,(mr_small)0,st,&sr);
#endif
            }
            else
            {
                ex1=mr_mip->base/(mr_mip->w2->w[lw2-1]+1);
                ex2=mr_mip->base/(mr_mip->w4->w[lw4-1]+1);
            }
            if (ex2>ex1) ex=ex1,ex1=ex2,ex2=ex;
            if (lz==prc) ex=ex2;
            else         ex=ex1;
            last=TRUE;
        }
        a=1;
        b=0;
        c=0;
        d=1;
        forever
        {
            q=(*gen)(_MIPP_ x,n);          
            if (q<0 || q>=MR_TOOBIG/mr_abs(d))
            { /* there could be more.... *** V3.21 mod *** */
                last=FALSE;
                break;
            }
            rm=b-q*d;
            b=d;
            d=rm;
            rm=a-q*c;
            a=c;
            c=rm;
            n++;
            if ((mr_small)(mr_abs(c-d))>ex) break;
        }
        premult(_MIPP_ mr_mip->w1,c,mr_mip->w7);
        premult(_MIPP_ mr_mip->w1,a,mr_mip->w1);
        premult(_MIPP_ mr_mip->w2,b,mr_mip->w0);
        premult(_MIPP_ mr_mip->w2,d,mr_mip->w2);
        add(_MIPP_ mr_mip->w1,mr_mip->w0,mr_mip->w1);
        add(_MIPP_ mr_mip->w2,mr_mip->w7,mr_mip->w2);
        premult(_MIPP_ mr_mip->w3,c,mr_mip->w7);
        premult(_MIPP_ mr_mip->w3,a,mr_mip->w3);
        premult(_MIPP_ mr_mip->w4,b,mr_mip->w0);
        premult(_MIPP_ mr_mip->w4,d,mr_mip->w4);
        add(_MIPP_ mr_mip->w3,mr_mip->w0,mr_mip->w3);
        add(_MIPP_ mr_mip->w4,mr_mip->w7,mr_mip->w4);
    }
    if (fit(mr_mip->w2,mr_mip->w4,mr_mip->nib)) fpack(_MIPP_ mr_mip->w2,mr_mip->w4,x);
    else                            fpack(_MIPP_ mr_mip->w1,mr_mip->w3,x);
    negify (x,x);
    if (q!=(-1)) mr_mip->EXACT=FALSE;
    MR_OUT
}

#endif

