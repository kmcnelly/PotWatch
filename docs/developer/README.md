
Welcome to CSE222! My project is a terrible example and I hope you can find a better one :P

In the case you are stuck with my project:
SetUp:
The hardware setup can be found in the circuit Diagram
The software setup will require you to modify the access token and deviceId for your specific Photon. You will also need to register and and connect your own IFTTT Messenger.
Other than that you should be good!

The ReadMe may provide a good guide for where things generally are. But in the WatchController.ino, instance variable will generally be at the top and organized into clumps such as state, light, target, etc.

The code is a very similar format to the garage assignment. Using your experience of those assignments and the sequnence diagram may be the best means of understanding the flow of my code.

For potential modifications, there is a lot of room on the input and output sides of the project.
  Coding a more knowledgeable understanding of cooking in which the user does not have to enter so much may be helpful.
  Also, when the timer/temp tracker is complete, there is a lot of room for response other than a notification and and a light.

To modify something more internal like adding a state, you will need to follow a process similar to this:
  -add the instance state to the enum
  -consider if there is any unique parameters for the state that must be taken into account
    -add the parameter checks to the isReady method which checks for when a state is complete.
  -Add state to all switch methods for displaying state in both the js and the .ino

To add an input/ sensor
  -add the instance of the input
  -if it is controllable/modify by the web-based interface
    -create the html in WatchApp.html as well ass corresponding JavaScript
  -connect the input data to WatchController by publishing over particle cloud using cloud functions
  -create helper methods for the function which assign instances with the new values as well as do small status checks
  -incorporate if statements of the new input data into the isReady() method to specialize the updates of states even more.

To add an output using IFTTT *assuming you have finished successfully setup the project
-add new IFTTT plug in and create new published event accordingly

–Sequence Diagram Code––––––––––––––––––––
title PotWatch Sequence Diagram

participant UI
participant Particle.io
participant Photon
participant IFTTT
participant Messenger


UI->Particle.io:targetTemp()
Particle.io->Photon: targetTemp()

UI->Particle.io:targetTime()
Particle.io->Photon: targetTime()

Photon->Particle.io: timeRemain()
Particle.io->UI: timeRemain()

Photon->Particle.io: curTemp()
Particle.io->UI: curTemp()

Photon->Particle.io: status()
Particle.io->UI: status()

Photon->Particle.io: doneAlert()
Particle.io->UI: doneAlert()

Photon->IFTTT: doneAlert()
IFTTT->Messenger: doneAlert()
––––––––––––––––––––––––––––––––––––––––––––
