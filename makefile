CXXFLAGS = -std=c++20 -g -Wall -Wextra #-Wfatal-errors
LDLIBS =

all: a.out ;

objects = $(patsubst %.cpp, obj/%.o, $(wildcard *.cpp))
$(objects): obj $(wildcard *.hpp)
obj: ; mkdir obj
obj/%.o: %.cpp ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
a.out: $(objects) ; $(CXX) $(objects) $(LDFLAGS) $(LDLIBS)

.PHONY: clean ; 
clean: ; -rm -rf obj app
