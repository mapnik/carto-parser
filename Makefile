all:
	@scons

clean:
	@scons -c

test:
	for FILE in tests/*mss; do \
		./carto --in $${FILE} --out /tmp/$$(basename $${FILE}).xml; \
	done

grind:
	for FILE in tests/*mss; do \
		valgrind --leak-check=full --log-fd=1 ./carto $${FILE} | grep definitely; \
	done

.PHONY: all clean test
