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



/*	PROGRAM FERRET - C version of MAIN program with reconfigurable memory */

/* compile this with:
   cc -c [-g] -Iferret_cmn fermain_c.c
         (and use -D_NO_PROTO for non-ANSI compilers)
*/

/*
* TMAP interactive data analysis program

* programmer - steve hankin
* NOAA/PMEL, Seattle, WA - Tropical Modeling and Analysis Program
*/

/*
* FERRET program history:
* initially tailored to output format and content of the Philander/Seigel 
* model from GFDL
* revision 0.0  - 4/3/86
* revision 1.0 - 11/17/86 - first "official" release
* revision 2.0 - 10/23/87 - "official" release
* revision 2.01(temporary) - 10/23/87 - smaller memory size, bug fixes,
*			2 typos in XVARIABLES_DATA, ^C added, ZT planes added
* revision 2.02 - ?????
* revision 2.10 - 5/6/88 - "final" release - /NODEBUG version
* FERRET 1.00     - 6/10/88 - rename of GFDL 2.10
* FERRET 1.10     -  8/2/88 - numerous bug fixes and enhancements
* FERRET 1.20     - 2/17/89 - numerous bug fixes and enhancements
* FERRET 1.21     - 4/19/89 - minor bug fixes
* FERRET 2.00	  - 5/??/89 - internal re-write: 4D grids and "object oriented"
*			      transformations
* FERRET 3.00     - 1/29/93 - revision 2.2-->2.3 changes became so extensive
*                             and prolonged it made sense to rename to V3.0
* FERRET 3.10     - 4/94 - official release using XGKS
* FERRET 3.11     - 5/94 - added FILE/ORDER=/FORMAT=STREAM
* FERRET 3.12     - 5/94 - restructured to be "dynamic memory" (C main routine)
*			   former MAIN became FERRET_DISPATCH routine
* FERRET 3.13     - 7/94 - relink of Solaris version
*                          (using IBM-portable TMAP libs)
*|*|*|*|*|*|*|*|*|*|*|*|
*
*/

/*
   revision history for MAIN program unit:
      11/16/94 - changed to a c version of ferret_dispatch_c
* FERRET 4.0     - 7/94 - using a C main program with dynamic memory
*                - 6/95 - *kob* had to add ifdef checks for 
*		  	  NO_ENTRY_NAME_UNDERSCORES for hp port, as hp
*			  doesnt need trailing underscores for c/fortran 
*			  compatibility
*    3/5/97 *sh* changes to incorporate "-batch" qualifier
* Linux Port 5/97 *kob*   Using NAG f90 for linux, we first have to 
*                         call f90_io_init() to set up lun's etc, and then
*                         call f90_io_finish() after we are done 
*                         to flush buffers,etc
*    7/25/97 *js* changes to incorporate output file for -batch
*    8/97 *kob* - had to add another ifdef check for entry_name_underscores
*              around call to curv_coord_sub
*
*    10/16/97 *kob* - Combining non-gui main program w/ gui main program so that
*                     there needs only be one main program.  added an ifdef 
*                     LINK_GUI_AS_MAIN around the gui-exclusive code
*/

 

/*#include "tmap_format/ferret.h"*/
#ifdef LINK_GUI_AS_MAIN
#  include <X11/Intrinsic.h>
#  include <X11/StringDefs.h>
#  include <X11/Xlib.h>
#endif   /* LINK_GUI_AS_MAIN */
#include <stdio.h>
#include <stdlib.h>


#ifdef LINK_GUI_AS_MAIN
/*---------------------------------------------------- 
 * UxXt.h needs to be included only when compiling a 
 * stand-alone application. 
 *---------------------------------------------------*/
#ifndef DESIGN_TIME
#include "UxXt.h"
#endif /* DESIGN_TIME */

XtAppContext	UxAppContext;
Widget		UxTopLevel;
Display		*UxDisplay;

int		UxScreen;
char init_command[128];
extern void SetInitialState(void);

/*----------------------------------------------
 * Insert application global declarations here
 *---------------------------------------------*/
/*
 * BEGIN of "FerretMain_mycode_1.c"
 *
 * Here are the contents of the file "FerretMain_mycode_1.c"
 * which was previously included as a separate file.
 */

#define __globalDefs
#include "ferret_fortran.h"
#include "ferret_structures.h"
#include "ferret_shared_buffer.h"
#include "JC_Utility.h" /* for windows[] and window_count in the SGI_POPUPS section */
#undef __globalDefs
#endif  /* LINK_GUI_AS_MAIN */

#ifndef LINK_GUI_AS_MAIN
#include "ferret.h"
#include "ferret_shared_buffer.h"
#endif  /* LINK_GUI_AS_MAIN */



#ifdef LINK_GUI_AS_MAIN
/* .................... Function Definitions .................... */

