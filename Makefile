cc=g++
cflags=
ldflags=
names=	audio			\
		controller		\
		engine			\
		graphics		\
		main			\
		mainstate		\
		matrix			\
		net				\
		physics			\
		timer			\
		triglookup		\
		ui				\
		wex				\
		world			\

sources=$(addprefix src/,$(addsuffix .cpp,$(names)))
objects=$(addsuffix .o,$(names)) 
targetdir=build/
name=wex_game
include_dir = ./include

all : $(name)

$(name) : $(objects)
	$(cc) -o $(name) -L ./lib $(objects) ./lib/mac/libSDL2-2.0.0.dylib ./lib/mac/libGLEW.2.0.0.dylib -framework OpenGL -framework OpenAL
	echo SOURCES DONE

$(objects) : $(sources)
	$(cc) -c $(cflags) -I$(include_dir) $(sources)
	echo OBJECTS DONE

clean : 
	rm -f $(objects)

fclean : clean 
	rm -f $(name)

re : fclean all 
