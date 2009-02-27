SRC_DIRS := $(addprefix $(SRC_ROOT)/, $(SRC_DIRS))
SRC_GLOB := $(addsuffix /*.cpp, $(SRC_DIRS))

ifndef OBJS
    OBJS := $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SRC_GLOB))))
endif

ifndef VER
    VER := 110
endif

ifndef DEBUG
    DEBUG := 0
endif

ifneq ($(DEBUG),0)
    DBG := d
    CXXFLAGS_DBG := -D"_DEBUG" -O0 -ggdb3
else
    DBG :=
    CXXFLAGS_DBG := -D"NDEBUG" -O3 -g -fomit-frame-pointer -march=i686 -ffast-math
endif


ifneq ("$(BINTYPE)","DLL")
    OUTBIN := ../../bin/$(TARGET)$(DBG).exe
    LINKFLAGS_DLL := -Wl,-call_shared
else
    OUTBIN := ../../bin/$(TARGET)$(VER)$(DBG).dll
    OUTLIB := ../../lib/lib$(TARGET)$(DBG).a
    LINKFLAGS_DLL := -shared -Wl,-call_shared,--out-implib,$(OUTLIB)
endif

ifndef CXX
    CXX := g++
endif

ifndef LINK
    LINK := $(CXX)
endif

CXXFLAGS := -D"WIN32" -D"_WINDOWS" $(CXXFLAGS_DBG) $(CXXFLAGS) -Wall -pipe -c
LINKFLAGS := -Wl,-L../../lib $(LINKFLAGS_DLL)
LIBS := -lkernel32 -luser32 -lgdi32 -ladvapi32 $(LIBS)

#$(error error is $(OBJS))
VPATH := $(SRC_DIRS)

$(OUTBIN): $(OBJS)
	@-mkdir ..\..\bin
	@-mkdir ..\..\lib
	@$(LINK) $(LINKFLAGS) $^ -o $@ $(LIBS)

%.o: %.cpp
	@echo $@
	@$(CXX) $(CXXFLAGS) $<

%.o: %.c
	@echo $@
	@$(CXX) $(CXXFLAGS) $<

$(OBJS): $(INCS)
