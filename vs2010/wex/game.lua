game = {}


function game.update()
	mousex,mousey = wex.input.getMousePosition()
	player:update()
end


function game.draw()
    wex.graphics.drawQuad(player.quad,player.x,player.y)
end