void help_text(void);
static void TimeOutCB(XtPointer cd, XtIntervalId *id);

/* .................... Function Declarations .................... */

static void TimeOutCB(XtPointer cd, XtIntervalId *id)
{
  XtAppAddTimeOut((XtAppContext)cd, (unsigned long)2000, TimeOutCB, cd);
  
#ifdef NO_ENTRY_NAME_UNDERSCORES
  xgks_x_events();
#else
  xgks_x_events_();
#endif

}
#endif  /* LINK_GUI_AS_MAIN */

void help_text()
{
#ifdef LINK_GUI_AS_MAIN
  printf(
"Usage:  ferret [-memsize Mwords] [-batch [outfile]] [-gif] [-gui] [-unmapped] [-help] \n\
       -memsize:  specify the memory cache size in megawords (default 3.2)\n\
         -batch:  output directly to metafile \"outfile\" w/out X windows\n\
      -unmapped:  use invisible output windows (superceded by -batch)\n\
           -gif:  output to GIF file w/o X windows only w/ FRAME command\n\
           -gui:  to start Ferret in point and click mode\n\
          -help:  obtain this listing\n");
#else
  printf(
"Usage:  ferret [-memsize Mwords] [-batch [outfile]] [-gif] [-unmapped] [-help] \n\
       -memsize:  specify the memory cache size in megawords (default 3.2)\n\
         -batch:  output directly to metafile \"outfile\" w/out X windows\n\
      -unmapped:  use invisible output windows (superceded by -batch)\n\
           -gif:  output to GIF file w/o X windows only w/ FRAME command\n\
          -help:  obtain this listing\n");
#endif  /* LINK_GUI_AS_MAIN */
  exit(0);
}

