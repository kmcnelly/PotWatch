
//when html loads
document.addEventListener("DOMContentLoaded", function (){

  setObserverFunc(updateUI);
  loadLogin();
  setup();
  // Controls Page =============================================================================

  //listen for door button
  document.getElementById("changeDoor").addEventListener("click",changeDoorStatus);

  //listen for light button
  document.getElementById("changeLight").addEventListener("click",changeLight);

  // Advanced Features Page ====================================================================

  //listen for autoClose time set button
  document.getElementById("autoDoorCloseTimeSet").addEventListener("click", setAutoCloseTime);
  //listen for autoClose ON button
  document.getElementById("autoDoorCloseOn").addEventListener("click",changeAutoClose);
  //listen for autoClose OFF button
  document.getElementById("autoDoorCloseOff").addEventListener("click",changeAutoClose);

  //listen for brightness
  document.getElementById("lightSlider").addEventListener("change",setBrightness);

  //listen for autoOff time set button
  document.getElementById("autoOffLightTimeSet").addEventListener("click", setAutoOffTime);
});

//updates the on-screen elements based on the model
function updateUI(updatedGarageObject){
  // Controls Page =============================================================================

  // Current Door state
  document.getElementById("door-status").innerText = "Door " + updatedGarageObject.door.getState() + ".";

  // ADD BUTTON SELECTED WHEN THE GARAGE DOOR IS OPEN OR LIGHT IS ON
  if (updatedGarageObject.door.getState() === "open") {
    document.getElementById("changeDoor").classList.add("button-selected");
  } else {
    document.getElementById("changeDoor").classList.remove("button-selected");
  }

  if (updatedGarageObject.light.getState()) {
    document.getElementById("changeLight").classList.add("button-selected");
  } else {
    document.getElementById("changeLight").classList.remove("button-selected");
  }

  // Advanced Features Page ====================================================================

  // fill auto close time text box ONLY IF EMPTY
  if(document.getElementById("autoCloseTime").value == "") {
    document.getElementById("autoCloseTime").value = updatedGarageObject.door.getAutoCloseTime();
  }

  // Auto Close Buttons
  if (updatedGarageObject.door.getAutoCloseEnabled()) {
    document.getElementById("autoDoorCloseOn").classList.add("button-selected");
    document.getElementById("autoDoorCloseOff").classList.remove("button-selected");
  } else if (!updatedGarageObject.door.getAutoCloseEnabled()) {
    document.getElementById("autoDoorCloseOn").classList.remove("button-selected");
    document.getElementById("autoDoorCloseOff").classList.add("button-selected");
  }

  //updates lightbrightness displayed value
  document.getElementById("lightBrightness").innerText = updatedGarageObject.light.getBrightness() + "% Brightness.";

  // fill auto off time text box ONLY IF EMPTY
  if(document.getElementById("autoOffTime").value == ""){
    document.getElementById("autoOffTime").value = updatedGarageObject.light.getAutoOffTime();
  }
}

function loadControls () {
  document.getElementById("accountcreation").hidden = true;
  document.getElementById("controls").hidden = false;
  document.getElementById("advancedfeatures").hidden = true;
  document.getElementById("login").hidden = true;
}

function loadAdvancedFeatures () {
  document.getElementById("accountcreation").hidden = true;
  document.getElementById("controls").hidden = true;
  document.getElementById("advancedfeatures").hidden = false;
  document.getElementById("login").hidden = true;
}
function loadLogin () {
  document.getElementById("accountcreation").hidden = true;
  document.getElementById("controls").hidden = true;
  document.getElementById("advancedfeatures").hidden = true;
  document.getElementById("login").hidden = false;
}
function loadAccountCreation () {
  document.getElementById("accountcreation").hidden = false;
  document.getElementById("controls").hidden = true;
  document.getElementById("advancedfeatures").hidden = true;
  document.getElementById("login").hidden = true;
}

// open/close door
function changeDoorStatus(){
  garage.door.changeState();
}

// turn on/off light
function changeLight(){
  garage.light.changeState();
}

function changeAutoClose(){
  if(this.value == "On"){
    garage.door.changeAutoClose("1");
  } else {
    garage.door.changeAutoClose("0");
  }
}

function setAutoCloseTime(){
  garage.door.changeAutoCloseTime(document.getElementById("autoCloseTime").value);
}

function setAutoOffTime(){
  garage.light.setAutoOffTime(document.getElementById("autoOffTime").value);
}

//updates brightness value of light
function setBrightness(){
  garage.light.setBrightness(document.getElementById("lightSlider").value);
}
