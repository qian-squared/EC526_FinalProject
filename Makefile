.SUFFIXES:
.SUFFIXES: .o .cpp
#============================================================

TARGET1 = q_learning

C_SOURCES =  
MY_INCLUDES1 = q_learning.cpp

ALL_SOURCES =

CCX = mpic++
CXXFLAGS = -std=c++11


#============================================================
all: $(TARGET1) $(TARGET2)  $(TARGET3) $(TARGET4)

$(TARGET1) :   $(MY_INCLUDES1)
	$(CCX) $(CXXFLAGS)  $^ $(LIBDIRS)  -o $@
# Implicit rules: $@ = target name, $< = first prerequisite name, $^ = name of all prerequisites
#============================================================


clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(C_OBJS1) $(C_OBJS2) $(C_OBJS3) $(C_OBJS4)  

