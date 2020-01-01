SRC=$(wildcard *.c)
LIB= -lm -lrt 
OBJ=$(addprefix ./, $(addsuffix .o, $(basename $(SRC))))
TARGET=knn
all: $(TARGET)
$(TARGET): $(SRC)
	$(CC)  -o $@  $^ $(CFLAGS) $(LDFLAGS) $(LIB)
clean:
	rm -f $(TARGET) $(OBJ)

