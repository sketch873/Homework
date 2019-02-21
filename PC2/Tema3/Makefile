build: dns file

dns: dnsclient.c auxiliare.c print.c
	gcc dnsclient.c auxiliare.c print.c -o dnsclient -g

file: dns.log message.log

dns.log:
	rm -f dns.log
	touch dns.log

message.log:
	rm -f message.log
	touch message.log

clean:
	rm -rf dnsclient dns.log message.log
