# Network Lab 2

## Server side (Run this first)

- Upload server folder and client folder to csegrid

- cd into server folder then type: make 
  - To run server type: ./server

```
make
./server
```

## Client side (Run this second)

- cd into client folder then type: make
  - To run client type: ./client csci-gnode-01 <- The number "01" varies depending on your server terminal number. This is just a sample case where your server runs on @csci-gnode-01:~/server
  
```
make
./client csci-gnode-01
```

## How to run program (on client side)

- command "ls": Show files at the client’s current directory
```
For command options type: 'help'
> ls
```

- command "pwd": Display of current directory of the client
```
For command options type: 'help'
> pwd
```

- command "spwd": Display current directory of the server
```
For command options type: 'help'
> spwd
```

- command "catalog": Show files at the server’s current directory
```
For command options type: 'help'
> catalog
```

- command "download": Download files
```
For command options type: 'help'
> download

Enter download source filename:
> [Source filename (locates in server)]

Enter download destination filename:
> [Destination filename (new file name to save in client)]
```

- command "upload": Upload files
```
For command options type: 'help'
> upload

Enter download source filename:
> [Source filename (locates in client)]

Enter download destination filename:
> [Destination filename (new file name to save in server)]
```

- command "help": Print all avaliable commands
```
For command options type: 'help'
> help
```

- command "bye": Disconnect from server
```
For command options type: 'help'
> bye
```

## Status

- 100% working!

