require "class"

Actor = class()

function Actor:__init(name)
   self.name = name
   self.quad = Quad.new()
   self.quad:setHeight(64)
   self.quad:setWidth(64)
   self.quad:setColor(1,0,0,1)
   self.xvel = 0
   self.yvel = 0
   self.speed = 10
   self.x = 200
   self.y = 200
   self.height = 64
   self.width = 64
   self.dvx = 0
   self.dvy = 0
   self.maxv = 20
end

function Actor:move(x,y)
	self.x = self.x + x
	self.y = self.y + y
end

function Actor:accelUp()
	self.dvy = -self.speed * wex.delta
end

function Actor:accelDown()
	self.dvy = self.speed * wex.delta
end

function Actor:accelLeft()
    self.dvx = -self.speed * wex.delta
end

function Actor:accelRight()
    self.dvx = self.speed * wex.delta
end

function Actor:update()

    if(wex.input.keyPressed(key.down) == true or wex.input.keyHeld(key.down) == true) then
        player:accelDown()
	end

	if(wex.input.keyPressed(key.up) == true or wex.input.keyHeld(key.up) == true) then
        player:accelUp()
	end

	if(wex.input.keyPressed(key.right) == true or wex.input.keyHeld(key.right) == true) then
        player:accelRight()
	end
	if(wex.input.keyPressed(key.left) == true or wex.input.keyHeld(key.left) == true) then
        player:accelLeft()
	end

    if(self.yvel > self.maxv) then
	    self.yvel = self.maxv
		self.dvy = 0
	end
	if(self.yvel < -self.maxv) then
	    self.yvel = -self.maxv
		self.dvy = 0
	end
	if(self.xvel > self.maxv) then
	    self.xvel = self.maxv
		self.dvx = 0
	end
	if(self.xvel < -self.maxv) then
	    self.xvel = -self.maxv
		self.dvx = 0
	end

	self.xvel = self.xvel + self.dvx
	self.yvel = self.yvel + self.dvy
    self.x = self.x + self.xvel
	self.y = self.y + self.yvel
	self.yvel = self.yvel * .95
	self.xvel = self.xvel * .95
	self.dvy = 0

	self.dvx = 0



end

