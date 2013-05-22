require "edit"
require "math"

game = {}

function horizontalCollision(a,b)
    if(a.x+a.width > b.x and a.x+a.width < b.x+b.width) then
	    return true
	end
	if(a.x < b.x+b.width and a.x > b.x) then
	    return true
	end
	if(a.x == b.x or a.x+a.width == b.x+b.width) then
	    return true
	end
end

function verticalCollision(a,b)
    if(a.y < b.y+b.height and a.y > b.y) then
	    return true
	end
	if(a.y+a.height > b.y and a.y+a.height < b.y + b.height) then
	    return true
	end
	if(a.y == b.y or a.y+a.height == b.y+b.height) then
	    return true
	end
end

function collisionDisplacement(a,b)
	local xdisp = 0
	local ydisp = 0
	if(a.x+a.width > b.x and a.x+a.width < b.x+b.width) then
		xdisp = b.x - (a.x+a.width)
	end
	if(a.x < b.x+b.width and a.x > b.x) then
	    xdisp = (b.x+b.width) - a.x
	end
	if(a.x == b.x and a.x+a.width == b.x+b.width) then
		xdisp = (b.x+b.width) - a.x
	end
	if(a.y < b.y+b.height and a.y > b.y) then
	    ydisp = (b.y+b.height) - a.y
	end
	if(a.y+a.height > b.y and a.y+a.height < b.y + b.height) then
	    ydisp = b.y - (a.y+a.height)
	end
	if(a.y == b.y and a.y+a.height == b.y+b.height) then
		ydisp = (b.y+b.height) - a.y
	end

	wex.console.print("collision displacement is" .. xdisp .. "," .. ydisp .. "\n")

	if(math.abs(ydisp) < math.abs(xdisp)) then
	    a.yvel = 0
		return 0,ydisp
	elseif (math.abs(xdisp) < math.abs(ydisp)) then
	    a.xvel = 0
	    return xdisp,0
	else
	    return 0,0
	end
end

function colliding(a,b)
	if(horizontalCollision(a,b) and verticalCollision(a,b)) then
	   wex.console.print("collision!!\n")
	   return true
	end
end

function game.handleEvents()
end

function game.update()
	mousex,mousey = wex.input.getMousePosition()
    player:update()

	for k,v in pairs(walls) do
        if(colliding(player,v) == true) then
		    player:move(collisionDisplacement(player,v))
		end
	end
end


function game.draw()
    wex.graphics.drawQuad(player.quad,player.x,player.y)
	for k,v in pairs(walls) do
	    wex.graphics.drawQuad(v.quad,v.x,v.y)
	end
end

function game.mouseDown(x,y,button)
end
