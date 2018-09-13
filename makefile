
BIN_DIR = build
SRC_DIR = src

tests:
	cd $(SRC_DIR) && $(MAKE)

clean:
	$(RM) $(BIN_DIR)/* 
