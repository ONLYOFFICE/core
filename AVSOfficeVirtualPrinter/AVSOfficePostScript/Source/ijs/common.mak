IJS_COMMON_OBJ=ijs$(OBJ)

all:	ijs_client_example$(EXE) ijs_server_example$(EXE)

ijs_client_example$(EXE):	ijs_client_example$(OBJ) ijs_client$(OBJ) $(IJS_COMMON_OBJ) $(IJS_EXEC_SERVER)
	$(CC) $(CFLAGS) $(FE)ijs_client_example$(EXE) ijs_client_example$(OBJ) ijs_client$(OBJ) $(IJS_COMMON_OBJ) $(IJS_EXEC_SERVER) $(LDLIBS)

ijs_server_example$(EXE):	ijs_server_example$(OBJ) ijs_server$(OBJ) $(IJS_COMMON_OBJ)
	$(CC) $(CFLAGS) $(FE)ijs_server_example$(EXE) ijs_server_example$(OBJ) ijs_server$(OBJ) $(IJS_COMMON_OBJ) $(LDLIBS)

common_clean:
	$(RM) *$(OBJ) ijs_client_example$(EXE) ijs_server_example$(EXE)

