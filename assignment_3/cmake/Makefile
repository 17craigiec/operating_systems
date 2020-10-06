#Executable Outputs
addem: addem.o Msg.o
	g++ -pthread addem.o Msg.o -o addem
	rm *.o

#Mains
addem.o: ../mains/addem.cpp
	g++ -pthread -c ../mains/addem.cpp

#Supporting classes
Msg.o: ../private/Msg.cpp ../public/Msg.h
	g++ -pthread -c ../private/Msg.cpp

clean:
	rm *.csv *.o addem
