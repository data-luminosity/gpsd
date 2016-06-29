CC=gcc
CFLAGS= -g
OBJECTS = app_entry.o setting_manager.o priv_handler.o coordinate_calculations.o main.o


all: test_settings

test_settings: main setting_manager app_entry priv_handler coordinate_calculations
	$(CC) $(OBJECTS)  -o test_settings -lm
main:
	$(CC) $(CFLAGS) -c main.c
setting_manager:
	$(CC) $(CFLAGS) -c setting_manager.c
app_entry:
	$(CC) $(CFLAGS) -c app_entry.c
priv_handler:
	$(CC) $(CFLAGS) -c priv_handler.c
coordinate_calculations:
	$(CC) $(CFLAGS) -c coordinate_calculations.c


clean: 
	rm *.o test
