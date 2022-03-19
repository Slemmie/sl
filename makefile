export:
	if [ -d "/usr/local/include/sl/" ]; then rm -rf /usr/local/include/sl/; fi;
	mkdir /usr/local/include/sl/
	cp -r src/* /usr/local/include/sl/
	cp scripts/copy_template.sh /usr/local/include/sl/
	g++ -std=c++17 scripts/dif.cpp -o /usr/local/include/sl/dif
	g++ -std=c++17 scripts/prep.cpp -o /usr/local/include/sl/prep
	cp scripts/ignore_lines /usr/local/include/sl/
	cp scripts/std_libs /usr/local/include/sl/
