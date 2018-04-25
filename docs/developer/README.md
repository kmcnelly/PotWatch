

–Sequence Diagram Code––––––––––––––––––––
title PotWatch Plan

participant UI
participant Particle.io
participant Photon



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
––––––––––––––––––––––––––––––––––––––––––––
