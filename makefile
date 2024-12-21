Compiler = g++
B        = build_language/
Target   = Language
Flags    = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

$(Target):      $(B)Main_Language.o $(B)Init_Language.o $(B)Dump_Language.o $(B)Parser_Language.o $(B)Lexical_analyzer.o $(B)Print.o  $(B)Processing.o #$(B)TxLib.o
	$(Compiler) $(B)Main_Language.o $(B)Init_Language.o $(B)Dump_Language.o $(B)Parser_Language.o $(B)Lexical_analyzer.o $(B)Print.o  $(B)Processing.o -o Lang.exe $(Flags)
	@echo [36m====================================================================================================================================================================[0m
#-----------------------------------------------------------------------

$(B)Main_Language.o : Main_Language.cpp                           \
				Common_Language.h                                 \
				Init_Language.h
	$(Compiler) -c Main_Language.cpp -o $(B)Main_Language.o $(Flags)

$(B)Init_Language.o : Init_Language.cpp                           \
				..\Processor\Onegin_for_proc\Onegin_processing.h  \
				..\Processor\Onegin_for_proc\Onegin_General.h     \
				..\Processor\Onegin_for_proc\Print.h              \
				Init_Language.h
	$(Compiler) -c Init_Language.cpp -o $(B)Init_Language.o $(Flags)

$(B)Parser_Language.o : Parser_Language.cpp                       \
						Parser_Language.h                         \
						Common_Language.h                         \
						SyntaxError.h							  \
						..\SuperLibs\COLOR.h     
	$(Compiler) -c Parser_Language.cpp -o $(B)Parser_Language.o $(Flags)

$(B)Dump_Language.o : Dump_Language.cpp                           \
				Common_Language.h                                 \
				Init_Language.h
	$(Compiler) -c Dump_Language.cpp -o $(B)Dump_Language.o $(Flags)

$(B)Lexical_analyzer.o : Lexical_analyzer.cpp                     \
				Common_Language.h                                 \
				Lexical_analyzer.h                                \
				SyntaxError.h
	$(Compiler) -c Lexical_analyzer.cpp -o $(B)Lexical_analyzer.o $(Flags)

$(B)Print.o : ../Processor/Onegin_for_proc/Print.cpp              \
		      ../Processor/Onegin_for_proc/Onegin_General.h       \
			  ../Processor/Onegin_for_proc/Print.h
	$(Compiler) -c  ../Processor/Onegin_for_proc/print.cpp -o $(B)Print.o

$(B)Processing.o : ../Processor/Onegin_for_proc/Processing.cpp      \
				   ../Processor/Onegin_for_proc/Onegin_General.h    \
				   ../Processor/Onegin_for_proc/Onegin_Processing.h \
				   ../Processor/Onegin_for_proc/Print.h
	$(Compiler) -c ../Processor/Onegin_for_proc/Processing.cpp -o $(B)Processing.o

#$(B)TxLib.o :
#	$(Compiler) -c TxLib.cpp -o $(B)TxLib.o
#---------------------------------------------------------------------------
clean :
	del build_language\*.o
