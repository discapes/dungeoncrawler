CXXFLAGS = -std=gnu++20 -g -Wall -Wextra# -fsanitize=address -Wfatal-errors
LDLIBS =## -lasan

all: a.out ;

objects = $(patsubst %.cpp, obj/%.o, $(wildcard *.cpp))
$(objects): obj $(wildcard *.hpp)
obj: ; mkdir obj
obj/%.o: %.cpp ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
a.out: $(objects) ; $(CXX) $(objects) $(LDFLAGS) $(LDLIBS)

.PHONY: clean ; 
clean: ; -rm -rf obj app
