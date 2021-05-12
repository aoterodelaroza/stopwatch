// stopwatch: a simple stopwatch program for the terminal.
//
// Copyright (c) 2021 Alberto Otero de la Roza <aoterodelaroza@gmail.com>
//
// stopwatch is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// stopwatch is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){

  // global constants
  const int BUFLEN = 40; // output buffer length
  const char *backstr = "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; // backtrack string

  // global variables
  int usemsec = 0;  // print milliseconds if true
  char buf[BUFLEN]; // buffer for the output string

  // parse the command line options (-h=help, -m=milliseconds)
  for(int i=1; i<argc; i++){
    if (!strcmp(argv[i],"-h")) {
      printf("Usage: %s [-h] [-m] \n\n",argv[0]);
      printf("    -h : show this help message.\n");
      printf("    -m : print milliseconds.\n\n");
      printf("Press ENTER to record a time. Control-C exits.\n");
      return 0;
    } else if (!strcmp(argv[i],"-m")){
      usemsec = 1;
    }
  }

  // sleep time and initial time
  struct timeval t0;
  unsigned int sleeptime;
  if (usemsec)
    sleeptime = 100; // tenth of a millisecond
  else
    sleeptime = 100000; // tenth of a second
  gettimeofday(&t0,NULL);

  while (1){
    // get the time
    struct timeval t1;
    gettimeofday(&t1,NULL);

    // calculate the time
    long long int dt = (t1.tv_sec - t0.tv_sec) * 1000LL + (t1.tv_usec - t0.tv_usec) / 1000LL;
    int msec = dt % 1000;
    dt = (dt - msec) / 1000;
    int sec = dt % 60;
    dt = (dt - sec) / 60;
    int min = dt % 60;
    dt = (dt - min) / 60;
    int h = dt % 24;
    long long int day = (dt - h) / 24;

    // build and write the message
    int backlen = 0;
    buf[0] = '\0';
    if (day > 0){
      backlen = sprintf(buf,"%lld-",day);
      if (backlen > BUFLEN-12){
	fprintf(stderr,"\nError: buffer exceeded - Not one will care at last when it is done...\n");
	return 1;
      }
    }
    if (day > 0 || h > 0)
      sprintf(buf,"%s%2.2d:",buf,h);
    backlen = sprintf(buf,"%s%2.2d:%2.2d",buf,min,sec);
    if (usemsec)
      backlen = sprintf(buf,"%s:%3.3d",buf,msec);
    printf("%s",buf);
    fflush(stdout);

    // sleep and backtrack
    usleep(sleeptime);
    printf("%.*s",backlen,backstr);
  }

  return 0;
}
