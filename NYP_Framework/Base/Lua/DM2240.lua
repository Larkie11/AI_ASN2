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
music = {Menu = "Music//Menu.mp3", BGM = "Music//Background.mp3"}
width = 800
height = 600
position = {100,0,130}
playerstats = {health = 1000, score = 0, weaponType = 1}
waves = {wave1 = 25,wave2 = 45, wave3 = 65, wave4 = 80, survive = 100}
Reload = "S";
UP = "Z";
LEFT = "X";
RIGHT = "C";
DOWN = "V";
WP1 = {x = 300, y = 0, z = 300}
WP2 = {x = 250, y = 0, z = 250}
WP3 = {x = -200, y = 0, z = 200}
WP4 = {x = 100, y = 0, z = -250}
enemywave = {no = 3, a = 2, b = 3, c = 4, d = 1}
robotse = "Music//Robot.mp3"
robotdie = "Music//RobotDie.mp3"
pictures = {splash = "Image//Splash.tga",menupic = "Image//Menu.tga", menulogo = "Image//Logo.tga", menuarrow = "Image//Arrow.tga", text = "Image//calibri.tga", lose = "Image//Lose.tga", win = "Image//Win.tga",
instructpic = "Image//InstructionsPage.tga", instructlogo = "Image//Instructions.tga", game = "Image//InGame.tga", settings = "Image//Settings.tga", settingslogo = "Image//SettingsLogo.tga"}
texts = {introtext = "Press <SPACE>",menutext1 = "PLAY", menutext2 = "INSTRUCTIONS", menutext3 = "SETTINGS", menutext4 = "HIGHSCORE", menutext5 = "QUIT", loading = "LOADING GAME", instruction1 = "MOVEMENT KEYS", instruction2 = "Left key: ", instruction3 = "Right key: ", instruction4 = "Up key: ", instruction5 = "Down key: ", losetext = "YOU LOST", wintext = "YOU SURVIVED"}
shotguninfo = {maground = 10,maxmaground = 10, totalround = 70, maxtotalround = 70, timebetween = 0.5, se = "Music//Mining.mp3"}
pistolinfo = {maground = 8, maxmaground = 8, totalround = 40, maxtotalround = 40, timebetween = 0.2, se = "Music//Mining.mp3"}