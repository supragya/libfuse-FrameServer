# libfuse-FrameServer
An experiment with LibFUSE to implement FrameServer using OCcore

The repository is divided as follows:
- `FrameServer-RedChannel` contains a dummy FrameServer made using libfuse. The program is based on earlier implementation of PiNG12RAW. Extracts the red channel from a RAW12 image and provides it as a File in FUSE based filesystem.
- `AVIWriter` contains a simple program to create AVI file from multiple frames. (undone currently)