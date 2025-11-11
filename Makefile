jotplusplus : jotplusplus.cpp
	g++ jotplusplus.cpp -o jotplusplus -std=c++14 -I/usr/local/include -L/usr/local/lib -lfltk -lfltk_images -lsqlite3
	@ echo "compiled succesfully"

.PHONY: clean

clean:
	rm -f notes.db jotplusplus
	@echo Cleaned succesfully.