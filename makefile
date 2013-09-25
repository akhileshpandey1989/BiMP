INC_PATH=inc
SRC_PATH=src
LIB_PATH=lib
OBJ_PATH=obj
CPP_COMPILER=g++
 
$(LIB_PATH)/libbimp.a : $(OBJ_PATH)/BMP_Header.o $(OBJ_PATH)/DIB_Header.o $(OBJ_PATH)/BMP_File.o $(OBJ_PATH)/utility_functions.o $(OBJ_PATH)/exception.o
	@echo "Creating libbimp...";
	@ar rcs $(LIB_PATH)/libbimp.a $(OBJ_PATH)/BMP_Header.o $(OBJ_PATH)/DIB_Header.o $(OBJ_PATH)/BMP_File.o $(OBJ_PATH)/utility_functions.o $(OBJ_PATH)/exception.o
	@echo "Done";

$(OBJ_PATH)/BMP_Header.o : $(SRC_PATH)/BMP_Header.cpp $(INC_PATH)/utility_functions.h $(INC_PATH)/BMP_Header.h $(INC_PATH)/exception.h
	@echo "Compiling BMP_Header.cpp"
	@$(CPP_COMPILER) -c -I$(INC_PATH) $(SRC_PATH)/BMP_Header.cpp -o $(OBJ_PATH)/BMP_Header.o
	@echo "Done";

$(OBJ_PATH)/DIB_Header.o : $(SRC_PATH)/DIB_Header.cpp $(INC_PATH)/utility_functions.h $(INC_PATH)/DIB_Header.h $(INC_PATH)/exception.h
	@echo "Compiling DIB_Header.cpp..."
	@$(CPP_COMPILER) -c -I$(INC_PATH) $(SRC_PATH)/DIB_Header.cpp -o $(OBJ_PATH)/DIB_Header.o
	@echo "Done";

$(OBJ_PATH)/BMP_File.o : $(SRC_PATH)/BMP_File.cpp $(INC_PATH)/utility_functions.h $(INC_PATH)/BMP_Header.h $(INC_PATH)/exception.h $(INC_PATH)/BMP_Header.h $(INC_PATH)/DIB_Header.h
	@echo "Compiling BMP_File.cpp..."
	@$(CPP_COMPILER) -c -I$(INC_PATH) $(SRC_PATH)/BMP_File.cpp -o $(OBJ_PATH)/BMP_File.o
	@echo "Done";

$(OBJ_PATH)/utility_functions.o : $(SRC_PATH)/utility_functions.cpp $(INC_PATH)/utility_functions.h
	@echo "Compiling utility_functions.cpp..."
	@$(CPP_COMPILER) -c -I$(INC_PATH) $(SRC_PATH)/utility_functions.cpp -o $(OBJ_PATH)/utility_functions.o
	@echo "Done";

$(OBJ_PATH)/exception.o : $(SRC_PATH)/exception.cpp $(INC_PATH)/exception.h
	@echo "Compiling exception.cpp..."
	@$(CPP_COMPILER) -c -I$(INC_PATH) $(SRC_PATH)/exception.cpp -o $(OBJ_PATH)/exception.o
	@echo "Done";

.PHONY : clean
clean: 
	@\rm -f $(LIB_PATH)/*
	@\rm -f $(OBJ_PATH)/*
	@echo "Cleaning the obj and lib areas..."
