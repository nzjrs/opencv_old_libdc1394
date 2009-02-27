!ifndef MY_CXX
MY_CXX = cl
!endif

!ifndef MY_CXXFLAGS
MY_CXXFLAGS = ""
!endif

!ifndef MY_LINK
MY_LINK = link
!endif

!ifndef MY_LINKFLAGS
MY_LINKFLAGS = ""
!endif

!ifndef PLATFORM
PLATFORM = I386
!endif

!ifndef PDBNAME
PDBNAME = vc60.pdb
!endif

!ifndef DEBUG
DEBUG = 0
!endif

!include make_all_ms.mak
