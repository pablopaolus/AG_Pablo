SRC = src
OBJ = obj
BIN = bin
RES = results

CXX = g++
CPPFLAGS = -c -Iinclude
LIBR = ar rsv
XMLFLAGS = -I/usr/include/boost_1_55_0

# ************ Objetivos ************

all: ag_completo

# ************ Compilación ************

ag_completo.o : $(SRC)/ag_completo.cpp
		$(CXX) $(CPPFLAGS) $(XMLFLAGS) $(SRC)/ag_completo.cpp -o $(OBJ)/ag_completo.o

# ************ Enlazado y creación de ejecutables ************

ag_completo: ag_completo.o
		$(CXX) $(OBJ)/ag_completo.o -o $(BIN)/ag_completo

# ************ Limpieza ***************

clean:
	@echo "Cleaning..."
	@\rm -rf $(OBJ)/* $(SRC)/*~

mrproper: clean
	@echo "Mrproper..."
	@\rm -rf $(BIN)/*
	@\rm -rf $(RES)/*
