CC=gcc
CXX=g++
CFLAGS=
EXECUTABLES=kaldi-to-svm
INCLUDE=-I include/
CPPFLAGS= -std=c++0x -Wall -fstrict-aliasing $(CFLAGS) $(INCLUDE)
 
.PHONY: debug all o3 kaldi-to-svm
all: $(EXECUTABLES) ctags

o3: CFLAGS+=-O3
o3: all
debug: CFLAGS+=-g -DDEBUG
debug: all

vpath %.h include/
vpath %.cpp src/
vpath %.cu src/

SOURCES=kaldi-io.cpp
OBJ=$(addprefix obj/,$(SOURCES:.cpp=.o))

kaldi-to-svm: kaldi-to-svm.cpp $(OBJ)
	$(CXX) $(CPPFLAGS) $(INCLUDE) -o $@ $^
ctags:
	@ctags -R *

obj/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

obj/%.d: %.cpp
	@$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

-include $(addprefix obj/,$(subst .cpp,.d,$(SOURCES)))

.PHONY:
clean:
	rm -rf $(EXECUTABLES) obj/* tags
