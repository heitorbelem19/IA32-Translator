INC_DIR =../include
CC = g++ -std=c++11
CFLAGS = -g -I $(INC_DIR)

ODIR = obj
LDIR =../lib

LIBS = -lm

_DEPS = translator.hpp io_manager.hpp pre_processor.hpp
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = tradutor.o translator.o io_manager.o pre_processor.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tradutor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 