#ifdef _NO_PROTO
main (argc, argv)
int argc;
char *argv[];
#else 
main (int argc, char *argv[])
#endif /* NO_PROTO */
{
  int status;
  smPtr sBuffer;
  char init_command[128];
#ifndef LINK_GUI_AS_MAIN
  float *memory; 
#endif
  char *home;
  FILE *fp;

  int i=1,
      max_mem_blks=PMAX_MEM_BLKS,
      mem_blk_size,
      old_mem_blk_size,
      mem_size =  PMEM_BLK_SIZE * PMAX_MEM_BLKS,
      ttout_lun=TTOUT_LUN;
  float rmem_size;
  int using_gui=0;
#ifdef LINK_GUI_AS_MAIN
  char tText[128], quote='"';
  extern void SetInitialState(void);

  /*-----------------------------------------------------------
   * Declarations.
   * The default identifier - mainIface will only be declared 
   * if the interface function is global and of type swidget.
   * To change the identifier to a different name, modify the
   * string mainIface in the file "xtmain.dat". If "mainIface"
   * is declared, it will be used below where the return value
   * of  PJ_INTERFACE_FUNCTION_CALL will be assigned to it.
   *----------------------------------------------------------*/ 

  Widget mainIface;

  /*---------------------------------
   * Interface function declaration
   *--------------------------------*/	

  Widget  create_FerretMainWd(swidget UxParent);
  swidget UxParent = NULL;


  /*------------------------------------------------------
   * Declare the fallback resources (added by JC 9.13.96)
   *-----------------------------------------------------*/

#include "fallback_resources.c"

#endif        /*LINK_GUI_AS_MAIN*/

#ifdef MIXING_NAG_F90_AND_C
  f90_io_init();
#endif


#ifdef LINK_GUI_AS_MAIN
/*
* Make sure the FERRET environment is set up and ready by looking for $FER_DIR.
*/
   if (getenv("FER_DIR") == NULL){
     printf("\n  The FERRET environment has not been properly set up.\n");
     printf("  (The environment variable FER_DIR is not defined)\n");
     printf("\n  Have you executed \"source your_system_path/ferret_paths\" ?\n");
     exit(1);
   }
#endif   /* LINK_GUI_AS_MAIN */



/* decode the command line options: "-memsize", and "-unmapped" */
  while (i<argc) {
    if (strcmp(argv[i],"-memsize")==0){
      if (++i==argc) help_text();
      if ( sscanf(argv[i++],"%f",&rmem_size) != 1 ) help_text();
      if ( rmem_size <= 0.0 ) help_text();
      mem_size = rmem_size * 1.E6;
    } else if (strcmp(argv[i],"-unmapped")==0) {
      WindowMapping(0);  /* new routine added to xopws.c */
      i++;    /* advance to next argument */
    } else if (strcmp(argv[i],"-gif")==0) {
      char *meta_name = ".gif";	/* Unused dummy name */
      set_batch_graphics_(meta_name);
      ++i;
    } else if (strcmp(argv[i],"-batch")==0) {
      char *meta_name = "metafile.plt";
      if (++i < argc && argv[i][0] != '-'){
	meta_name = argv[i++];
      }
#ifdef NO_ENTRY_NAME_UNDERSCORES
      set_batch_graphics(meta_name);  /* inhibit X output altogether */
#else
      set_batch_graphics_(meta_name);  /* inhibit X output altogether */
#endif
#ifdef LINK_GUI_AS_MAIN
    } else if (strcmp(argv[i],"-gui")==0) {
      using_gui = 1;  /* new routine added to xopws.c */
      i++;
#endif  /* LINK_GUI_AS_MAIN */
    } else  /* -help also comes here */
      help_text();
  }

/* initial allocation of memory space */
  mem_blk_size =  mem_size / max_mem_blks;
  memory = (float *) malloc(mem_size*sizeof(float));
  if ( memory == 0 ) {
    printf("Unable to allocate the requested %f Mwords of memory.\n",mem_size/1.E6);
    exit(0);

  }
#ifdef NO_ENTRY_NAME_UNDERSCORES     /* added 6/95 -kob- */
/* initialize stuff: keyboard, todays date, grids, GFDL terms, PPL brain */
  initialize();

/* turn on ^C interrupts  */
/* 10/97 *kob* add check for gui now that there is only one main program */
  if (!using_gui) {
  set_ctrl_c( ctrlc_ast );
}
/*  prepare appropriate console input state and open the output journal file */
  init_journal( &status );

/* program name and revision number */
/* 10/97 *kob* add check for gui now that there is only one main program */
  if (!using_gui ) {
  proclaim_c( &ttout_lun, "\t" );
}
/* initialize size and shape of memory and linked lists */
  init_memory( &mem_blk_size, &max_mem_blks );
#else
/* initialize stuff: keyboard, todays date, grids, GFDL terms, PPL brain */
  initialize_();

/* turn on ^C interrupts */
/* 10/97 *kob* add check for gui not that there is only one main program */
  if (!using_gui ) {
    set_ctrl_c_( ctrlc_ast_ );
}
/*  prepare appropriate console input state and open the output journal file */
  init_journal_( &status );

/* program name and revision number */
/* 10/97 *kob* add check for gui not that there is only one main program */
  if (!using_gui ) {
    proclaim_c_( &ttout_lun, "\t" );
}
/* initialize size and shape of memory and linked lists */
  init_memory_( &mem_blk_size, &max_mem_blks );
#endif            /* ENTRY_NAME_UNDERSCORES */

#ifdef LINK_GUI_AS_MAIN
  /*
   * ==>> place FERRET under GUI control here  <<== 
   */
  if ( using_gui ) {
#ifdef NO_ENTRY_NAME_UNDERSCORES
    mode_gui_on();
#else
    mode_gui_on_();
#endif
  }
#endif /* LINK_GUI_AS_MAIN */

/* set up to execute $HOME/.ferret if it exists: '\GO "$HOME/.ferret"' */
/* --> need to see if it exists!!! */
  home = getenv("HOME");
  if (home != NULL ) {
    strcpy( init_command, home );
    strcat( init_command, "/.ferret" );
    fp = fopen( init_command, "r" );
    if ( fp == NULL )
      strcpy( init_command, " " );
    else
      strcpy( init_command, "GO \"$HOME/.ferret\"" );
    fclose( fp );
  } else
    strcpy( init_command, " " );

  /* GUI OFF -- Only using the command line interface. */

  if ( !using_gui ) {

/* allocate the shared buffer */
  sBuffer = (sharedMem *)malloc(sizeof(sharedMem));

/* run the initialization file
* Note 1: do not pass a fixed string as the command - FERRET needs to blank it
* Note 2: if not under GUI control we will not normally exit FERRET_DISPATCH
*	  until we are ready to exit FERRET */
  while ( 1) {
/*    ferret_dispatch_( memory, init_command, rtn_buff );  FORTRAN version */
    ferret_dispatch_c( memory, init_command, sBuffer );

/* debugging flow control checks */
/*
    printf("       --> control returned to C MAIN:%d %d %d\n",
           sBuffer->flags[FRTN_CONTROL],
	   sBuffer->flags[FRTN_STATUS],
	   sBuffer->flags[FRTN_ACTION]);
*/

/* ***** REALLOCATE MEMORY ***** */
    if (sBuffer->flags[FRTN_ACTION] == FACTN_MEM_RECONFIGURE) {
      old_mem_blk_size = mem_blk_size;
      mem_size = sBuffer->flags[FRTN_IDATA1];
      mem_blk_size = mem_size / max_mem_blks;
/*
      printf("memory reconfiguration requested: %d\n",mem_size);
      printf("new mem_blk_size = %d\n",mem_blk_size);
*/
      free ( (void *) memory );
      memory = (float *) malloc(mem_size*sizeof(float));
      if ( memory == 0 ) {
	printf("Unable to allocate %d Mwords of memory.\n",mem_size/1.E6 );
	mem_blk_size = old_mem_blk_size;
	mem_size = mem_blk_size * max_mem_blks;
	memory = (float *) malloc(mem_size*sizeof(float));
	if ( memory == 0 ) {
	  printf("Unable to reallocate previous memory of %d Mwords.\n",mem_size/1.E6 );
	  exit(0);
	} else {
	  printf("Restoring previous memory of %f Mwords.\n",mem_size/1.E6 );
	}
      }
#ifdef NO_ENTRY_NAME_UNDERSCORES    /* added 6/95 -kob- */
      init_memory( &mem_blk_size, &max_mem_blks );
#else
      init_memory_( &mem_blk_size, &max_mem_blks );
#endif
    }

/* ***** EXIT ***** */
    else if  (sBuffer->flags[FRTN_ACTION] == FACTN_EXIT ) {
/*      printf("exit from FERRET requested\n"); */
      exit(0);
    }

/* ***** TEMPORARY RETURN IN CASE MAIN NEEDS TO DISPLAY FERRET MSG ***** */
    else {

/*
   check the sBuffer->flags[FRTN_STATUS] to see if you need
   to display any messages from FERRET to the user
*/
      if (sBuffer->flags[FRTN_STATUS] != 3 ) {
	printf("error buffer from FERRET: %d\n", sBuffer->numStrings);
	printf("error lines:\n%s\n",sBuffer->text);
      }
      else
	printf("no action - returning from GUI to FERRET\n");

    }
  }
/******* GUI SECTION *******/
/* 10/97 *kob* added gui section now that there is only one main program */
#ifdef LINK_GUI_AS_MAIN
} else {



  /*    #include "FerretMain_mycode_3.c"*/
/*
 * END of "FerretMain_mycode_3.c"
 */

#ifdef XOPEN_CATALOG
    if (XSupportsLocale()) {
      XtSetLanguageProc(NULL,(XtLanguageProc)NULL,NULL);
    }
#endif


    /*UxTopLevel = XtAppInitialize(&UxAppContext, "Ferret",
 				     NULL, 0, &argc, argv, NULL, NULL, 0);*/

    UxTopLevel = XtVaAppInitialize(&UxAppContext, "Ferret",
				   NULL, 0, &argc, argv, fallbacks, NULL);

    UxDisplay = XtDisplay(UxTopLevel);
    UxScreen = XDefaultScreen(UxDisplay);

    /*
     * We set the geometry of UxTopLevel so that dialogShells
     * that are parented on it will get centered on the screen
     * (if defaultPosition is true).
     */

    XtVaSetValues(UxTopLevel,
		  XtNx, 0,
		  XtNy, 0,
		  XtNwidth, DisplayWidth(UxDisplay, UxScreen),
		  XtNheight, DisplayHeight(UxDisplay, UxScreen),
		  NULL);

    /*-------------------------------------------------------
     * Insert initialization code for your application here 
     *------------------------------------------------------*/
	
    /*----------------------------------------------------------------
     * Create and popup the first window of the interface.  The 	 
     * return value can be used in the popdown or destroy functions.
     * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
     * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
     *---------------------------------------------------------------*/

    mainIface = create_FerretMainWd(UxParent);

    SetInitialState();
    UxPopupInterface(mainIface, no_grab);

#ifdef X_REFRESH
    XtAppAddTimeOut(UxAppContext, 2000, TimeOutCB, UxAppContext);
#endif

#ifdef SGI_POPUPS
    /* NB__ UxTopLevel is declared external in UxXt.h */
    windows[window_count++] = XtWindow(UxTopLevel);
    XSetWMColormapWindows(XtDisplay(UxTopLevel), XtWindow(UxTopLevel),
			  windows, window_count);
#endif /* SGI_POPUPS */

    /*-----------------------
     * Enter the event loop 
     *----------------------*/
    {

      XEvent event;

      for (;;)
	{
	  XtAppNextEvent(UxAppContext, &event);

	  if (event.type == ClientMessage) {
	    ;
	  }
	  switch (event.type)
	    {

	      /*---------------------------------------------------
	       * Insert code here to handle any events that you do
	       * not wish to be handled by the interface.
	       *---------------------------------------------------*/
	    default:
	      XtDispatchEvent(&event);
	      break;
	    }
	}

    } /* event loop block */

  } /* END of GUI ON section. */
#else

}   /* close paren for no_gui section */
#endif /*LINK_GUI_AS_MAIN*/
/* 
 *kob* 5/97 - need to close f90 files and flush buffers.....
 */
#ifdef MIXING_NAG_F90_AND_C
  f90_io_finish();
#endif
}

