CXXFLAGS = -std=c++20 -Wfatal-errors -g -Wall -Wextra

all: a.out ;

objects = $(patsubst %.cpp, obj/%.o, $(wildcard *.cpp))
$(objects): obj $(wildcard *.hpp)
obj: ; mkdir obj
obj/%.o: %.cpp ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
a.out: $(objects) ; $(CXX) $(objects) $(LDFLAGS) $(LDLIBS)

.PHONY: clean ; 
clean: ; -rm -rf obj app
