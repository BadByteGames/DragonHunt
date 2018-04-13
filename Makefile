DRAGONHUNTDIR=DragonHunt/DragonHunt

TINYXMLDIR=DragonHunt/tinyxml2

BINDIR=bin/

INCLUDEARG=-I DragonHunt -std=c++11

LIBARG=-L $(BINDIR)

all: DragonHunt

rebuild: clean all

clean:
	$(RM) *.o

DragonHunt: tinyxml2lib Player.o XMLHandler.o Adventure.o AdventureItem.o CCHandler.o Events.o Location.o Logger.o Parser.o main.o
	g++ $(BINDIR)Player.o $(BINDIR)XMLHandler.o $(BINDIR)Adventure.o $(BINDIR)AdventureItem.o $(BINDIR)CCHandler.o $(BINDIR)Events.o $(BINDIR)Location.o $(BINDIR)Logger.o $(BINDIR)Parser.o $(BINDIR)main.o $(LIBARG) -o $(BINDIR)DragonHunt -ltinyxml2

Player.o:
	g++ -c $(DRAGONHUNTDIR)/Player.cpp -o $(BINDIR)$@ $(INCLUDEARG)
	
XMLHandler.o:
	g++ -c $(DRAGONHUNTDIR)/XMLHandler.cpp -o $(BINDIR)$@ $(INCLUDEARG)
	
Adventure.o: 
	g++ -c $(DRAGONHUNTDIR)/Adventure.cpp -o $(BINDIR)$@ $(INCLUDEARG)

AdventureItem.o: 
	g++ -c $(DRAGONHUNTDIR)/AdventureItem.cpp -o $(BINDIR)$@ $(INCLUDEARG)

CCHandler.o: 
	g++ -c $(DRAGONHUNTDIR)/CCHandler.cpp -o $(BINDIR)$@ $(INCLUDEARG)

Events.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Events.cpp -o $(BINDIR)$@

Location.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Location.cpp -o $(BINDIR)$@

Logger.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Logger.cpp -o $(BINDIR)$@
	
Parser.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Parser.cpp -o $(BINDIR)$@

main.o: $(DRAGONHUNTDIR)/main.cpp 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/main.cpp -o $(BINDIR)$@

tinyxml2lib: libtinyxml2.a

libtinyxml2.a: tinyxml2.o
	$(AR) $(ARFLAGS)s $(BINDIR)$@ $(BINDIR)$^

tinyxml2.o: 
	g++ -c $(TINYXMLDIR)/tinyxml2.cpp -o $(BINDIR)$@