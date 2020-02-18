# CJ's Tips & Tricks for vision tracking

For the better part of 2 years i've been working closely with vision tracking and detections. This is partly because our teams have never really been that successful with vision tracking (bar 2016). This partly because some people don't realise how much work can sometimes go into tracking a target and forget that in some cases it requires a completely seperate programming team for it.

And when the time came to program it, we never had experienced people. Because even when we had people work on it one year. Coincidently the next year they were no longer students and went into the world never to be seen again. We came very close in 2019 using a tinkerboard and software jaci wrote to make it easier to deploy to them. (Which is what this library was closely based on) But at the last minute when we tried to add multi threading to increase performance... we uhh, completely destroyed the code. And we couldn't use it in comp.

THAT BEING SAID, i've gone through the pleasures of failures so you don't have to.

That's why every function in this lib is multi-threaded off the bat...

Anyway, below are some of the things that I learned when working with vision tracking.


## Camera's

- This is pretty easy right? Cameras are the fundamentals for vision tracking. But selecting a camera is a big part of the process. I mean it literally makes 1 3rd of the 3 stages of vision tracking (Aquiring->Processing->Acting).

So what camera should you choose?
This is a difficult question but it depends on what you need it for, personally our team is with the motto to just take and use whatever we have on hand. But for the logical person who thinks before they buy remember what your using it for.

Something with a high resolution is nice but constly in the processing side. It may sound nice to get all those pixels. But it doesn't help when the POV is not as high. And your processing for the sake of processing. 

#### Resolution
- If your tracking a target for say shooting. And your using that camera to search for a target on the field, a low resolution like 720p is perfectly fine. In most cases you want to lower that number to 640, it makes little difference when your only using two values. x & y in a plane.


#### POV
- Which takes me to my next point, wider lense is a good idea. Too large and it can conflict with lights and other things on the field. But 60 degree POV is usually a good middle ground for this. It gives you wide enough point of view to detect almost the whole field (8 meters) if your on the other side of the field. And if your have a 640p detection, it provides just the right amount of sharpness if your close to your target to do precise PID movements.


#### Camera quirks
- But make sure to know everything about your camera, it may have quirks that you don't know about. For instance by pure conincidence (Actually the reason i made this tip&tricks file) I have been testing our vision cameras in the same location for the last two build seasons. When I put it on the robot (in a different space) the camera had a feature to adjust to the rooms colour saturation... and it's way of doing this was moving the entire colour wheel 30 degrees positive. To let you know, in HSV detection most of the time the colour green in the hue range of 50-70. Hm, that's odd, it's exactly 30 degrees. You can guess what happened next.

It also brings up why some people use exposure detection rather than colour detection. But i'll bring that up later.

We were using the Microsoft lifecam 3000. Which is actually a common camera for FRC vision tracking. So I was surprised that it even had this feature. Annoyingly it's a microsoft setting. So you can't dissable it without using a microsoft computer. And most teams use linux. There is a way in linux which i am currently working on to implement into the library. But just be aware. It happens rarely but it can occure even on the field.

The quick solution to this is when booting up the robot/camera flood the camera with something it can't adjust too, we simply place a sliver of retro tape over the camera, and the ring light does the rest. Flooding the camera, making it impossible to adjust and it using it's deafult colour wheel. It's good practice to link the video to your driver station, when your setting the robot up on the field bring a sliver. It might not be an issue. But it could be a life saver, place it on the robot. Then when your getting the feed, make sure your detecting the piece of tape. 

## Detecting

#### Detection type
