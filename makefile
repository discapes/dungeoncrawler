CXXFLAGS = -std=c++20 -g -Wall -Wextra #-Wfatal-errors
LDLIBS =
NAME = Skall.exe

cpp_objects = $(patsubst %.cpp, obj/%.o, $(wildcard *.cpp))
c_objects = $(patsubst %.c, obj/%.o, $(wildcard *.c))

$(NAME): $(c_objects) $(cpp_objects) ; $(CXX) $(cpp_objects) $(c_objects) $(LDFLAGS) $(LDLIBS) -o $(NAME)

$(cpp_objects): obj $(wildcard *.hpp) $(wildcard *.h)
$(c_objects): obj $(wildcard *.h)
obj: ; mkdir obj
obj/%.o: %.cpp ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
obj/%.o: %.c ; $(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean ; 
clean: ; -rm -rf obj $(NAME)