## AVIEncode

This system encodes frames to uncompressed AVI format. Based on references:
```
https://msdn.microsoft.com/en-us/library/windows/desktop/dd318189(v=vs.85).aspx
https://cdn.hackaday.io/files/274271173436768/avi.pdf
```
**BUILD:** Use `make` to build system
**RUN:** Use `./avi_generator`
**OUTPUT:** `AviFile.avi`

Encoder available in `src/AviEncode`

Current TODO (strikethrought are done):

0. Compute filesizes for list and chunks
1. ~~Write RIFF tag~~
2. ~~Write HDR1 list~~
3. Write MOVI list
4. Encode Frames
