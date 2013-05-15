require "math"
editor = {}

cursor = Quad.new()
cursor:setWidth(32)
cursor:setHeight(32)
cursor:setColor(1,1,1,1)


function editor.update()
	mousex,mousey = wex.input.getMousePosition()
    cursor:setColor(math.sin(wex.time),-math.sin(wex.time),math.sin(-wex.time),1)
end


function editor.draw()
    wex.graphics.drawQuad(cursor,mousex,mousey,1,1,wex.time*25.4)
end
