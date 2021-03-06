/*
*
*  This software was developed by the Thermal Modeling and Analysis
*  Project(TMAP) of the National Oceanographic and Atmospheric
*  Administration's (NOAA) Pacific Marine Environmental Lab(PMEL),
*  hereafter referred to as NOAA/PMEL/TMAP.
*
*  Access and use of this software shall impose the following
*  obligations and understandings on the user. The user is granted the
*  right, without any fee or cost, to use, copy, modify, alter, enhance
*  and distribute this software, and any derivative works thereof, and
*  its supporting documentation for any purpose whatsoever, provided
*  that this entire notice appears in all copies of the software,
*  derivative works and supporting documentation.  Further, the user
*  agrees to credit NOAA/PMEL/TMAP in any publications that result from
*  the use of this software or in any product that includes this
*  software. The names TMAP, NOAA and/or PMEL, however, may not be used
*  in any advertising or publicity to endorse or promote any products
*  or commercial entity unless specific written permission is obtained
*  from NOAA/PMEL/TMAP. The user also understands that NOAA/PMEL/TMAP
*  is not obligated to provide the user with any support, consulting,
*  training or assistance of any kind with regard to the use, operation
*  and performance of this software nor to provide the user with any
*  updates, revisions, new versions or "bug fixes".
*
*  THIS SOFTWARE IS PROVIDED BY NOAA/PMEL/TMAP "AS IS" AND ANY EXPRESS
*  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL NOAA/PMEL/TMAP BE LIABLE FOR ANY SPECIAL,
*  INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
*  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
*  CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN
*  CONNECTION WITH THE ACCESS, USE OR PERFORMANCE OF THIS SOFTWARE.  
*
*/



/*
 * put_frame( ws_id, filename, status )
 * dump an XGKS window as a GIF file
 *
 * programmer - steve hankin
 * NOAA/PMEL, Seattle, WA - Tropical Modeling and Analysis Program
 *
 * revision 0.0 - 8/03/94
 * 
 * *kob* 5/25/95 replaced defunct ifdef confition AIX_XLF with current 
 *               NO_ENTRY_NAME_UNDERSCORES
 * *js* 9.97 added put_frame_batch 
 *
 * *jli* 5/08 changed the parameters of put_frame_batch() for making 
 *            transparent images
 *
 * *acm*  1/12 - Ferret 6.8 ifdef double_p for double-precision ferret,
 *               see the definition of macro DFTYPE in ferret.h
 * *acm* 12/13 - Removed unused routine put_temp_frame_batch
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include "gks_implem.h"
#include "ferret.h"

void FORTRAN(put_frame)(int *ws_id, char *filename, char *errstr, char *format, int *status)
{
  WS_STATE_ENTRY *ws;

/* determine the XGKS ws state entry structure from ws_id */
  ws  = OPEN_WSID (*ws_id);	

/* call up the capture routine */
/* Errors internal to Window_Dump com out in errstr */
   Window_Dump(ws->win,ws->dpy,filename,format);
   *status = 0;   /* not much use, really */

   return;
}

/*  acm create separate GIFFlush routines for transparency. Passing the argumetns transp, red, green, blue
    in direcly into GIFFlush as arguments did not work on porter.
*/
void FORTRAN(put_frame_batch)(int *ws_id, char *filename, char *format, int *transp, 
                              DFTYPE *red, DFTYPE *green, DFTYPE *blue, char *errmsg, int *status)
{
  WS_STATE_ENTRY *ws = OPEN_WSID(*ws_id);
  *status = 0;

  if (ws == 0 || ws->mf.any == 0){
    strcpy(errmsg, "No open workstations for batch FRAME command");
    return;
  }

  if (ws->mf.any->type != MF_GIF){
    strcpy(errmsg, "Batch FRAME only works for GIF files");
    return;
  }
/*
if (GIFFlush(&ws->mf, filename) != OK){

  if (GIFFlush(&ws->mf, filename, transparent_color) != OK){
*/
  if(*transp > 0)
    { 
/* white background was used */
	  if(((int)*red == 0) && ((int)*green == 0) && ((int)*blue == 0))
       {
		  if (GIFFlusht0(&ws->mf, filename) != OK)
	      {
            sprintf(errmsg, "Couldn't write out GIF file %s\n", filename);
            return;
          }
	   }
/* black background was used */
	  if(((int)*red == 1) && ((int)*green == 1) && ((int)*blue == 1))
       {
		  if (GIFFlusht1(&ws->mf, filename) != OK)
	      {
            sprintf(errmsg, "Couldn't write out GIF file %s\n", filename);
            return;
          }
	   }
    }
	else
/* not transparent */

		if (GIFFlush(&ws->mf, filename) != OK)
		{
         sprintf(errmsg, "Couldn't write out GIF file %s\n", filename);
         return;
		{
	} 
   }

}



