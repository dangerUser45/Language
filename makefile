Compiler = g++
B        = build_language/
Target   = Language
#-----------------------------------------------------------------------

$(Target):      $(B)Language_Main.o $(B)Language_Init.o $(B)Lexical_analyzer.o $(B)Print.o $(B)Processing.o $(B)TxLib.o
	$(Compiler) $(B)Language_Main.o $(B)Language_Init.o $(B)Lexical_analyzer.o $(B)Print.o $(B)Processing.o $(B)TxLib.o -o Lang.exe
#-----------------------------------------------------------------------

$(B)Language_Main.o : Language_Main.cpp                           \
				LanguageCommon.h                                  \
				Language_init.h
	$(Compiler) -c Language_Main.cpp -o $(B)Language_Main.o

$(B)Language_Init.o : Language_Init.cpp                           \
				..\Processor\Onegin_for_proc\Onegin_processing.h  \
				..\Processor\Onegin_for_proc\Onegin_General.h     \
				..\Processor\Onegin_for_proc\Print.h              \
				Language_Init.h                                   
	$(Compiler) -c Language_Init.cpp -o $(B)Language_Init.o

$(B)Lexical_analyzer.o :Lexical_analyzer.cpp                      \
				LanguageCommon.h                                  \
				Lexical_analyzer.h
	$(Compiler) -c Lexical_analyzer.cpp -o $(B)Lexical_analyzer.o

$(B)Print.o : ../Processor/Onegin_for_proc/Print.cpp              \
		      ../Processor/Onegin_for_proc/Onegin_General.h       \
			  ../Processor/Onegin_for_proc/Print.h
	$(Compiler) -c  ../Processor/Onegin_for_proc/print.cpp -o $(B)Print.o

$(B)Processing.o : ../Processor/Onegin_for_proc/Processing.cpp      \
				   ../Processor/Onegin_for_proc/Onegin_General.h    \
				   ../Processor/Onegin_for_proc/Onegin_Processing.h \
				   ../Processor/Onegin_for_proc/Print.h
	$(Compiler) -c ../Processor/Onegin_for_proc/Processing.cpp -o $(B)Processing.o

$(B)TxLib.o :
	$(Compiler) -c TxLib.cpp -o $(B)TxLib.o
#---------------------------------------------------------------------------
clean :
	del build_language\*.o
