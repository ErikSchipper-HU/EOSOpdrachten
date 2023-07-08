all: shell tool

clean:
	rm -f shell tool

test: all
	./testShell.sh ./shell
	./testTool.sh ./tool 42

%: %.cc %.hh
	g++ -o $@ $<
