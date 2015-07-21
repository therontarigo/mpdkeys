/*

    mpdkeys: bind keys on a Linux event device to control Music Player Daemon

    Usage: mpdkeys [DEVICE]
        where [DEVICE] is a Linux event device in /dev/input

    Copyright 2015 Theron Tarigo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#define KEYCODE_PLAY 164
#define KEYCODE_PREV 165
#define KEYCODE_NEXT 163

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

int main(int argc, char *argv[]) {

	const char* dev;

	if (argc!=2 || (argc==2 && !strcmp(argv[1],"-h") ) ) {
		fprintf(stderr, "Usage: mpdkeys [DEVICE]\n");
		return 1;
	} else {
		dev = argv[1];
	}

	struct input_event ev;

	int fd = -1;

	printf("Using device %s\n",dev);

	while (1) {

		if (access(dev, F_OK)==-1) {
			if (fd!=-1) {
				close(fd);
				printf("Lost connection from device\n");
			}
			printf("Waiting for connection from device... ",dev);
			fflush(stdout);
			while (access(dev, F_OK)==-1) {
				sleep(1);
			}
			printf("Device connected\n");
			fflush(stdout);
		}

		if (fd==-1 || fcntl(fd, F_GETFD)==-1) {
			fd = open(dev, O_RDONLY);
		}

		struct pollfd pfd;
		pfd.fd = fd;
		pfd.events=( POLLIN );

		int st = poll(&pfd, 1, 1000);

		if (st) {

			read(fd, &ev, sizeof(ev));

			if (ev.type==EV_KEY && ev.value==0) {
				if ((int)ev.code==KEYCODE_PLAY) {
					system("mpc toggle");
				}
				if ((int)ev.code==KEYCODE_PREV) {
					system("mpc prev");
				}
				if ((int)ev.code==KEYCODE_NEXT) {
					system("mpc next");
				}
			}
		}
	}

	close(fd);

	return 0;
}
