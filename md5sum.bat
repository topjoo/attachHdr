@REM --------------------------------------------------------
@REM MD5Sum and SHA1Sum
@REM --------------------------------------------------------
IF EXIST ah.exe (
        echo MD5Sum
    ) ELSE (
        rename ah.__e   ah.exe 
    )
@REM --------------------------------------------------------
@REM Modify the input file name or file name extension.
@REM  --input @.zip
@REM --------------------------------------------------------
@REM --- SHA1 or MD5 hash ---
ah.exe --input @.gz  --output SHA1Sum.txt  --checksum MD5 --verbose size
ah.exe --input @.gz  --append SHA1Sum.txt  --checksum SHA1  --verbose size
@REM --------------------------------------------------------
@REM --------------------------------------------------------
PAUSE
