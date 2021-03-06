var myParticleAccessToken = "f76cd4700e1489f30c1fdf0f54d702d63236800c";//find out when plug in
var myDeviceId =            "1f003c001947343438323536";
var topic =                 "cse222/thisWatch";

function newWatchEvent(objectContainingData) {
  console.log("Hi?");
  console.dir(objectContainingData);
  // TODO: Parse the incoming state and update any listeners
  // Hints:
  //    1. Use console.dir() to show the state object on the console.
  //    2. Consider JSON.parse, but be aware that proper JSON enclosed property
  //       names in DOUBLE quotes ("proper":value)
  var obj = JSON.parse(objectContainingData.data);
  console.dir(obj);
  console.log(obj.state);

  switch (obj.state)
  {
    case 0: // IDLE
    watch.state = "Idle";
    console.log("Idle");
    break;
    case 1: // PREP
    watch.state = "Prep";
    console.log("Prep");
    break;
    case 2: // COOKING
    watch.state = "Cooking";
    console.log("Cooking");
    break;
    case 3: // COOLING
    watch.state = "Cooling";
    console.log("Cooling");
    break;
  }

  //update enabled timer
  if(obj.timerEnabled == 1){
    watch.time.enabled = true;
  }
  else{
    watch.time.enabled = false;
  }

  //update enabled temp
  if(obj.tempEnabled == 1){
    watch.temp.enabled = true;
  }
  else{
    watch.temp.enabled = false;
  }

  //update remaining time
  watch.time.curTime = obj.remainingTime;

  //update temp
  watch.time.curTemp = obj.curTemp;

  updateWatch();
}
//AHHHHHHH MESSED UP BRACKETS
var watch = {

  // NOTE: A new object to access particle JavaScript functions
  particle: new Particle(),

  state: "Idle",

  getState: function() {
    return watch.state;
  },

  changeState: function(){
    if(watch.temp.enabled || watch.time.enabled){
      watch.buttonPush();
      watch.sendState(this.state);

      if(watch.temp.enabled && (watch.temp.curTemp > watch.temp.tempTarget)){
        alert('Not at target temperature yet');
      }

      console.log(watch.time.curTime);
      console.log(watch.time.timeTarget);
      if(watch.time.enabled && (watch.time.curTime > watch.time.timeTarget)){
        alert('Not at target time yet');
      }
    }
    else {
      alert("Enter one or both target values to continue.");
    }
  },


  buttonPush: function() {
      console.log("(fake) button pushed");
      callParticleFunction("trigButton", "test");
  },

  sendState: function(myState){
    this.state = myState;
    callParticleFunction("publishState", "" + this.state);
  },

  temp: {
    enabled: false,
    curTemp: 0,
    tempTarget: 0, // 30 deg

    getTemp: function(){
      return watch.time.curTemp;
    },

    //update TARGET temperature
    setTemp: function(nextTemp){
      watch.temp.tempTarget = nextTemp;
      callParticleFunction("changeTemp",""+ this.tempTarget);
      console.log("set Temp to " + watch.temp.tempTarget);
    }
  },

  time: {
    enabled: false,
    curTime: 0,
    timeTarget: 30, // 30 deg

    getTimeLeft: function(){
      return watch.time.curTime;
    },

    // //update CURRENT time
    // changeTime: function(newTime){
    //   watch.time.curTime = newTime;
    //   callParticleFunction("publishState","" + this.curTime)
    // },
    //
    //update TARGET time
    setTimer: function(nextTime){
      console.log("set Timer");
      watch.time.timeTarget = nextTime;
      callParticleFunction("changeTime","" + this.timeTarget)
    }
  },

  observerFunc: null //Observer
};

function callParticleFunction(functionName, functionArg) {
  var functionData = {
    deviceId:myDeviceId,
    name: functionName,
    argument: functionArg,
    auth: myParticleAccessToken
  }
  function onSuccess(e) { console.log(functionName + " call success"); }
  function onFailure(e) { console.log(functionName + " call failed");
  console.dir(e); }
  watch.particle.callFunction(functionData).then(onSuccess,onFailure);

  updateWatch();
}


function setup() {

  // Get ready to subscribe to the event stream
  function onSuccess(stream) {
    // DONE:  This will "subscribe' to the stream and get the state"
    // console.log("getEventStream success");
    stream.on('event', function(data) {
      // console.log("Event: ", data);
      newWatchEvent(data);
    });
    // NOTE: This is here in the callback to the subscribe --- it will request the state
    //       once successbully subscribed.
  }
  function onFailure(e) { console.log("getEventStream call failed"), console.dir(e) }
  // Subscribe to the stream
  watch.particle.getEventStream( { name: topic, auth: myParticleAccessToken }).then(onSuccess, onFailure)
}

// Observer Function===================================
// sends an update so that the UI can be updated
function updateWatch() {
  if(watch.observerFunc){
    watch.observerFunc(watch);
  }
}

//sets observer as a func and updates
function setObserverFunc(observerFunc){
  watch.observerFunc = observerFunc;
  updateWatch();
}

// watch.particle.getEventStream( { deviceId: 'mine', auth: myParticleAccessToken }).then(
//   function (stream) {
//     // DONE:  This will "subscribe' to the stream and get the state"
//
//     stream.on('event', function(data) {
//     console.log("Event: ", data);
//     newWatchEvent(data);
//   });
//
//     // NOTE: This is here in the callback to the subscribe --- it will request the state
//     //       once successbully subscribed.
//   },
//   function (e) { console.log("getEventStream call failed"), console.dir(e) });
