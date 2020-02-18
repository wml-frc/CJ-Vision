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

Which takes me to my next point, wider lense is a good idea. Too large can conflict with lights and other things on the field. But 60 degree POV is usually a good middle ground for this. It's also a good idea to remember those values.
