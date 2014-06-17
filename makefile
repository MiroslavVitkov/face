# For my Fedora 19.
LIBPATHS= -L/usr/lib64 -L/usr/lib64/
LIBS= -lstdc++ -lopencv_core -lopencv_video -lopencv_imgproc -lopencv_highgui -lopencv_objdetect
INCLUDEPATHS=

# For Windows 7 isntallation.
#LIBPATHS= -L"..\opencv\mybuild\install\x86\mingw\lib"
#LIBS= -lstdc++ -lopencv_core249 -lopencv_video249 -lopencv_imgproc249 -lopencv_highgui249 -lopencv_objdetect249
#INCLUDEPATHS= -I"..\opencv\mybuild\install\include"


/build/exe: src/main.cpp
	gcc   $(LIBPATHS) $(LIBS) $(INCLUDEPATHS) src/main.cpp -o build/exe

.PHONY : clean
clean:
	rm -rf build/*


#--------- theirs---------------------#

#CC = g++
#CFLAGS = -g -Wall
 
#SRCS = src/main.cpp
#PROG = HelloWorld.exe
 
#OPENCV = -I"..\opencv\mybuild\install\include" -L"..\opencv\mybuild\install\x86\mingw\lib" -lstdc++ -lopencv_core249 -lopencv_video249 -lopencv_imgproc249 -lopencv_highgui249 -lopencv_objdetect249

 
#$(PROG) : $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(OPENCV)
