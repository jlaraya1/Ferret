	LOGICAL FUNCTION TM_ITS_SUBSPAN_MODULO (axis)

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
*
* Return TRUE if the indicated axis is both modulo and has a modulo length
* that is less than (or not equal to) its full span

* V541 *sh* 2/02
* V552 *acm*6/03  Too precise a test. When modulo_axlen is from a cdf file, 
*                 it is single precision. When put into double prec, the last 
*                 several digits have random stuff, so got a return of TRUE  
*                 from this fcn when it wasnt.  Do single-precision comparison.
* V720 *acm* 3/17 Fix ticket 2520. Axis longer than modulo lenght was marked subspan.
*      *acm* 8/18 Call TM_FPEQ_SNGL to make the comparison of axis lengths in 
*                 single precision.

	include 'tmap_dims.parm'
	include	'xtm_grid.cmn_text'

* calling argument declarations
	INTEGER axis

* internal variable declarations
	REAL*8 TM_WW_AXLEN, TM_MODULO_AXLEN
        LOGICAL TM_FPEQ_SNGL
        REAL wwlen, modlen
	REAL*4 rwwlen, rmodlen

	IF ( axis .LT. 0 .OR. axis .GT. line_ceiling ) THEN
	   TM_ITS_SUBSPAN_MODULO = .FALSE.
	ELSEIF ( line_modulo(axis) ) THEN

C for ticket 2520: axis that is slightly too LONG, was marked as subspan!
           wwlen = TM_WW_AXLEN(axis)
           modlen = TM_MODULO_AXLEN(axis) 

	   rwwlen = wwlen
	   rmodlen = modlen
	   TM_ITS_SUBSPAN_MODULO = (.NOT. TM_FPEQ_SNGL(rwwlen, rmodlen) )
     .                              .AND. (wwlen.LT.modlen)

	ELSE 
	   TM_ITS_SUBSPAN_MODULO = .FALSE.
	ENDIF

	RETURN
	END

C
C
C

        INTEGER FUNCTION TM_ITS_SUBSPAN_MODULO_INT(axis)
C
C For calling from C since it is dangerous to assume 
C what Fortran LOGICAL and their values are in C.
C
        INTEGER axis
        LOGICAL TM_ITS_SUBSPAN_MODULO

        IF ( TM_ITS_SUBSPAN_MODULO(axis) ) THEN
            TM_ITS_SUBSPAN_MODULO_INT = 1
        ELSE
            TM_ITS_SUBSPAN_MODULO_INT = 0
        ENDIF

        RETURN
        END
