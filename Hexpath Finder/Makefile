project3: main.o hexoutput.o disjointset.o sampler.o 
		g++ -o project3 main.o hexoutput.o disjointset.o sampler.o

main.o: main.cc hexpathfinder.h disjointset.h sampler.h stack.h
	g++ -c main.cc

hexoutput.o: hexoutput.cc hexpathfinder.h
	g++ -c hexoutput.cc

disjointset.o: disjointset.cc disjointset.h
	g++ -c disjointset.cc

sampler.o: sampler.cc sampler.h
	g++ -c sampler.cc

