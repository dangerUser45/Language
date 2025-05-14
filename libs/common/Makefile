SRC		         = src/
B		         = build/

AR               = ar
ARFLAGS          = rcs

NAME_LIB         = Common
LIB              = $(B)lib$(NAME_LIB).a

CXX              = g++
CXXFLAGS         = -I./include

CXXFLAGS_DEBUG	 = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector
CXXFLAGS_RELEASE = -mavx -mavx2 -O3 -DNDEBUG

CPPSRC 	         = $(SRC)Common.cpp
CPPOBJ 	         = $(CPPSRC:$(SRC)%.cpp=$(B)%.o)
CPPDEP           = $(patsubst %.o, %.d, $(CPPOBJ))
#==================================================================================
#----------------------------------------------------------------------------------
#==================================================================================

BUILD ?= release

ifeq ($(BUILD), debug)
	CXXFLAGS += -g  $(CXXFLAGS_DEBUG)

else ifeq ($(BUILD), release)
	CXXFLAGS +=  $(CXXFLAGS_RELEASE)

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

.PHONY: lib clear
#----------------------------------------------------------------------------------

lib: $(LIB)

$(LIB): $(CPPOBJ)
	$(AR) $(ARFLAGS) $@ $(CPPOBJ)

$(B)%.o: $(SRC)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MT '$@ $(@:.o=.d)' -c $< -o $@

-include $(CPPDEP)

#----------------------------------------------------------------------------------

clear:
	rm -rf $(B)*
#----------------------------------------------------------------------------------
