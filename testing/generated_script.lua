
package.cpath = package.cpath .. ';dist/?.so;../dist/?.so'
local homm3lua = require('homm3lua')local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_SMALL)
instance:name('"2P Duel Template"')
instance:description('"A balanced map for 2 players with a contested central area."')
instance:difficulty(homm3lua.DIFFICULTY_EASY)

instance:terrain(homm3lua.TERRAIN_GRASS)
instance:player(homm3lua.PLAYER_1)
instance:town(homm3lua.TOWN_STRONGHOLD, {x=5, y=5, z=0}, homm3lua.PLAYER_1, true)
instance:hero(homm3lua.HERO_CHRISTIAN, {x=5, y=6, z=0}, homm3lua.PLAYER_1)

instance:player(homm3lua.PLAYER_2)
instance:town(homm3lua.TOWN_INFERNO, {x=10, y=10, z=0}, homm3lua.PLAYER_2, true)
instance:hero(homm3lua.HERO_JENOVA, {x=10, y=11, z=0}, homm3lua.PLAYER_2)

instance:player(homm3lua.PLAYER_3)
instance:town(homm3lua.TOWN_NECROPOLIS, {x=15, y=15, z=0}, homm3lua.PLAYER_3, true)
instance:hero(homm3lua.HERO_FAFNER, {x=15, y=16, z=0}, homm3lua.PLAYER_3)

-- Dynamic terrain adjustments for linear paths
instance:terrain(function (x, y, z)
    -- Hub point
    -- Hub coordinates: (20, 8)
    if x == 5 and y == 5 then return nil, 1 end
    if x == 6 and y == 5 then return nil, 1 end
    if x == 7 and y == 5 then return nil, 1 end
    if x == 8 and y == 5 then return nil, 1 end
    if x == 9 and y == 5 then return nil, 1 end
    if x == 10 and y == 5 then return nil, 1 end
    if x == 11 and y == 5 then return nil, 1 end
    if x == 12 and y == 5 then return nil, 1 end
    if x == 13 and y == 5 then return nil, 1 end
    if x == 14 and y == 5 then return nil, 1 end
    if x == 15 and y == 5 then return nil, 1 end
    if x == 16 and y == 5 then return nil, 1 end
    if x == 17 and y == 5 then return nil, 1 end
    if x == 18 and y == 5 then return nil, 1 end
    if x == 19 and y == 5 then return nil, 1 end
    if x == 20 and y == 5 then return nil, 1 end
    if x == 20 and y == 6 then return nil, 1 end
    if x == 20 and y == 7 then return nil, 1 end
    if x == 10 and y == 10 then return nil, 1 end
    if x == 11 and y == 10 then return nil, 1 end
    if x == 12 and y == 10 then return nil, 1 end
    if x == 13 and y == 10 then return nil, 1 end
    if x == 14 and y == 10 then return nil, 1 end
    if x == 15 and y == 10 then return nil, 1 end
    if x == 16 and y == 10 then return nil, 1 end
    if x == 17 and y == 10 then return nil, 1 end
    if x == 18 and y == 10 then return nil, 1 end
    if x == 19 and y == 10 then return nil, 1 end
    if x == 20 and y == 10 then return nil, 1 end
    if x == 20 and y == 9 then return nil, 1 end
    if x == 15 and y == 15 then return nil, 1 end
    if x == 16 and y == 15 then return nil, 1 end
    if x == 17 and y == 15 then return nil, 1 end
    if x == 18 and y == 15 then return nil, 1 end
    if x == 19 and y == 15 then return nil, 1 end
    if x == 20 and y == 15 then return nil, 1 end
    if x == 20 and y == 14 then return nil, 1 end
    if x == 20 and y == 13 then return nil, 1 end
    if x == 20 and y == 12 then return nil, 1 end
    if x == 20 and y == 11 then return nil, 1 end
    if x == 20 and y == 10 then return nil, 1 end
    if x == 20 and y == 9 then return nil, 1 end
    if x == 20 and y == 8 then return nil, 1 end
    return nil
end)
instance:write('/home/gk/.local/share/vcmi/Maps/test.h3m')
