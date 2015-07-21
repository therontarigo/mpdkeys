mpdkeys:
	gcc mpdkeys.c -o mpdkeys	

clean:
	rm mpdkeys

install:
	cp mpdkeys /usr/local/sbin/mpdkeys
