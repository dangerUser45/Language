TARGET                      = Language

SRC                			= src/
B                  			= build/
D                  			= dump/
G                           = Graphviz_dot/
IMAGES                      = images/
LIB_PATH                  	= libs/

LIB_COMMON_NAME             = Common
LIB_COMMON_PATH             = $(LIB_PATH)common/
LIB_COMMON                  = $(LIB_COMMON_PATH)$(B)libCommon.a

CXX                			= g++

AR                          = ar
ARFLAGS                     = rcs

CXXFLAGS_DEBUG     			= -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector
CXXFLAGS_RELEASE   			= -O3 -DNDEBUG
CXXFLAGS           			= -I./include/ -I./libs/common/include/

LDFLAGS                     = -L$(LIB_COMMON_PATH)$(B) -l$(LIB_COMMON_NAME)

EXEC_FILE          		    = $(B)$(TARGET).out

CPPSRC             			= $(SRC)LanguageCtorDtor.cpp $(SRC)LanguageFrontendMain.cpp $(SRC)LanguageFrontendFunc.cpp $(SRC)LanguageFrontendDump.cpp
CPPOBJ             			= $(CPPSRC:$(SRC)%.cpp=$(B)%.o)
CPPDEP             			= $(patsubst %.o, %.d, $(CPPOBJ))
#==================================================================================
#----------------------------------------------------------------------------------
#==================================================================================

BUILD ?= release

ifeq ($(BUILD), debug)
	CXXFLAGS += -g $(CXXFLAGS_DEBUG)

else ifeq ($(BUILD), release)
	CXXFLAGS += $(CXXFLAGS_RELEASE)

else
$(error Unknown build type: $(BUILD). Use debug or release)
endif
#----------------------------------------------------------------------------------

ASSERT ?= off

ifeq ($(ASSERT), on)
	CXXFLAGS += -DASSERT_ON

else ifeq ($(ASSERT), off)

else
$(error Unknown assert mode: $(ASSERT). Use on or off)
endif
#----------------------------------------------------------------------------------

CODE_FILE ?= source_data/code.txt
#==================================================================================
#----------------------------------------------------------------------------------
#==================================================================================

.PHONY: all lib_common run memcheck clear clearall
#----------------------------------------------------------------------------------

all: $(EXEC_FILE)

lib_common: $(LIB_COMMON)

$(LIB_COMMON):
	$(MAKE) -C $(LIB_COMMON_PATH) lib

$(EXEC_FILE): $(LIB_COMMON) $(CPPOBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

$(B)%.o: $(SRC)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MT '$@ $(@:.o=.d)' -c $< -o $@
-include $(CPPDEP)
#----------------------------------------------------------------------------------

run: $(EXEC_FILE)
	$(EXEC_FILE) $(CODE_FILE)
#----------------------------------------------------------------------------------

memcheck: $(EXEC_FILE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(EXEC_FILE) $(CODE_FILE)
#----------------------------------------------------------------------------------

clear:
	rm -rf $(B)* $(D)*.html  $(G)*.txt $(G)$(IMAGES)
#----------------------------------------------------------------------------------

clearall:
	rm -rf $(B)* $(D)*.html  $(D)$(G)*.txt $(D)$(G)$(IMAGES)*
	$(MAKE) -C $(LIB_COMMON_PATH) clear
#----------------------------------------------------------------------------------
