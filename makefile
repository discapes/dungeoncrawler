CXXFLAGS = -std=c++20 -g -Wall -Wextra #-Wfatal-errors
LDLIBS =

all: a.out ;

cpp_objects = $(patsubst %.cpp, obj/%.o, $(wildcard *.cpp))
c_objects = $(patsubst %.c, obj/%.o, $(wildcard *.c))
$(cpp_objects): obj $(wildcard *.hpp) $(wildcard *.h)
$(c_objects): obj $(wildcard *.h)
obj: ; mkdir obj
obj/%.o: %.cpp ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
obj/%.o: %.c ; $(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
a.out: $(c_objects) $(cpp_objects) ; $(CXX) $(cpp_objects) $(c_objects) $(LDFLAGS) $(LDLIBS)

.PHONY: clean ; 
clean: ; -rm -rf obj a.out
