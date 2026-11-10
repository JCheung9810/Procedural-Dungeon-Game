# Procedural-Dungeon-Game
A 1 player dungeon game where each playthrough features a uniquely generated dungeon. The player will be able to explore the dungeon, fight AI-controlled enemies, and progress through increasingly challenging areas.

# Implemented Features
- Random dungeon generation
  - Dead end generation
  - Room types and grid based generation
  - Boss room selection and generation
- Wall collision detection
- Room to room camera control
- 8 direction movement with animation
- Health
- Damage
- Enemy dummy
- Simple enemy ai
- Dodge/roll animation and functionality
- Death
- Different weapon types and functionality
- UI shows current weapon, magSize, bullets left, reload on counter
- LDTK level loading

# To Do
- COMBAT
  - Make enemies use weapon objects (make firing use those stats) 
  - Draw enemy weapons 
  - Better projectile hitbox 
  - Advanced enemy ai 
- UI
  - Reload mouse cursor 
  - Minimap/map 
  - 3D Wall overlap
- MECHANICS
  - Room content + Enemy generation 
  - Pickups (ammo,health) 
  - Room trigger 
  - Enemy spawn 
  - Doors close until cleared 
  - Breakable room objects
  - Usable/Interactable room objects
- OTHER
  - Lobby/Main menu 
  - Shop 

# Priority List (Highest to Lowest)
  - Change static arrays into vectors (SpriteInfo, FloorSpawnPool, Room)
  - Room content + enemy generation
  - Room trigger
  - Enemy spawn
  - Doors close until cleared
  - Advanced enemy ai
  - 3D Wall overlap
  - Make enemies use weapon objects (make firing use those stats)
  - Draw enemy weapons
  - Better projectile hitbox
  - Pickups (ammo,health)
  - Reload mouse cursor
  - Minimap/map
  - Shop
  - Lobby/main menu
  - Breakable room objects
  - Usable/Interactable room objects
  
# Distant Features
- Free placed rooms instead of grid
- Different sized rooms