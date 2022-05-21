CC = g++
CFLAGS = -Wall -O3 --std=c++11

.PHONY: all clean \
	01 011 012 013 	01clean 011clean 012clean 013clean \
	02 021 022 		  02clean 021clean 022clean \
	03 031 			    03clean 031clean \
	04 041		 	    04clean 041clean \
	05 051 			    05clean 051clean \
	06 061 			    06clean 061clean \
	08 081 082		  08clean 081clean 082clean 

all: 01 02 03 04 05 08
clean: 01clean 02clean 03clean 04clean 05clean 08clean


#random number generator
RND= lib/Random/
$(RND)obj/random.o: $(RND)random.cpp | $(RND)obj
	$(CC) $(CFLAGS) -c $^ -o $@
$(RDN)obj:
	mkdir -p $@

#data blocking
DBH = lib/DataBlocking/datablocking.h

#point class
PTH = lib/Point/point.h 

#metropolis funcions
MEH = lib/Metropolis/metropolis.h

MIS = lib/Misc/
$(MIS)obj/misc.o: $(MIS)misc.cpp | $(MIS)obj
	$(CC) $(CFLAGS) -c $^ -o $@
$(MIS)obj:
	mkdir -p $@

#Esercitazione 1
01: 011 012 013
01clean: 011clean 012clean 013clean

011: 01/obj/011.o $(RND)obj/random.o $(MIS)obj/misc.o $(DBH) $(PTH) | 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/011.x 
01/obj/011.o: 01/src/011.cpp 01/in/011-ave_var-conf.inl 01/in/011-chi2-conf.inl | 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

012: 01/obj/012.o $(RND)obj/random.o $(MIS)obj/misc.o | 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/012.x 
01/obj/012.o: 01/src/012.cpp 01/in/012-conf.inl | 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

013: 01/obj/013.o $(RND)obj/random.o $(MIS)obj/misc.o $(DBH) $(PTH) | 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/013.x 
01/obj/013.o: 01/src/013.cpp 01/in/013-conf-experiment.inl 01/in/013-conf-datablocking.inl   | 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

01/bin:
	ls $@
01/obj:
	ls $@

011clean: 
	rm -f 01/out/011*
	rm -f 01/obj/011*
	rm -f 01/bin/011*

012clean: 
	rm -f 01/out/012*
	rm -f 01/obj/012*
	rm -f 01/bin/012*

013clean: 
	rm -f 01/out/013*
	rm -f 01/obj/013*
	rm -f 01/bin/013*

#Esercitazione 2
02: 021 022
02clean: 021clean 022clean 

021: 02/obj/021.o $(RND)obj/random.o $(DBH) $(PTH) | 02/bin
	$(CC) $(CFLAGS) $^ -o 02/bin/021.x 
02/obj/021.o: 02/src/021.cpp | 02/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

022: 02/obj/022.o $(RND)obj/random.o $(MIS)obj/misc.o $(DBH) $(PTH)  | 02/bin
	$(CC) $(CFLAGS) $^ -o 02/bin/022.x 
02/obj/022.o: 02/src/022.cpp | 02/obj
	$(CC) $(CFLAGS) -c $< -o $@

02/bin:
	ls $@
02/obj:
	ls $@

021clean: 
	rm -f 02/out/021*
	rm -f 02/obj/021*
	rm -f 02/bin/021*

022clean: 
	rm -f 02/out/022*
	rm -f 02/obj/022*
	rm -f 02/bin/022*

#Esercitazione 3
03: 031 
03clean: 031clean 

031: 03/obj/031.o $(RND)obj/random.o $(DBH) $(PTH) | 03/bin
	$(CC) $(CFLAGS) $^ -o 03/bin/031.x 
03/obj/031.o: 03/src/031.cpp | 03/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

03/bin:
	ls $@
03/obj:
	ls $@

031clean: 
	rm -f 02/out/031*
	rm -f 02/obj/031*
	rm -f 02/bin/031*

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

051: 05/obj/051.o $(RND)obj/random.o $(MIS)obj/misc.o $(DBH) $(PTH) $(MEH) | 05/bin
	$(CC) $(CFLAGS) $^ -o 05/bin/051.x 
05/obj/051.o: 05/src/051.cpp | 05/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

05/bin:
	ls $@
05/obj:
	ls $@


#Esercitazione 6
06: 061 

061: 06/obj/061.o $(RND)obj/random.o | 06/bin
	$(CC) $(CFLAGS) $^ -o 06/bin/061.x 
06/obj/061.o: 06/src/061.cpp | 06/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

06/bin:
	ls $@
06/obj:
	ls $@


#Esercitazione 8
08: 081 082

081: 08/obj/081.o $(RND)obj/random.o $(MIS)obj/misc.o $(DBH) $(PTH) $(MEH) | 08/bin
	$(CC) $(CFLAGS) $^ -o 08/bin/081.x 
08/obj/081.o: 08/src/081.cpp | 08/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

08/bin:
	ls $@
08/obj:
	ls $@
