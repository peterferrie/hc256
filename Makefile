msvc:
		cl /nologo /O2 /Ot /DTEST test.c hc256.c
gnu:
		gcc -DTEST -Wall -O2 test.c hc256.c -otest	 
clang:
		clang -DTEST -Wall -O2 test.c hc256.c -otest	    