CC = $(CXX)
CXXFLAGS = -w
LDFLAGS = -L/usr/local/lib -L/usr/lib -lgd -lpng -lz -ljpeg -lfreetype -lm


imagemapmaker: imagemapmaker.o Application.o Config.o ImageMap.o

imagemapmaker.o: imagemapmaker.cpp
Application.o: Application.h Application.cpp Config.o ImageMap.o
Config.o: Config.h Config.cpp
ImageMap.o: ImageMap.h ImageMap.cpp Config.o

clean:
	rm *.o
	rm imagemapmaker
