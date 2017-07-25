all: pam_test

%.o: %.c
	$(CC) -o $@ -c $<

pam_test: pam_test.o
	$(CC) -o $@ $^ -lpam
	sudo chown root: pam_test
	sudo chmod a+s pam_test

clean:
	rm -f pam_test *.o
