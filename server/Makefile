TARGET = server
#Includes
HIREDIS = -I/usr/local/include/hiredis
REDIS = /usr/local/lib/libhiredis.so.0

#Libs
LIBS =  $(REDIS) -levent

CFLAGS = -Wall -c  $(HIREDIS)
HEADERS = chat.h clientRedis.h
OBJECTS = server.o chat.o clientRedis.o
GCC = gcc

%.o:	%.c $(HEADERS)
	$(GCC) $(CFLAGS) $<
				

all:	$(TARGET)

server: $(OBJECTS)
	$(GCC) $(OBJECTS) $(LIBS) -o $(TARGET)

clean:
		rm -rf *.o server 

