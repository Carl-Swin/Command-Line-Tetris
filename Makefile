PP = g++ -std=c++17

F1 = Main
F2 = Tetris
F3 = $(F2)_Game
F4 = $(F2)_Input

# $  : denotes variables
# $@ : this is the name of the target
# $< : this is the name of the first prerequisite
# $^ : this is the names of all the prerequisites


all: $(F2).exe

cnsl: $(F2)_cnsl.exe

win: $(F2)_win.exe


$(F2).exe: $(F1).o $(F2).o $(F3).o $(F4).o
	$(PP) $^ -o $@
$(F2)_cnsl.exe: $(F1).o $(F2).o $(F3).o $(F4).o
	$(PP) $^ -lgdi32 -lwsock32 -lws2_32 -o $@
$(F2)_win.exe: $(F1).o $(F2).o $(F3).o $(F4).o
	$(PP) $^ -lgdi32 -lwsock32 -lws2_32 -o $@ -Wl,--subsystem,windows


$(F1).o: $(F1).cpp $(F2).h
	$(PP) -c $<
$(F2).o: $(F2).cpp $(F2).h
	$(PP) -c $<
$(F3).o: $(F3).cpp $(F2).h
	$(PP) -c $<
$(F4).o: $(F4).cpp $(F2).h
	$(PP) -c $<


clean: 
	del *.o

wipe:
	del *.o
	del $(APP)*.exe


vi:
	vim *.cpp *.h [Mm]akefile -p
