

# 1. Description
The PotWatch : a water temperature tracker and alert device.

The PotWatch is a device that will read the temperature of the water in a pot and alert a user with sound, an OLED screen, and push notifications of key temperature points as well as upcoming steps and instructions. The PotWatch is something that can easily be used frequently and implemented in standard weekly cooking operations. This IoT device will primarily be designed as a better assistant when cooking pasta. Currently, the market for pasta timers mainly consists of reasonably cheap and somewhat fun devices that are placed in the water and sing to notify the user. While fairly close in functionality to these pasta timers, the PotWatch device is intended to be a more versatile device with greater customization and accessibility through a mobile interface. The most similar product currently being sold is the iPot, a smart pot that links to mobile devices allowing them to control the pot.
iPot: https://instantpot.com/instant-pot-launches-the-worlds-first-bluetooth-smart-connected-smartcooker-at-ces-2014/
The iPot is fairly expensive, priced around $100 dollars and rigid in its design as the user is forced to cook only with the specific pot in order to reap the benefits of IoT. The PotWatch on the other hand is indented to complement a variety of pot and at a much lower cost.
Knowing there is already a market for similar devices, I am confident there is a market for the PotWatch. Although it is not a necessity item, the PotWatch is designed for simplicity and productivity. One of the most popular dinners, pasta, is now one IOT device away. The PotWatch works for a vast demographic, from those who may forgetful about time to those who just have better things to do. A hardworking bachelor/bachelorette may use the time to get some work done right before dinner or simply relax a bit, while an incompetent college grad may live off of it and have to worry about one less thing.
Moreover, the product is completely worth cost. Financially, the low price of about $35 is save over the time you spend not thinking about the cooking pot. As a pre-assembled kit, the installation procedure is as easy as hooking it up to your phone and starting to cook. And asides from batteries and a connection to the home wifi, the PotWatch is completely independent, requiring no maintenance and the only preparation is your own customization.

convenience – this product will allow its user to be more productive with there time. The cost it takes to maintain the device and purchase it will be much lower than the cost in time absorbed by waiting and watching the pot.
safety - if the user is very bad at cooking

# 2. Hardware and Cloud Infrastructure Needed

## Hardware:
Photon, Waterproof temperature sensor, OLED, Push button, Maybe Piezo Buzzer?, Maybe RGB LED

## Cloud Infrastructure:
Particle cloud, Applet push notifications

# 3. Unknowns and Challenges
While we have done studios on utilizing temperature sensors and Applet, I know I will  have challenges with them as I am still not confident with them.

# 4. User Stories & Usage Scenarios
As an avid pasta eater,I want a exact cooking process, so that my pasta is just the way I like it every time.

As a very stressed and busy worker, I want something to pay attention to the cooking process for me, so that I can use those 30 or so minutes productively.

As a very lazy student, who has lived off of BD and the village for four years, I want a very simple step-through guide/assistant helping me cook decent food, so that I don't burn my apartment down.

# 5. Paper Prototypes


# 6. Implementation: Sequence Diagrams
//Sequence Diagram Code=======================
title PotWatch Plan

participant UI/Phone
participant Particle.io
participant Photon


Note right of Particle.io:Smarter implementation of calculating alert time will come later

UI/Phone->Particle.io:targetTemp()
Particle.io->Photon: targetTemp()

UI/Phone->Particle.io:targetTime()
Particle.io->Photon: targetTime()

Photon->Particle.io: timeRemain()
Particle.io->UI/Phone: timeRemain()

Photon->Particle.io: curTemp()
Particle.io->UI/Phone: curTemp()

Photon->Particle.io: status()
Particle.io->UI/Phone: status()

Photon->Particle.io: doneAlert()
Particle.io->UI/Phone: doneAlert()
//==============================================
# 7. Plan and Schedule

## Weekly Schedule / Progress

| Week End     | Deliverables & Accomplishments          |
|:-------------|:--------------------------------------  |
| By April 7   | Complete Embedded:Temperature sensor... |
| By April 14  | Complete UI and JavaScript              |
| By April 21  | Complete cloud and Push notifications   |
| April 24th   |  Complete Project Due!                  |


## Times Reserved for Project Work

Fill in a schedule of times reserved for the project.  If you can't set regular weekly times, create a schedule based on specific days.

| Week Day | Times |
|:---------|:------|
| Monday   |       |   
| Tuesday  | 1hr   |    
| Wednesday| 1-2hrs|
| Thursday |       |
| Friday   | 2hrs  |
| Saturday | 3hrs  |
| Sunday   |       |
