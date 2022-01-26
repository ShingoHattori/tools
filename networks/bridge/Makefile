OBJS=main.o netutil.o
SRCS=$(OBJS:%.o=%.c)
CFLAGS=-g -Wall
LDLIBS=
TARGET=bridge

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $(OBJS) $(LDLIBS)

clean:
	$(OBJS) $(TARGET)
