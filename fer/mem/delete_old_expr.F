	SUBROUTINE DELETE_OLD_EXPR

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
* delete from memory all "expression" variables - internally defined user-vars
* that were created by expressions given with the commmand line

* programmer - steve hankin
* NOAA/PMEL, Seattle, WA - Tropical Modeling and Analysis Program
* written for VAX computer under VMS operating system
*
* revision 0.0 - 10/31/86
* revision 0.1 - 1/30/87 - modified for mr_protected declared as INTEGER
* revision 1.0 - 2/23/87 - changes for conversion to INTERPRETATION STACK using
*			   the idea that all expressions are special cases of
*			   user-defined variables
* revision 1.1 - 5/6/87  - corrected bug in UVAR_NUMBER (status) call
* V200:   7/7/89 - expression may have been deleted outside this routine
* 	 2/19/90 - major rewrite: loop through ALL user vars deleting "EX#"
* V540:  11/01 - initialize last_cmnd_buff
*	         (formerly local array in get_prot_cmnd_data)
*        2/12 *kms* Add E and F dimensions (use nferdims in tmap_dims.parm)
* V698  2/16 *acm* For ticket 2352: variables defined with LET/D are
*                  added to that dataset in the attribute structure. Extra
*                  argument to DELETE_USER_VAR

        IMPLICIT NONE
        include 'tmap_dims.parm'
	include 'ferret.parm'
	include 'errmsg.parm'
	include 'xvariables.cmn'
	include 'xprog_state.cmn'

* internal variable declarations:
	INTEGER	uvar, j, num_indices, result_array(max_uvar), dset

* eliminate all user variables created by the last expression given
*	DO 100 uvar = 1, max_uvar
*	   IF ( uvar_num_items( uvar ) .EQ. uvar_deleted ) GOTO 100
        CALL deleted_list_get_undel(uvar_num_items_head,
     .                              result_array(1),
     .                              max_uvar, num_indices )
        dset = pdset_uvars
        DO 100 j = 1, num_indices
           uvar = result_array(j)        
	   IF ( uvar_name_code(uvar)(:3) .EQ. 'EX#' )
     .					CALL DELETE_USER_VAR( uvar, dset )
 100	CONTINUE

* remove memory of previous expressions
	last_cmnd_buff = ' '
	len_last_cmnd = 1

	RETURN
	END

