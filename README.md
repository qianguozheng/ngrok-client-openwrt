# ngrok-client-openwrt
Ngrok Client for openwrt package

Remote Debugging
---
For openwrt users, maybe need to modify config or do some operations outside of NAT,
the easy way is to use *ngrok*, 

	Local Router( NAT ) -- Server --Remote User
	Ngrok client(On OpenWrt) --Server(Ngrok Server) -- Remote User
	
From above you can see the simple Arch of each role in this program.

I have setup a server on ![hiweeds.net](hiweeds.net).

###On Server:
	
	ngrokd -domain hiweeds.net
	
###On OpenWrt:
	
	/usr/sbin/ngrokc -SER[Shost:hiweeds.net,Sport:4443] -AddTun[Type:tcp,Lhost:127.0.0.1,Lport:22,Rport:8022]
	
###On Remote Client:

	ssh -p 8022 root@hiweeds.net
	
Now, we are login on OpenWrt system in anywhere that can access the Internet.

Remote Web/Samba/Telnet/SSH Service and so on
---

Do as above, you can get access to any service your openwrt provide on local.


Base on: https://github.com/dosgo/ngrok-c.git

Contact Me: guozhengqian0825@126.com [AliPay]
If you found useful, donate will be appreciate. 

