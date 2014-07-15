startCode: startCode.c 
	gcc startCode.c -o startCode

install: startCode
	sh install.sh

uninstall: 
	rm -rf ~/.startCode /usr/bin/startCode & echo "Removed binary and ~/.startCode folder"

clean:
	rm startCode
