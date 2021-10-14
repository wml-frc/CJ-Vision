## Testing methods (Used netcat)

- `netstat -p UDP -a` checks computer for open udp sockets

### NT check

#### Singular

- Test using nmap
	##### Server recv
	- `ncat -vv -u <ip address> <port>` allows sending of char data

	##### Client send
	- `ncat -vv -ulp <port>` allows receiving of char data
