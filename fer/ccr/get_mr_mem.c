/*
 *
 * Allocate c heap storage needed for the storage of Ferret mrs
 * and pass the pointer to it to an FORTRAN 90
 * routine that will save it in COMMON
 *
 * V702 1/17 *sh* for trac enhancement #2369 -- dynamic memory management
 *
 */

/* F90 pointers are not true pointers.  Instead they are simply names that 
 serve as aliases for normal FORTRAN variables.  The only way I could think
 of to pass the c-malloc'ed pointer into the F90 pointer was to disguise
 it as a normal FORTRAN variable by passing it as a subroutine argument.
*/

#include <stdlib.h>
/*  the relevant definition of "FORTRAN" pulled from
 *   #include "../common/ferret.h"
 */
#ifndef FORTRAN
#ifdef NO_ENTRY_NAME_UNDERSCORES
#define FORTRAN(a) a
#else
#define FORTRAN(a) a##_
#endif
#endif


void FORTRAN(get_mr_mem)( double *index, int *alen, int *status )

/*
  input  - index:  Ferret mr index at which to store the array pointer
  input  - alen:   array length
*/

{

  double *pointer;

  pointer = (double *) malloc(sizeof(double) * (*alen));

  if (pointer)
    {
      FORTRAN(store_mr_ptr) (index, alen, pointer);
      *status = 3;
    }
  else
    *status = 0;

  return;
}
