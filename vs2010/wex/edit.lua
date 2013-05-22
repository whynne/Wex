require "math"
require "class"
require "table"
require "actor"
require "io"

editor = {}

Wall = class()

function Wall:__init()
   self.type = "wall"
   self.quad = Quad.new()
   self.quad:setHeight(64)
   self.quad:setWidth(64)
   self.quad:setColor(0,0,0,1)
   self.x = 50
   self.y = 50
   self.height = 64
   self.width = 64
end

function Wall:copy()
   local nwl = Wall()
   nwl.quad:setWidth(self.quad:getWidth())
   nwl.quad:setHeight(self.quad:getHeight())
   nwl.x = self.x
   nwl.y = self.y
   return nwl
end


walls    = {}
actors   = {}
triggers = {}


block = Wall()

toolbox = {block}


currenttool = 1
mode = "place"
selected = 0

function editor.handleEvents()
    if(type(selected)==type(Wall)) then
	    if(wex.input.keyPressed(key.left) == true) then
		    selected.x = selected.x - 1
	    end
	    if(wex.input.keyPressed(key.right) == true) then
		    selected.x = selected.x + 1
	    end
		if(wex.input.keyPressed(key.up) == true) then
		    selected.y = selected.y - 1
	    end
	    if(wex.input.keyPressed(key.down) == true) then
		    selected.y = selected.y + 1
	    end
	end

    if(type(selected)==type(Wall)) then
	    if(wex.input.keyPressed(key.left) == true) then
		    selected.x = selected.x - 1
	    end
	    if(wex.input.keyPressed(key.right) == true) then
		    selected.x = selected.x + 1
	    end
		if(wex.input.keyPressed(key.up) == true) then
		    selected.y = selected.y - 1
	    end
	    if(wex.input.keyPressed(key.down) == true) then
		    selected.y = selected.y + 1
	    end
	end

	if(wex.input.keyPressed(key.insert) == true) then
	    if(mode == "place") then
		    wex.console.print("switching to select\n")
		    mode = "select"
		else
		    wex.console.print("mode change to place\n")
		    mode = "place"
		end
	end

end

function editor.update()


end

function editor.draw()
    if(mode == "place") then
	    wex.graphics.drawQuad(toolbox[currenttool].quad,wex.input.getMousePosition())
	else

	end

	for k,v in pairs(walls) do
	    wex.graphics.drawQuad(v.quad,v.x,v.y)
	end


end

function editor.mouseDown(x,y,button)
    if(mode == "place") then
		wex.console.print("Placing wall\n")
	    if(toolbox[currenttool].type == "wall") then
		    local newwall = toolbox[currenttool]:copy()
			newwall.x = x
			newwall.y = y
			table.insert(walls,newwall)
		end
	end

end

