main: example.cpp ThreadPool.hpp
	g++ $< -o $@ -lpthread

main2: example2.cpp
	g++ $< -o $@ -lpthread
	

.PHONY: clean
clean:
	rm main
	rm main2
