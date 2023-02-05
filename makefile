compile:

	g++ -Wall -pedantic-errors -o head head.c
	g++ -Wall -pedantic-errors -o tail tail.c
	g++ -Wall -pedantic-errors -o true true.c
	g++ -Wall -pedantic-errors -o false false.c
	g++ -Wall -pedantic-errors -o wc wc.c
	g++ -Wall -pedantic-errors -o env env.c

run:

	./head
	./tail
	./true
	./false
	./wc
	./env

clean:

	rm -f head
	rm -f tail
	rm -f true
	rm -f false
	rm -f wc
	rm -f env

