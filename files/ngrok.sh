#/bin/sh

/usr/sbin/ngrokc -SER[Shost:hiweeds.net,Sport:4443] -AddTun[Type:tcp,Lhost:127.0.0.1,Lport:22,Rport:8022] &
