CC 			= gcc
CFLAGS 		= -Wall -Wextra
LDFLAGS 	= -lX11
OBJFILES 	= SKText.o
TARGET = SKText

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
	rm -f $(OBJFILES) *~

clean:
	rm -f $(TARGET) *~
