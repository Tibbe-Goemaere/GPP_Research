# GPP_Research : Npc awareness in a stealth game

# Why?

We want to make the AI in stealth games feel believable, if you want to distract a guard with a coin or making a sound you don't want all guards to come investigate it. This would make it way harder for the player to move unnoticed and it would make no sense, why would all guards come investigate some small distraction?

# Interests Sources

In a stealth game there are multiple types of interests. If you take assassins creed for example, in the game you can throw coins or whistle at guards. Both of these actions will distract the AI and make them go investigate is. These are interest sources. 
Before continuinig with interest sources we need to talk about senses, all interest sources have senses. The most common senses and senses we are going to use are sight and sound. An AI will go investigate the interest source when they can.

For sight we gave our AI a simple vision cone and when a target is inside the vision cone, the Ai will go investigate is.
Target is represented by the black circle with a cross in it.
![afbeelding](https://user-images.githubusercontent.com/114002199/212195536-459b6aa3-d9fd-442d-8348-887182e567c1.png)

For sound we gave our sounds a radius which represents how far the sound goes, if our player is inside this radius he hears this sounds and investigates it.
Sound is represented by the red circle.
![afbeelding](https://user-images.githubusercontent.com/114002199/212198772-453c9355-6ae5-49af-a971-7f2d1c19a5fb.png)

But it's not that easy, there are obstacles in our way which block sounds and vision. 
![afbeelding](https://user-images.githubusercontent.com/114002199/212176083-19ff1788-3e6a-4c76-bd90-390425d25715.png)
