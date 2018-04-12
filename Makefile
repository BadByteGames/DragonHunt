DRAGONHUNTDIR=DragonHunt/DragonHunt

TINYXMLDIR=DragonHunt/tinyxml2

INCLUDEARG=-I DragonHunt -std=c++11

LIBARG=-L .

all: DragonHunt

rebuild: clean all

clean:
	$(RM) *.o

DragonHunt: tinyxml2lib Player.o XMLHandler.o Adventure.o AdventureItem.o CCHandler.o Events.o Location.o Logger.o Parser.o main.o
	g++ Player.o XMLHandler.o Adventure.o AdventureItem.o CCHandler.o Events.o Location.o Logger.o Parser.o main.o $(LIBARG) -o DragonHunt -ltinyxml2

Player.o:
	g++ -c $(DRAGONHUNTDIR)/Player.cpp -o $@ $(INCLUDEARG)
	
XMLHandler.o:
	g++ -c $(DRAGONHUNTDIR)/XMLHandler.cpp -o $@ $(INCLUDEARG)
	
Adventure.o: 
	g++ -c $(DRAGONHUNTDIR)/Adventure.cpp -o $@ $(INCLUDEARG)

AdventureItem.o: 
	g++ -c $(DRAGONHUNTDIR)/AdventureItem.cpp -o $@ $(INCLUDEARG)

CCHandler.o: 
	g++ -c $(DRAGONHUNTDIR)/CCHandler.cpp -o $@ $(INCLUDEARG)

Events.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Events.cpp -o $@

Location.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Location.cpp -o $@

Logger.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Logger.cpp -o $@
	
Parser.o: 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/Parser.cpp -o $@

main.o: $(DRAGONHUNTDIR)/main.cpp 
	g++ $(INCLUDEARG) -c $(DRAGONHUNTDIR)/main.cpp -o $@

tinyxml2lib: libtinyxml2.a

libtinyxml2.a: tinyxml2.o
	$(AR) $(ARFLAGS)s $@ $^

tinyxml2.o: 
	g++ -c $(TINYXMLDIR)/tinyxml2.cpp -o $@