# NeoPixel-Playtime
Just a boy who wants to play with LED's

Just a bit tired of the dependency/scripting madness of web development. And I don't even do web development.

What?  
AdaFruit NeoPixel 60 LED/m  
*  3.5A @ 5V full-tilt
*  I only have a 1V @ 5V power supply. (But I made it so I can probably bump it up with some short work. A 2A regulator would be supah easy. Power is readily available, heat has nowhere to go though.)

Using Arduino Uno R3 to drive  
Just playing around with code. I'm undecided if I'm going to do multiple sketches or just revision one.

Maybe I should try a fun tach w/ 1 LED = 100 RPM. All LED's would give me a 6000 RPM range... I can't imagine the Jeep getting higher than that. I'd feel bad for it if it did. Poor thing.

## Fun Fact
The adafruit/Adafruit_NeoPixel lib currently has a Build | Failed tag on their git... Just a cheeky compile with the lads.


## On the Tach Beginnings FadeLoop Dealio.
So. Here's something fun. The NeoPixel library does not support (other than .setPixelColor() ) setting individual brightness in an algorithmic fashion.  
This means that I've been writing my own dimming algorithm based on Mask+Shift bit codes (which is somewhat lovely since dividing by two is just a bit shift to the right. Magic.). However... order of the bits varies between LED strips and even iterations of strips. Therefore the lib said "You know what? These people should just be able to assume it's RGB to make the code more portable w/ less hassle." Brilliant. Unless you're doing bit-level stuff and the lib doesn't account for that (it takes each argument separately or generates the color code from separate inputs). So my strip, unfortunately, is GRB. Which is why blue is a beautiful, strong, independent color who don't need no bug-finding and the others are coke addicts.  
Strangely enough though, when 'dimming' red or green, the dimmed pixel turns off. Working as intended! Dims to the point of turning off. This is more helpful than it seems though because all 0's are easy to catch but my eyes cannot tell the difference between 60 and 100 brightness.  
So from here I need to look at the .show() function to see how it unpacks and maybe write a layer in between. I'd rather not modify the lib.

I should also try setting my strip to RGB in the constructor and see if I can get away with it. TBD  
^ Yeah, that didn't work. As one might expect ^

Still, now I think I know what to do. Eschew the setPixelColor and just use the same technique as the method itself did. Now I have to handle a pointer to the pixel array but that's not a problem.

The problem was actually the brightness attribute in the strip. The setPixelColor() method actually multiplied the rgb value by the brightness, which I don't touch.  
That means that every time I would adjust the rgb values, setPixelColor() would do whatever the fuck it felt like when I sent the rgb code to it.
