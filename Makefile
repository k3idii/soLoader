all:
	gcc loader.c -o loader_32 -ldl  -m32 
	gcc loader.c -o loader_64 -ldl  -m64 

