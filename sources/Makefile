
TARGET=snake
CSRC=main.c snake.c screen.c
CSRC+=

CC=gcc
OPT=2
DEBUG=-ggdb3
CFLAGS=-Wall $(DEBUG) -O$(OPT)
LDFLAGS=-lm -lGL -lGLU -lglut

COBJ=$(CSRC:.c=.o)

all: $(TARGET)

$(TARGET): $(COBJ)
	@echo "linking ...."
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(COBJ): %.o: %.c
	@echo "compiling - $< ...."
	$(CC) -c $< $(CFLAGS)

clean:
	@echo "deleting generated files ...."
	rm -f $(TARGET) $(COBJ)

.PHONY: clean all
