
package.cpath = package.cpath .. ';dist/?.so;../dist/?.so'
local homm3lua = require('homm3lua')
local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_SMALL)

instance:name('"2P Duel Template"')
instance:description('"A balanced map for 2 players with a contested central area."')
instance:difficulty(homm3lua.DIFFICULTY_HARD)

instance:player(homm3lua.PLAYER_1)
-- Zone 1
instance:town(homm3lua.TOWN_CASTLE, {x=5, y=5, z=0}, homm3lua.PLAYER_1, true)

instance:player(homm3lua.PLAYER_2)
-- Zone 2
instance:town(homm3lua.TOWN_INFERNO, {x=10, y=10, z=0}, homm3lua.PLAYER_2, true)

instance:player(homm3lua.PLAYER_3)
-- Zone 3
instance:town(homm3lua.TOWN_NECROPOLIS, {x=15, y=15, z=0}, homm3lua.PLAYER_3, true)

instance:write('/home/gk/.local/share/vcmi/Maps/test.h3m')