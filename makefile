
export:
	if [ -d "/usr/local/include/sl" ]; then rm -rf /usr/local/include/sl; fi
	mkdir /usr/local/include/sl
	cp *.h /usr/local/include/sl
