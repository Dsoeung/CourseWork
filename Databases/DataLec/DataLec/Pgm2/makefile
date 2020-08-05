CFLAGS = gcc -g
OBJ = cs3743p2Driver.o cs3743p2.o

%.o: %.c
		${CFLAGS} -c $<

%.o: %.cpp
		${CFLAGS} -c $<

p2: ${OBJ}
		${CFLAGS} -o p2 ${OBJ}
		
clean:
		rm ${OBJ}
	