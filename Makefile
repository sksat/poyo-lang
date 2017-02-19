TARGET	= poyo
OBJS	= main.o bf-vm.o

%.o:%.c
	gcc -g -c $<

default:
	make $(TARGET)

run:
	make
	./poyo

$(TARGET):$(OBJS)
	gcc -o $@ $^


