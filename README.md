# GPP_Research : Npc awareness in a stealth game

# Why?

We want to make the AI in stealth games feel believable, if you want to distract a guard with a coin or making a sound you don't want all guards to come investigate it. This would make it way harder for the player to move unnoticed and it would make no sense, why would all guards come investigate some small distraction?

# Interests Sources

In a stealth game there are multiple types of interests. If you take assassins creed for example, in the game you can throw coins or whistle at guards. Both of these actions will distract the AI and make them go investigate is. These are interest sources. 
Before continuinig with interest sources we need to talk about senses, all interest sources have senses. The most common senses and senses we are going to use are sight and sound. An AI will go investigate the interest source when they can.

![afbeelding](https://user-images.githubusercontent.com/114002199/212176083-19ff1788-3e6a-4c76-bd90-390425d25715.png)

For sight we gave our AI a simple vision cone and when a target is inside the vision cone, the Ai will go investigate is.

Target is represented by the black circle with a cross in it.

![afbeelding](https://user-images.githubusercontent.com/114002199/212195536-459b6aa3-d9fd-442d-8348-887182e567c1.png)

For sound we gave our sounds a radius which represents how far the sound goes, if our player is inside this radius he hears this sounds and investigates it.

Sound is represented by the red circle.

![afbeelding](https://user-images.githubusercontent.com/114002199/212198772-453c9355-6ae5-49af-a971-7f2d1c19a5fb.png)

But it's not that easy, there are obstacles in our way which block sounds and vision, with our level and all these obstacles we made a Navigation mesh*, on this mesh we can call A-star pathfinding* to find the path between our npc and the source of the interest. 
*For more information:
NavMesh : 
https://en.wikipedia.org/wiki/Navigation_mesh
A-Star:
https://en.wikipedia.org/wiki/A*_search_algorithm

Calling the A-star algorithm will give us a path from our npc to the origin location of the interest source. If this path is longer then the radius of our sounds, the sound will not get to the npc, we also use this to check if there is an obstacle between the npc and a target.

![afbeelding](https://user-images.githubusercontent.com/114002199/212200861-126b4c7a-d1ec-4195-8bca-7e35f0641c78.png)

We can easily create new interest sources, but to create them we need to define them with a couple of variables:
- A lifetime, sounds have a lifetime and this will determine how long the sounds stay
- An origin, the place where the interest starts
- An radius, this will determine whether the npc detects the interest or not

# Priority

The next thing we have to think about is priority, which interest source will the npc investigate next. It makes no sense for the AI to just ignore all new incoming interests, if you sea a broken vase on the ground and hear someone screaming a bit further you will stop researching the vase and go investigate the person. So in our interest sources we have a priority value of a simple integer. It is very simple, if an npc comes across a new interest source with a higher priority, the higher priority will be investigated. 

And what about equal priorities, some interests are equal or almost equally important. Hearing some quit noise or seeing a coin on the ground is about the same. So what do we investigate first? It makes more sense to keep on investigating the interest source you are investigating so that is exactly what we are going to do. 

# Project

In my project I made a simple representation of an npc patrolling around and investigating interest sources. You can use middle mouse button to change interest sources, there are sounds and a dead body(sight) with different priorities. With right mouse button you can move around the navigation mesh and with left mouse button you can place interest sources wherever you like. 

# Source 
The documentation where my project is based on:
https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiF37m-mMP8AhUr87sIHaQwAhkQFnoECAkQAQ&url=http%3A%2F%2Fwww.gameaipro.com%2FGameAIPro%2FGameAIPro_Chapter32_How_to_Catch_a_Ninja_NPC_Awareness_in_a_2D_Stealth_Platformer.pdf&usg=AOvVaw0PZOOHpm4YUkxvjl1T582w



