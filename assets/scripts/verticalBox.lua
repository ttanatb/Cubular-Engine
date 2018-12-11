
-- vars
moveLeft = true
edge = 2.5
increment = 0.05
lives = 5

-- start function
function start ()

	--setting basic vars
	entity:setName("Basic Tank")
	entity:setMaterial("tankMat")
	entity:setMesh("tankMesh")
	
	--randomness for each box
	delta = math.random() * 3.0
	if math.random() < 0.5 then
		delta = -delta
	end
	entity:setPosition(entity:getPositionX() + delta, entity:getRotationY() + delta, entity:getPositionZ())
	entity:setScale(0.3 * math.random(1,2), 0.05 * math.random(1,5), 0.1 * math.random(1,4))
	entity:setRotation(delta, delta, delta)
	entity:setAlbedo( math.random() * 0.8 + 0.2, 0, math.random()* 0.8 + 0.2 )
end

-- update function
function update ()	

	--get position
	posX = entity:getPositionX()
	posY = entity:getPositionY()
	
	-- move the position
	if moveLeft then
		posY = posY + increment
		if posY > edge then
			moveLeft = false
			increment = increment / 3
			lives = lives - 1
		end

	else 
		posY = posY - increment
		if posY < -edge then
			moveLeft = true
			increment = increment * 3
		end
		
		if lives == 0 then
			entity:destroy()
		end
	end
	
	-- make it fancy
	posX = math.cos(posY) 
	entity:setPosition(posX, posY, entity:getPositionZ())
	entity:setRotation(posX, posY, math.sin(posX))
end
