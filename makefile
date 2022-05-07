CC = g++
CFLAGS = -Wall -O3 --std=c++11

.PHONY: all clean \
	01 011 012 013 	01clean 011clean 012clean 013clean \
	02 021 022 		02clean 021clean 022clean \
	03 031 			03clean 031clean \
	04 041		 	04clean 041clean \
	05 051 			05clean 051clean 

all: 01 02 03 04 05
clean: 01clean 02clean 03clean 04clean 05clean


#random number generator
RND= lib/Random/
$(RND)obj/random.o: $(RND)random.cpp | $(RND)obj
	$(CC) $(CFLAGS) -c $^ -o $@
$(RDN)obj:
	ls $@

#data blocking
DBH = lib/DataBlocking/datablocking.h
#$(DB)obj/datablocking.o: $(DB)datablocking.cpp | $(DB)obj
#	$(CC) $(CFLAGS) -c  $^ -o $@
#$(DB)obj:
#	ls $@

#point come lo piazzo dentro?
PTH = lib/Point/point.h 
PTS = lib/Point/point.inl


#Esercitazione 1
01: 011 012 013

011: 01/obj/011.o $(RND)obj/random.o $(DB) $(PTH) | 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/011.x 
01/obj/011.o: 01/src/011.cpp 01/in/011-ave_var-conf.inl 01/in/011-chi2-conf.inl | 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

012: 01/obj/012.o $(RND)obj/random.o  | 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/012.x 
01/obj/012.o: 01/src/012.cpp 01/in/012-conf.inl | 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

013: 01/obj/013.o $(RND)obj/random.o  $(DB) $(PTH) | 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/013.x 
01/obj/013.o: 01/src/013.cpp 01/in/013-conf.inl  | 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

01/bin:
	ls $@
01/obj:
	ls $@


#Esercitazione 2
02: 021 022

021: 02/obj/021.o $(RND)obj/random.o $(DB)obj/datablocking.o | 02/bin
	$(CC) $(CFLAGS) $^ -o 02/bin/021.x 
02/obj/021.o: 02/src/021.cpp | 02/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

022: 02/obj/022.o $(RND)obj/random.o $(DB)obj/datablocking.o | 02/bin
	$(CC) $(CFLAGS) $^ -o 02/bin/022.x 
02/obj/022.o: 02/src/022.cpp | 02/obj
	$(CC) $(CFLAGS) -c $< -o $@

02/bin:
	ls $@
02/obj:
	ls $@


#Esercitazione 3
03: 031 

031: 03/obj/031.o $(RND)obj/random.o $(DB)obj/datablocking.o | 03/bin
	$(CC) $(CFLAGS) $^ -o 03/bin/031.x 
03/obj/031.o: 03/src/031.cpp | 03/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

03/bin:
	ls $@
03/obj:
	ls $@


#Esercitazione 4
04: 041 

041: 04/obj/041.o $(RND)obj/random.o | 04/bin
	$(CC) $(CFLAGS) $^ -o 04/bin/041.x 
04/obj/041.o: 04/src/041.cpp | 04/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

04/bin:
	ls $@
04/obj:
	ls $@


#Esercitazione 5
05: 051 

051: 05/obj/051.o $(RND)obj/random.o $(DB)obj/datablocking.o | 05/bin
	$(CC) $(CFLAGS) $^ -o 05/bin/051.x 
05/obj/051.o: 05/src/051.cpp | 05/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

05/bin:
	ls $@
05/obj:
	ls $@
