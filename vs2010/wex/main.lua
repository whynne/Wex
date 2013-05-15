require "math"
require "actor"
require "edit"
require "game"

mousex = 0
mousey = 0

state = game

player = Actor.new("Haas Foresydes")

function wex.init()

end


function wex.handleEvents()
    if(wex.input.keyPressed(key.insert) == true) then
	    state = editor
	end
	if(wex.input.keyPressed(key.home) == true) then
	    state = game
	end
end


function wex.update()
    state.update()
end


function wex.draw()
    state.draw()
end
