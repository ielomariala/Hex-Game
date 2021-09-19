GSL_PATH?=/net/ens/renault/save/gsl-2.6/install

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g

SRC_DIR=src
INSTALL_DIR=install
TEST_DIR=test
BUILD_DIR=build

GSL_INCLUDE=-I${GSL_PATH}/include
GSL_FLAGS=-L${GSL_PATH}/lib -lgsl -lgslcblas -ldl -lm

DYN_LIB_FLAGS=-shared -fPIC

DYN=-rdynamic --coverage -O0

.PHONY: clean

# Compiles the project
build: server
	cp *.so server ${INSTALL_DIR}

# Copies server and alltests excutable plus all *.so into install directory
install: build test

# Compile the server

server: graph_struct.o server.o libMinimax.so libAdjacenta.so libAtchoum.so libRandomista.so is_winning.o pile.o
	${CC} ${CFLAGS} ${GSL_INCLUDE} ${DYN} ${BUILD_DIR}/server.o ${BUILD_DIR}/graph_struct.o ${BUILD_DIR}/is_winning.o ${BUILD_DIR}/pile.o ${GSL_FLAGS} -o $@

# Run the hex board game
game: install
	LD_LIBRARY_PATH=${GSL_PATH}/lib ./${INSTALL_DIR}/server ./${INSTALL_DIR}/libMinimax.so ./${INSTALL_DIR}/libAdjacenta.so

game2: install
	LD_LIBRARY_PATH=${GSL_PATH}/lib ./${INSTALL_DIR}/server ./${INSTALL_DIR}/libMinimax.so ./${INSTALL_DIR}/libRandomista.so

game3:install
	LD_LIBRARY_PATH=${GSL_PATH}/lib ./${INSTALL_DIR}/server ./${INSTALL_DIR}/libAdjacenta.so ./${INSTALL_DIR}/libRandomista.so


# All player libraries

libRandomista.so: ${SRC_DIR}/Randomista.c ${SRC_DIR}/player.h
	${CC} ${CFLAGS} ${DYN_LIB_FLAGS} ${GSL_INCLUDE} $^ -o $@

libMinimax.so: ${SRC_DIR}/Minimax.c ${SRC_DIR}/player.h
	${CC} ${CFLAGS} ${DYN_LIB_FLAGS} ${GSL_INCLUDE} $^ -o $@

libAdjacenta.so: ${SRC_DIR}/Adjacenta.c ${SRC_DIR}/player.h
	${CC} ${CFLAGS} ${DYN_LIB_FLAGS} ${GSL_INCLUDE} $^ -o $@

libAtchoum1: ${SRC_DIR}/compo_connexes.c
	gcc -fPIC ${CFLAGS} ${GSL_INCLUDE} -c -o cc.o ${SRC_DIR}/compo_connexes.c

libAtchoum2: ${SRC_DIR}/projet_jam.c
	gcc -fPIC ${CFLAGS}  ${GSL_INCLUDE} -c -o pj.o ${SRC_DIR}/projet_jam.c

libAtchoum3: ${SRC_DIR}/set/set_dynamic.c
	gcc -fPIC ${CFLAGS} ${GSL_INCLUDE} -c -o sd.o ${SRC_DIR}/set/set_dynamic.c

libAtchoum4: ${SRC_DIR}/queue.c
	gcc -fPIC ${CFLAGS} ${GSL_INCLUDE} -c -o q.o ${SRC_DIR}/queue.c

libAtchoum5: ${SRC_DIR}/jam_strategy.c
	gcc -fPIC ${CFLAGS}  ${GSL_INCLUDE} -c -o js.o ${SRC_DIR}/jam_strategy.c

libAtchoum.so: libAtchoum1 libAtchoum2 libAtchoum3 libAtchoum4 libAtchoum5
	gcc -shared ${GSL_INCLUDE} -o libAtchoum.so q.o sd.o js.o pj.o cc.o


# All rules to compile files
pile.o: ${SRC_DIR}/pile.h ${SRC_DIR}/pile.c  
	${CC} ${CFLAGS} $^ -c
	mv $@ ${BUILD_DIR}

queue.o: ${SRC_DIR}/queue.h ${SRC_DIR}/queue.c  
	 ${CC} ${CFLAGS} $^ -c
	 mv $@ ${BUILD_DIR}


graph_struct.o: ${SRC_DIR}/graph.h ${SRC_DIR}/graph_struct.h ${SRC_DIR}/graph_struct.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}


server.o: ${SRC_DIR}/server.c ${SRC_DIR}/graph_struct.h
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

Randomista.o: ${SRC_DIR}/player.h ${SRC_DIR}/Randomista.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

Adjacenta.o: ${SRC_DIR}/player.h ${SRC_DIR}/Adjacenta.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

Minimax.o: ${SRC_DIR}/player.h ${SRC_DIR}/Minimax.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

graph_analysis.o: ${SRC_DIR}/graph_analysis.c	
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

compo_connexes.o: ${SRC_DIR}/compo_connexes.c	
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

jam_strategy.o: ${SRC_DIR}/jam_strategy.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

projet_jam.o: ${SRC_DIR}/projet_jam.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} $^ -c
	mv $@ ${BUILD_DIR}

is_winning.o: ${SRC_DIR}/is_winning.h ${SRC_DIR}/is_winning.c 
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}	

set_void.o: ${SRC_DIR}/set/set_void.h ${SRC_DIR}/set/set_void.c 
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}

set_dynamic.o: ${SRC_DIR}/set/set_dynamic.h ${SRC_DIR}/set/set_dynamic.c 
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}


# Compiles and launches tests

