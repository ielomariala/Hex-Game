# HEX PROJECT

## This is not Final Version of project

**HEX PROJECT** is a programming project designed by Mr. David RENAULT, conducted by all first year informatics engineering students of ENSEIRB-MATMECA in the 2019/2020 scholar year.

Briefly, students are divided to 4 students' groups. The goal is to have us work together and think outside the box and write a highly robust code using all of the knowledge and assets we learned through out this semester. 

## 1	 Compilation:
Once you are locally in a version of this project, compile using the "Makefile". It has different functionnalities:
     You can write "make" in order to compile the server and multiple clients as dynamic libraries (.so)
     You can write "make install" in order to copy important executables (server, .so and alltets*) to one directory named "install"
     You can write "make test" to compile the tests and to create many exacutables (alltests1, alltests2, alltests3, ...). This will give you the number of tests, related to many structures and functions (alltests1), or even strategies (alltests*), that have passed correctly. Of course, if an error occurs, you will be informed exaclty which test in which structure did the test fail. 
     You can also write "make clean" in order to delete all the executables 'server' , 'alltests*',  clients (*.so), and the linkers ( .o files) if they aren't already deleted.

## 2   	 Locally:
You'll see in addition to the different versions of the project, many files:
       authors.txt : A file containing the authors of this project
       README.txt: This file describes our project
       name.txt: This file contains one string (our name in the ladder)
You'll also find 5 directories:
        src: Contains all source codes, for structures, clients and for the server.
        test: Contains all source codes for tests.
        install: Empty folder. When you excute "make install", it'll contain important executables.
        build: Empty folder. Well, this is the heaven of linkers (*.o). Every linker compiled during comilation resides there.
        report: Conatins hex.tex which is the source code of our report, written in LaTeX. Unfortunately, we couldn't commit and push images because we were exceeding the limit of pushed (.png) files. So, this file can't compile correctly.

       

## 3   	  Usage:
In order to launch a game of hex in 11 dimensionned hexagonal graph between player1 and player2:
    - make
    - make install
    -./install/server -m11 -th ./install/player1.so ./install/player2.so
In order launch tests:
    -make install
    -make test
In order to clean the deposit:
    -make clean

    
## 4    	License
	ENSEIRB-MATMECA  

# Reminder: This is not Final Version of project
