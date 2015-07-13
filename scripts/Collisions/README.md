Collision detection with the tracking data was not possible. We discussed this with Thomas 
at the start of the coding exercise and tried to do it anyway, however we gave up once we saw
our robots behaviour. For those of you who want to try anyway here is some code, mainly for
the detection of line segement-circle intersections. The reasons that collision detection with
tracking data is not possible are:
The tracking data and the model of the room are not precise enough. The room and robot can be 
modelled with line segements and circles, however if the room is not in excatly the same condition
for every experiment everything fails. Everything needs to be very precise to distinguish between
the robot stopping before a block and the robot bumping into it. Furthermore the robot (ours at least)
pushes some objects around while it cleans, making the static model of the room completely useless.
It is also not possible to detect a bump by the sudden deceleration of a robot. To make a proper
collision detection, image processing or other sensors are necessary. This, however, exceeds the scope of the task and 
the video material is too large and the access restricted. 