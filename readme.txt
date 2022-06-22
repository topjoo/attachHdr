■ How to compile
$ gcc -o ah.exe -mno-cygwin attachHdr.c

■ cygwin 삭제안될 경우 삭제하는 방법

C:\> takeown /r /d y /f cygwin
C:\> icacls cygwin /t /grant Everyone:F
C:\> 삭제하면 된다.

