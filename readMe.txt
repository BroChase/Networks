CSCI 4761: Introduction to Computer Networks Lab 2
*****************************************************
Learn how to develop an application protocol as well as build an application over UNIX(linux) socket.
*******************************************************
##Chase Brown
##Student ID 106015389
##Source Files (Client)
	main.cpp
	ClientSocket.cpp
	ClientSocket.h
	passwordfunctions.cpp
	passwordfunctions.h
	users.cpp
	users.h
##Source Files(Server)
	main.cpp
	ClientSocket.cpp
	ClientSocket.h
	fileops.cpp
	fileops.h
	passwordfunctions.cpp
	passwordfunctions.h
	users.cpp
	users.h
##C++   Tested on csegrid from 03/20/2018 to 04/06/2018

##Loading server
	Start up a node on the cse grid.  Inside of "/Networks_lab2_server_v2/"
##To Make
	make
##To Run
	./server
##to Clean
	make clean

##Loading Client
	Start up a seperate node on the cse grid.  Inside of "/Networks_lab2_client_v2/"
##To Make
	make
##To Run !"replace XX with node server is currently running on."!
	./client ucdencsecnodeXX.ucdenver.pvt
##to Clean
	make clean
