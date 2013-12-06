SOURCES = $(wildcard *.cpp) $(wildcard ../modules/*.cpp) ../add2systemC/my_fxtype_params.cpp 
OBJS := $(SOURCES:.cpp=.o)
# OBJS = main.o ../modules/snk.o  ../modules/src.o 
G++ = g++
SYSTEMC_DIR = /cygdrive/c/systemc-2.3.0/systemc-2.3.0
I_DIR = -I./ -I $(SYSTEMC_DIR)/src -I ../modules -I ../add2systemC
L_DIR = -L $(SYSTEMC_DIR)/lib-cygwin
LIB = -lsystemc
project: $(OBJS)
	$(G++) $(I_DIR) $(L_DIR) -o $@ $(OBJS) $(LIB)
.cpp.o:
	$(G++) $(I_DIR) -c -o $@ $<
clean:
	rm -f project
	rm -f *.o *.pgm 
  # ../add2systemC/*.o ../modules/*.o