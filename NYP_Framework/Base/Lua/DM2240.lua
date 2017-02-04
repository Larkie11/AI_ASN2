function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Lua/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Lua/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2 - z1)*(z2-z1)
	print (distanceSquare)
return distanceSquare
end

title = "DM2240 - Week 14 Scripting"
width = 800
height = 600
position = {100,0,130}
playerstats = {health = 100, score = 0, weaponType = 1}
waves = {wave1 = 20, wave2 = 50, wave3 = 80, wave4 = 130, survive = 150}
UP = "Z";
LEFT = "X";
RIGHT = "C";
DOWN = "V";