CC      := g++
CFLAGS  := -std=c++14 -Wall -g
LFLAGS  := -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lpthread
SRCDIR  := src
OBJDIR  := obj
DOCDIR  := doc
APPNAME := app
SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))

$(shell mkdir -p $(dir $(OBJECTS)))

$(APPNAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $(APPNAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: ${APPNAME}
	./$(APPNAME)

debug: ${APPNAME}
	gdbserver :9091 $(APPNAME)

doc:
	mkdir -p ${DOCDIR}
	doxygen Doxyfile

clean:
	rm -rf $(OBJDIR)
	rm -rf ${DOCDIR}
	rm -f ${APPNAME}
