/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "complxop.h"

/*--------------------------------------------------------------------------*/
/*  Function  cmpy3216                                                      */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Multiply complex variables                                              */
/*--------------------------------------------------------------------------*/
/*  complex32   var1 (i)  variable 1                                        */
/*  complex16   var2 (i)  variable 2                                        */
/*  complex32   *res (o)  result                                            */
/*--------------------------------------------------------------------------*/
void cmpy3216(complex32 var1, complex16 var2, complex32 *res)
{
    res->r = L_sub(L_mls(var1.r, var2.r), L_mls(var1.i, var2.i));
    res->i = L_add(L_mls(var1.r, var2.i), L_mls(var1.i, var2.r));
#if(WMOPS)
  move32();
  move32();
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  cmpy3216s                                                     */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Multiply complex variable with a scalar                                 */
/*--------------------------------------------------------------------------*/
/*  complex32   var1 (i)  variable 1                                        */
/*  word16      var2 (i)  variable 2                                        */
/*  complex32   *res (o)  result                                            */
/*--------------------------------------------------------------------------*/
void cmpy3216s(complex32 var1, Word16 var2, complex32 *res)
{
    res->r = L_mls(var1.r, var2);
    res->i = L_mls(var1.i, var2);
#if(WMOPS)
  move32();
  move32();
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  cadd3232                                                      */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Add complex variables                                                   */
/*--------------------------------------------------------------------------*/
/*  complex32   var1 (i)  variable 1                                        */
/*  complex32   var2 (i)  variable 2                                        */
/*  complex32   *res (o)  result of addition                                */
/*--------------------------------------------------------------------------*/
void cadd3232(complex32 var1, complex32 var2, complex32 *res)
{
    res->r = L_add(var1.r,var2.r);
    res->i = L_add(var1.i,var2.i);
#if(WMOPS)
  move32();
  move32();
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  csub3232                                                      */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Subtract complex variables                                              */
/*--------------------------------------------------------------------------*/
/*  complex32   var1 (i)  variable 1                                        */
/*  complex32   var2 (i)  variable 2                                        */
/*  complex32   *res (o)  result of subtraction                             */
/*--------------------------------------------------------------------------*/
void csub3232(complex32 var1, complex32 var2, complex32 *res)
{
    res->r = L_sub(var1.r,var2.r);
    res->i = L_sub(var1.i,var2.i);
#if(WMOPS)
  move32();
  move32();
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  cmpy32j                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Find orthogonal complex variable                                        */
/*--------------------------------------------------------------------------*/
/*  complex32   *v1  (i/o)  variable                                        */
/*--------------------------------------------------------------------------*/
void cmpy32j(complex32* v1)
{
    Word32 a;

    a = v1->r;
    v1->r = L_negate(v1->i);
    v1->i =  a;
#if(WMOPS)
  move32();
  move32();
  move32();
#endif

}
