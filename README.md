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
- Room enemy encounter trigger

# To Do Priority List (Highest to Lowest)
  - Make enemy shots to be more consistent (no spread for single, shotgun uses spread and bullets are spread even)
  - Implement enemies getting knockback, build in knockback stat
  - Enemy sprites, animations (idle, walking, stunned)
  - Offset shooting start times
  - Pickups (ammo,health)
  - Add shields (temp health, always takes a full 1 hit no matter the damage)
  - Make a boss entrance
  - Make floor exit
  - Minimap/map
  - Item inventory
  - Shop
  - Lobby/main menu
  - Black layer, change opacity over time when passed through door, 
  - Make door initially closed until walked through, door is not gate, gate locks player in, door is for exploration
  - Remove camera clamp, make camera positioned at some point between player and mouse
  - Reload mouse cursor
  - Reload bar above head
  - Breakable room objects
  - Usable/Interactable room objects
  - Door animation
  
# Distant Features
- Better projectile hitbox (for guns that dont shoot circular projectiles)
- Advanced enemy AI (path finding)
- Free placed rooms instead of grid
- Different sized rooms

# Notes to Self