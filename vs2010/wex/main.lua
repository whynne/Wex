require "math"
require "actor"
require "edit"
require "game"

mousex = 0
mousey = 0

wex.console.setRows(20)
wex.console.setColumns(100)
wex.console.setColor(0,0,1,1)

state = game

player = Actor("Haas Foresydes")

function wex.init()

end

function wex.handleEvents()
    if(wex.input.keyPressed(key.insert) == true) then
	    state = editor
	end
	if(wex.input.keyPressed(key.home) == true) then
	    state = game
	end
	    state.handleEvents()
end

function wex.update()
    state.update()
end


function wex.draw()
    state.draw()
end

function wex.mouseDown(x,y,button)
    state.mouseDown(x,y,button)
end
