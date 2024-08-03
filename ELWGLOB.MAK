###############################################################################
#####               This makefile generates ELWGLOB.DLL                   #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

VIRDIR = .\GCVIRDAT

DLLLIB =            \
     $(DLLLIB)      \
     wgserver.lib   \
     galme.lib      \
     galgsbl.lib

VIRFILES =          \
     GCVIRDAT\elwglobc.vir   \
     GCVIRDAT\elwglobg.vir   \
     GCVIRDAT\elwglobn.vir   \
     GCVIRDAT\elwglobs.vir   \
     GCVIRDAT\elwglobu.vir
     
OBJFILES =          \
     mjrtime.obj    \
     daterous.obj   \
     globrous.obj   \
     elwglob.obj     

elwglob.dll:       \
     $(GCBUILD)     \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
