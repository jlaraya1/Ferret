	SUBROUTINE FIND_MR_SLOT( mr, status )

*
*
*  This software was developed by the Thermal Modeling and Analysis
*  Project(TMAP) of the National Oceanographic and Atmospheric
*  Administration''s (NOAA) Pacific Marine Environmental Lab(PMEL),
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
*
* locate an unused slot in the memory variable table and assign it to the
* hash pile associated with the given context (cx)

* programmer - steve hankin
* NOAA/PMEL, Seattle, WA - Tropical Modeling and Analysis Program
* written for VAX computer under VMS operating system
*
* revision 0.0 - 7/21/88
* V200:  6/12/89 - delete priorities handled through linked lists
*        2/12 *kms* Add E and F dimensions (use nferdims in tmap_dims.parm)
* V702 *sh* 1/17 - split half of the code into the new HOOK_MR_SLOT
*                  as part of the new dynamic memory management

        IMPLICIT NONE
        include 'tmap_dims.parm'
	include 'ferret.parm'
	include 'errmsg.parm'
	include 'xvariables.cmn'
	include 'xcontext.cmn'

* calling argument declarations:
	INTEGER mr, status

* internal variable declarations:

*****

* start with the first entry on the free mr list
	mr = mv_flink( 0 )

* if it isn't free, then free the most vulnerable mr on the delete chain
	IF ( mr .EQ. 0 ) THEN
	   mr = mr_del_blink(0)
	   IF ( mr .EQ. 0 ) GOTO 5010
	   CALL DELETE_VARIABLE( mr )
	ENDIF

* remove it from the free list to indicate 
*    ... remove from free list head
!	mr = mv_flink( 0 )
	mv_flink( 0 ) = mv_flink( mr )

* successful completion
	status = ferr_ok
 5000	RETURN

* error exit : no space available (empty delete chain)
 5010	CALL ERRMSG( ferr_too_many_vars, status,' ', *5000 )
	END
