.PHONY: all
all: generator primeCounter

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c
	gcc -o primeCounter primeCounter.c -lm -pthread

.PHONY: clean
clean:
	-rm randomGenerator primeCounter 2>/dev/null