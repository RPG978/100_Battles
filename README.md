# 100 Battles — A Momentum Driven, Choice Based RPG

## Overview

100 Battles is a single player, turn based RPG built in modern C++ where the player must survive a gauntlet of battles, strategically choosing their build, equipment, and risk level. The game blends momentum based combat, procedural character generation, and player driven difficulty scaling to create a data driven experience with high replay value. The player wins by completing 100 battles in a row, but how they approach that challenge is entirely up to them.
Core Gameplay Features (Shipping Day Goals)
1. Procedurally Generated Character Selection
At the start of each run, the player chooses from three randomly generated characters, each with a distinct stat identity: attack focused, defense focused, and elementally diverse. Each option is generated from the SQLite database, ensuring variety and high replay value. The player may rename their character before beginning the run.
2. Initial Equipment Draft
Before entering combat, the player is presented with 3 random weapons, 3 random armor pieces, and 3 random accessories. All are procedurally generated from the database. The player chooses one of each, forming their initial build; this creates a “draft phase” similar to roguelites and deck builders.
3. Player Chosen Battle Streaks
Before each combat segment, the player chooses how many battles they want to fight in a row. The reward structure scales exponentially: more battles = higher gold and XP, but also greater risk. There is no healing until the streak ends. This gives the player meaningful control over difficulty and pacing.
4. Store Phase Between Streaks
After completing the chosen number of battles, the player enters the Store State, where they can heal, buy items, sell items, purchase new equipment and prepare for the next streak. Once ready, the player chooses their next streak length and re enters the battle loop.
5. Victory Condition
The player wins the game by completing 100 battles in a row (with healing and shopping only between streaks). This creates a long form challenge with strategic planning, resource management, and risk assessment.

## Combat System

The combat engine is built around momentum states that reward or punish streaked actions:
### Agro
Triggered by 3 consecutive attacks. While agro, the next attack deals 200% damage and defense is halved for one turn.
### Tranquil
Triggered by 3 consecutive defends. While tranquil, their next turn is skipped. These states apply to both the player and enemies, creating a dynamic, symmetrical combat system.

## Data-Driven Architecture

All game content is stored in SQLite: Character stat templates, equipment pools, enemy definitions, scaling curves, shop inventory and procedural generation rules. This allows rapid iteration and designer friendly content updates.
### Gameplay Loop
1.	Choose from 3 procedurally generated characters
2.	Rename your character
3.	Draft initial equipment (weapon, armor, accessory)
4.	Choose how many battles to fight in a row
5.	Fight through the streak
6.	Enter the store (heal, buy, sell, upgrade)
7.	Repeat steps 4–6
8.	Complete 100 battles to win

## Roadmap

### Phase 1 — Core Systems (Complete / Near Complete)
• Combat engine 

• Agro/Tranquil momentum 

• Enemy AI 

• SQLite integration
### Phase 2 — Procedural Generation (In Progress)
• Character stat generation 

• Equipment draft generation 

• Scaling reward system
### Phase 3 — Store & Progression (Next)
• Healing system 

• Buy/sell mechanics 

• Equipment upgrades 

• Streak reward scaling
### Phase 4 — Final Game Loop
• Victory condition 

• Run summary 

• Difficulty tuning 

• Balancing
### Phase 5 — Presentation & Polish
• Combat log formatting 

• UI improvements 

• Documentation 

• Portfolio ready packaging

### Build Instructions
Visual Studio 2022

## Author
Chris Poole — C++ developer specializing in modular architecture, data driven systems, and gameplay programming.
