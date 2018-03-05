# libfuse-FrameServer
An experiment with LibFUSE to implement FrameServer using OCcore

## What does the application do?
Currently not much. It takes in a RAW12 image, finds the red channel and serves the file using the created FileSystem. The file is raw in nature and every 8 bits give the gray shade of the pixel.

## Getting the system up and running
Clone the directory and build using
```
cc program.c -D_FILE_OFFSET_BITS=64 -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
```

Once built, you have the application `a.out`. Open a new terminal and use the following commands to run:
```
mkdir /tmp/newfsmountpoint
./a.out -d -s -f /tmp/newfsmountpoint
```

This will start the FUSE FS on the terminal. Go back to previous terminal and make sure it is mounted, then
```
cd /tmp/newfsmountpoint
```

Now you may do normal operations with the file system. Copy the `redbytedump` file from the new file sytem to the current directory and open the file with a hex editor to verify.
