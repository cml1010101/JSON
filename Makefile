SRCS = $(wildcard src/*.cpp)
OBJS = ${SRCS:.cpp=.o}
all: libjson_1.0-1_amd64.deb

libjson_1.0-1_amd64.deb: libjson_1.0-1_amd64/DEBIAN/control libjson_1.0-1_amd64/usr/lib/libjson.a libjson_1.0-1_amd64/usr/include/JSON
	dpkg-deb --build --root-owner-group libjson_1.0-1_amd64
	rm -r libjson_1.0-1_amd64
libjson_1.0-1_amd64/DEBIAN/control:
	mkdir -p libjson_1.0-1_amd64/DEBIAN
	cp control libjson_1.0-1_amd64/DEBIAN/control
libjson_1.0-1_amd64/usr/lib/libjson.a: libjson.a
	mkdir -p libjson_1.0-1_amd64/usr/lib
	cp libjson.a libjson_1.0-1_amd64/usr/lib/libjson.a
libjson_1.0-1_amd64/usr/include/JSON:
	mkdir -p libjson_1.0-1_amd64/usr/include
	cp -r include libjson_1.0-1_amd64/usr/include/JSON
libjson.a: ${OBJS}
	ar rcs libjson.a ${OBJS}
	rm ${OBJS}
%.o: %.cpp
	g++ -c $< -o $@ -Iinclude
clean:
	rm -f -r libjson_1.0-1_amd64 ${OBJS} libjson.a