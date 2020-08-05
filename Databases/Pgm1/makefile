CFLAGS = gcc -g
OBJ = cs3743p1Driver.o cs3743p1.o

%.o: %.c
		${CFLAGS} -c $<

%.o: %.cpp
		${CFLAGS} -c $<

p1: ${OBJ}
		${CFLAGS} -o p1 ${OBJ}
		
clean:
		rm ${OBJ}
	