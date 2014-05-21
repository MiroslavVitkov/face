
/build/exe: src/main.cpp
	gcc -L/usr/lib64 -L/usr/lib64/ -lstdc++ -lopencv_core -lopencv_video -lopencv_imgproc -lopencv_highgui src/main.cpp -o build/exs

.PHONY : clean
clean:
	rm -rf build/*