test: alltests1 alltests2 alltests3 alltests4 alltests5
	cp alltests* ${INSTALL_DIR}

# Compiles all tests
alltests1:set_dynamic.o compo_connexes_test.o compo_connexes.o jam_strategy_test.o jam_strategy.o graph_analysis_test.o graph_analysis.o graph_struct.o graph_struct_test.o helper_tests_func.o ${TEST_DIR}/tests.c is_winning.o is_winning_test.o ${SRC_DIR}/pile.o queue.o set_void_test.o set_void.o
	${CC} ${CFLAGS} ${GSL_INCLUDE} ${DYN} ${BUILD_DIR}/set_dynamic.o ${BUILD_DIR}/compo_connexes.o ${BUILD_DIR}/compo_connexes_test.o ${BUILD_DIR}/jam_strategy.o ${BUILD_DIR}/jam_strategy_test.o ${BUILD_DIR}/graph_analysis.o ${BUILD_DIR}/graph_analysis_test.o ${BUILD_DIR}/set_void_test.o ${BUILD_DIR}/set_void.o ${BUILD_DIR}/graph_struct.o ${BUILD_DIR}/graph_struct_test.o ${BUILD_DIR}/helper_tests_func.o ${TEST_DIR}/tests.c ${BUILD_DIR}/is_winning.o ${BUILD_DIR}/is_winning_test.o ${SRC_DIR}/pile.o ${BUILD_DIR}/queue.o ${GSL_FLAGS} -ftest-coverage -I ${SRC_DIR} -o $@


alltests2: Randomista.o randomista_test.o helper_tests_func.o graph_struct.o
	${CC} ${CFLAGS} ${GSL_INCLUDE} ${DYN} ${BUILD_DIR}/Randomista.o ${BUILD_DIR}/randomista_test.o ${BUILD_DIR}/helper_tests_func.o ${BUILD_DIR}/graph_struct.o ${GSL_FLAGS} -ftest-coverage -I ${SRC_DIR} -o $@

alltests3: projet_jam.o projet_jam_test.o jam_strategy.o helper_tests_func.o compo_connexes.o graph_struct.o queue.o set_dynamic.o
	${CC} ${CFLAGS} ${GSL_INCLUDE} ${DYN} ${BUILD_DIR}/projet_jam.o ${BUILD_DIR}/projet_jam_test.o ${BUILD_DIR}/jam_strategy.o ${BUILD_DIR}/helper_tests_func.o ${BUILD_DIR}/compo_connexes.o ${BUILD_DIR}/graph_struct.o ${BUILD_DIR}/queue.o ${BUILD_DIR}/set_dynamic.o ${GSL_FLAGS} -ftest-coverage -I ${SRC_DIR} -o $@

alltests4: Adjacenta.o adjacenta_test.o helper_tests_func.o graph_struct.o 
	${CC} ${CFLAGS} ${GSL_INCLUDE} ${DYN} ${BUILD_DIR}/Adjacenta.o ${BUILD_DIR}/adjacenta_test.o ${BUILD_DIR}/helper_tests_func.o ${BUILD_DIR}/graph_struct.o ${GSL_FLAGS} -ftest-coverage -I ${SRC_DIR} -o $@

alltests5: Minimax.o minimax_test.o helper_tests_func.o graph_struct.o
	${CC} ${CFLAGS} ${GSL_INCLUDE} ${DYN} ${BUILD_DIR}/Minimax.o ${BUILD_DIR}/minimax_test.o ${BUILD_DIR}/helper_tests_func.o ${BUILD_DIR}/graph_struct.o ${GSL_FLAGS} -ftest-coverage -I ${SRC_DIR} -o $@

# All rules to compile tests

helper_tests_func.o: ${TEST_DIR}/helper_tests_func.h ${TEST_DIR}/helper_tests_func.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}

graph_struct_test.o: ${SRC_DIR}/graph.h ${SRC_DIR}/graph_struct.h ${TEST_DIR}/graph_struct_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}

adjacenta_test.o: ${SRC_DIR}/player.h ${TEST_DIR}/adjacenta_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}

minimax_test.o: ${SRC_DIR}/player.h ${TEST_DIR}/minimax_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}

randomista_test.o: ${SRC_DIR}/player.h ${TEST_DIR}/randomista_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -c $^
	mv $@ ${BUILD_DIR}

is_winning_test.o: ${TEST_DIR}/is_winning_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -I ${SRC_DIR} -c $^
	mv $@ ${BUILD_DIR}

set_void_test.o: ${TEST_DIR}/set_void_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -I ${SRC_DIR} -c $^
	mv $@ ${BUILD_DIR}

graph_analysis_test.o: ${TEST_DIR}/graph_analysis_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -I ${SRC_DIR} -c $^
	mv $@ ${BUILD_DIR}

jam_strategy_test.o: ${TEST_DIR}/jam_strategy_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -I ${SRC_DIR} -c $^
	mv $@ ${BUILD_DIR}

projet_jam_test.o: ${TEST_DIR}/projet_jam_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -I ${SRC_DIR} -c $^
	mv $@ ${BUILD_DIR}

compo_connexes_test.o: ${TEST_DIR}/compo_connexes_test.c
	${CC} ${CFLAGS} ${GSL_INCLUDE} -I ${SRC_DIR} -c $^
	mv $@ ${BUILD_DIR}

# Clear all executables and libraries
clean:
	rm -f *.so server *.gcda *.gcno *.o alltests* ${BUILD_DIR}/* ${INSTALL_DIR}/* ${SRC_DIR}/*.gch ${TEST_DIR}/*.gch ${SRC_DIR}/*.o ${SRC_DIR}/set/*.o ${SRC_DIR}/set/*.gch
