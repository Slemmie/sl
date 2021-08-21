export:
	if [ -d "/usr/local/include/sl" ]; then rm -rf /usr/local/include/sl; fi
	mkdir /usr/local/include/sl
	cp -r src/* /usr/local/include/sl
	mkdir /usr/local/include/sl/scripts
	cp scripts/prep* /usr/local/include/sl/scripts
	cp scripts/copy_template* /usr/local/include/sl/scripts
	cp scripts/std_libs /usr/local/include/sl/scripts
	cp scripts/ignore_lines /usr/local/include/sl/scripts
