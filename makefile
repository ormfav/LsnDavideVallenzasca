CC = g++
CFLAGS = -Wall -O3 --std=c++11


.PHONY: all clean \
	01 011 012 013 	01clean 011clean 012clean 013clean \
	02 021 022 		02clean 021clean 022clean \
	03 031 			03clean 031clean \

all: 01 02 03 
clean: 01clean 02clean 03clean


#random number generator
RND= lib/Random/
$(RND)obj/random.o: $(RND)random.cpp
	mkdir -p $(PATH)obj
	$(CC) $(CFLAGS) -c $^ -o $@

#data blocking
DB = lib/DataBlocking/
$(DB)obj/datablocking.o: $(DB)datablocking.cpp
	mkdir -p $(DB)obj
	$(CC) $(CFLAGS) -c  $^ -o $@



#Esercitazione 1
01: 011 012 013

011: 01/obj/011.o $(RND)obj/random.o $(DB)obj/datablocking.o
	mkdir -p 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/011.x 
01/obj/011.o: 01/src/011.cpp 01/config/011-ave_var-conf.inl 01/config/011-chi2-conf.inl
	mkdir -p 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

012: 01/obj/012.o $(RND)obj/random.o 
	mkdir -p 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/012.x 
01/obj/012.o: 01/src/012.cpp 01/config/012-conf.inl
	mkdir -p 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

013: 01/obj/013.o $(RND)obj/random.o $(DB)obj/datablocking.o
	mkdir -p 01/bin
	$(CC) $(CFLAGS) $^ -o 01/bin/013.x 
01/obj/013.o: 01/src/013.cpp 01/config/013-conf.inl
	mkdir -p 01/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

#Esercitazione 2
02: 021 022

021: 02/obj/021.o $(RND)obj/random.o $(DB)obj/datablocking.o
	mkdir -p 02/bin
	$(CC) $(CFLAGS) $^ -o 02/bin/021.x 
02/obj/021.o: 02/src/021.cpp
	mkdir -p 02/obj
	$(CC) $(CFLAGS) -c $< -o $@ 

022: 02/obj/022.o $(RND)obj/random.o $(DB)obj/datablocking.o
	mkdir -p 02/bin
	$(CC) $(CFLAGS) $^ -o 02/bin/022.x 
02/obj/022.o: 02/src/022.cpp
	mkdir -p 02/obj
	$(CC) $(CFLAGS) -c $< -o $@

#Esercitazione 3
03: 031 

031: 03/obj/031.o $(RND)obj/random.o $(DB)obj/datablocking.o
	mkdir -p 03/bin
	$(CC) $(CFLAGS) $^ -o 03/bin/031.x 
03/obj/031.o: 03/src/031.cpp
	mkdir -p 03/obj
	$(CC) $(CFLAGS) -c $< -o $@ 


