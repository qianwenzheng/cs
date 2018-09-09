# What are you looking for, here?
csqrt:	sqrt.c
	gcc -Wall -g -DUSE_C -o csqrt sqrt.c

asqrt:	sqrt.c root.s
	gcc -Wall -g -o asqrt sqrt.c root.s

clean:
	@rm -rf *~ *.dSYM
	@echo Clean.

realclean:	clean
	@rm -f asqrt csqrt
	@echo Real clean.

love:
	@echo Not war.

