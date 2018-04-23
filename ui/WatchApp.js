
//when html loads
document.addEventListener("DOMContentLoaded", function (){

  // setObserverFunc(updateUI);
  loadCover();
  setup();
  // Controls Page =============================================================================

  //listen for timer
  document.getElementById("timerSlider").addEventListener("change",changeTimer);

  //listen for temp
  document.getElementById("tempSlider").addEventListener("change",changeTemp);

});

// updates the on-screen elements based on the model
function updateUI(updatedWatchObject){
  // Controls Page =============================================================================
  if (updatedWatchObject.getState() === undefined) {
    // Hey dont do that
  } else {
    document.getElementById("temp-status").innerText = updatedWatchObject.getTemp();
  }

  // Current time state
  // document.getElementById("timer-status").innerText = "Time " + /*updatedWatchObject.timer.getMin() */+ ".";
}

function loadControls () {
  document.getElementById("controls").hidden = false;
  document.getElementById("cover").hidden = true;
}

function loadCover () {
  document.getElementById("controls").hidden = true;
  document.getElementById("cover").hidden = false;
}

//updates temp displayed value
function changeTemp(){
  document.getElementById("Temp").innerText = document.getElementById("tempSlider").value;
}

//updates timer displayed value
function changeTimer(){
  document.getElementById("Timer").innerText = document.getElementById("timerSlider").value;
}

//update the target temperature
function setTemp(){
  //watch.temp.setTarget(document.getElementById("tempSlider").value);
}

//update the target cook time
function setTimer(){
  //watch.timer.setTarget(document.getElementById("timerSlider").value);
}
