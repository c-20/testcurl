set PATH=C:\Windows\System32;C:\MinGW\bin
set gcc=mingw32-gcc.exe
set gpp=mingw32-g++.exe
set src=main.c
set out=main.o
set exe=testcurl.exe
set libs=lib\libcurl.a lib\libcurl.dll.a

%gcc% -Wall -g -Iinclude -c %src% -o %out%
%gpp% -o %exe% %out% %libs%


goto end

% This will compile testcurl.exe from main.c
% Assuming MinGW exists at C:\MinGW (https://github.com/c-20/MinGW)
% Note that it overrides system PATH but only in this cmd window

:end