
export:
	if [ -d "/usr/local/include/sl" ]; then rm -rf /usr/local/include/sl; fi
	mkdir /usr/local/include/sl
	cp src/*.h /usr/local/include/sl
	mkdir /usr/local/include/sl/scripts
	cp scripts/prep* /usr/local/include/sl/scripts
	cp scripts/std_libs /usr/local/include/sl/scripts
