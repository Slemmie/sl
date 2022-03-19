export:
	if [ -d "/usr/local/include/sl/" ]; then rm -rf /usr/local/include/sl/; fi;
	mkdir /usr/local/include/sl/
	cp -r src/* /usr/local/include/sl/
	mkdir /usr/local/include/sl/scripts/
	cp scripts/copy_template.sh /usr/local/include/sl/scripts/
	g++ -std=c++17 scripts/dif.cpp -o /usr/local/include/sl/scripts/dif
	g++ -std=c++17 scripts/prep.cpp -o /usr/local/include/sl/scripts/prep
	cp scripts/ignore_lines /usr/local/include/sl/scripts/
	cp scripts/std_libs /usr/local/include/sl/scripts/
