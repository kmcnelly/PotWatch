
Welcome to CSE222! My project is a terrible example and I hope you can find a better one :P

In the case you are stuck with my project:
SetUp:
The hardware setup can be found in the circuit Diagram
The software setup will require you to modify the access token and deviceId for your specific Photon. You will also need to register and and connect your own IFTTT Messenger.
Other than that you should be good.

What would they need to know to setup your project and get it running?
What would they need to know to modify it? (Assume that they don’t want to read through all your code - they want some sort of a quick start guide that will help them identify where they should start looking/working first)
Assume that the feature will be assigned to them. You can’t make any assumptions about how they may need to modify your work, so you should give a general overview of all aspects of it with sufficient detail for them to know where to look to make modifications.
Examples of modifications could include
Changing timing of some hardware feature
Adding a new sensor or output to the device that needs to be shown or controlled from the UI
Adding a new “page” to the UI.
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
