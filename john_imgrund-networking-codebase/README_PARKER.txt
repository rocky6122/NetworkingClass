added the display of the backlog

put the constructor and Destructors in for the SendMessageRequest, hopefully that helps.
(I havent implemented them anywhere, just put them in as a reminder)

Alex was talking about the Async giving out shit variables for special characters.
So I made a special case if statement in handleInput (isTyping) to convert it into the correct /

I added an if else to our gs->isTyping because in its current form it checks if we hit backspace, but then continues
into our 'add letter' section, this causes it to actually place a 'backspace' into the character array. Could possibly explain our x1 problem

THATS WHY OUR FUCKING x1 COMES UP! We arent checking to exclude things like enter or backspace inside the key check for letters.
So we press enter to start and it puts and x1 because enter doesnt have a real value. then when we go to send it checks isTyping first
then its pops an enter into the array, again making it an x1. THEN IT SENDS

Found this keycode site that maps the values of asyncgetkeystate http://cherrytree.at/misc/vk.htm 

Fixed the leaving system. Users can now propperly leave the servers by pressing esc and can leave the app by pressing q from the lobby.

backspace was written incorrectly from the start. It now works as intended.

Im getting a weird problem where when u hit enter to start u get a bunch of 'garbage' displaying by default.
but if u type a letter then hit delete it goes away, but then u type another letter and it comes back.

It might have to do with the for loop again inside of the 
