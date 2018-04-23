var myParticleAccessToken = "f76cd4700e1489f30c1fdf0f54d702d63236800c";//find out when plug in
var myDeviceId =            "1f003c001947343438323536";
var topic =                 "thisWatch";

function newWatchEvent(objectContainingData) {
  // TODO: Parse the incoming state and update any listeners
  // Hints:
  //    1. Use console.dir() to show the state object on the console.
  //    2. Consider JSON.parse, but be aware that proper JSON enclosed property
  //       names in DOUBLE quotes ("proper":value)
  var obj = JSON.parse(objectContainingData.data);

  //console.log(objectContainingData);
  console.dir(obj);
  console.log(obj.state);
  // switch (obj.state)
  // {
  //   case 0: // OPEN
  //   garage.door.state = "open";
  //   console.log("open");
  //   break;
  //   case 1: // CLOSED
  //   garage.door.state = "closed";
  //   console.log("closed");
  //   break;
  //   case 2: // OPENING
  //   garage.door.state = "opening";
  //   console.log("opening");
  //   break;
  //   case 3: // CLOSING
  //   garage.door.state = "closing";
  //   console.log("closing");
  //   break;
  // }
  // console.log(garage.door.state);
  //
  // switch (obj.lS)
  // {
  //   case "0": // ON
  //   garage.light.on = true;
  //   break;
  //   case "1": // FADING
  //   garage.light.on = true;
  //   break;
  //   case "2": // OFF
  //   garage.light.on = false;
  //   break;
  // }
  // console.log(garage.light.on);
  //
  // garage.door.autoClose.enabled = obj.aCE;
  //
  // garage.door.autoClose.time = obj.aCT;
  //
  // garage.light.brightness = obj.lB;
  //
  // garage.light.autoOff.time = obj.aOT;

  updateGarage();
}
//AHHHHHHH MESSED UP BRACKETS
var watch = {

  // NOTE: A new object to access particle JavaScript functions
  particle: new Particle(),

  state: "closed",

  getState: function() {
    return watch.state;
  },
  changeState: function(){
    watch.buttonPush();
    watch.sendState(this.state);
  }


  buttonPush: function() {
    console.log("(fake) button pushed");
    callParticleFunction("trigButton", "test");
  },

  sendState: function(myState){
    this.state = myState;
    callParticleFunction("publishState", "" + this.state);
  }

  temp: {
    enabled: false,
    curTemp: 0,
    tempTarget: 30, // 30 deg

    getTime: function(){
      return watch.time.curTime;
    }

    //update CURRENT temperature
    changeTemp: function(newTemp){
      watch.temp.curTemp = newTemp;
      callParticleFunction("publishState","" + this.curTemp)
    },

    //update TARGET temperature
    setTemp: function(nextTemp){
      watch.temp.tempTarget = nextTemp;
      callParticleFunction("publishState",""+ this.tempTarget);
    }
  },

  time: {
    enabled: false,
    curTime: 0,
    timeTarget: 30, // 30 deg

    getTime: function(){
      return watch.time.curTime;
    }

    //update CURRENT time
    changeTime: function(newTime){
      watch.time.curTime = newTime;
      callParticleFunction("publishState","" + this.curTime)
    },

    //update TARGET time
    setTime: function(nextTime){
      watch.time.timeTarget = nextTime;
      callParticleFunction("publishState","" + this.timeTarget)
    }
  },
  //
  //   changeState: function() {
  //     //simulate button push
  //     watch.cook.buttonPush();
  //     watch.cook.sendState();
  //   },
  //   getAutoCloseEnabled: function () {
  //     return garage.door.autoClose.enabled;
  //   },
  //   getAutoCloseTime: function() {
  //     return garage.door.autoClose.time;
  //   },
  //   sendState: function(myState) {
  //     this.state = myState;
  //     callParticleFunction("publishState", "" + this.state);
  //   },
  //   buttonPush: function() {
  //     console.log("(fake) button pushed");
  //     callParticleFunction("trigButton", "test");
  //   },
  //   changeAutoClose: function(s) {
  //     console.log("autoclose changed");
  //     callParticleFunction("changeAC", s);
  //   },
  //   changeAutoCloseTime: function(t) {
  //     if(t > 60){
  //       newTime = 60;
  //       alert('time set for greater than 60s, Garage cant wait that long!');
  //     }
  //     else if(t < 5){
  //       newTime = 5;
  //       alert('time set for less than 5s, Garage aint that quick!');
  //     }
  //     console.log("autoclose time changed");
  //     callParticleFunction("changeACT", t);
  //   }
  // },
  light: {
    on: false,
    brightness: 50, // 50% brightness
    autoOff: {
      enabled: true,
      time: 2000 // 2 seconds
    },
    getState: function() {
      return garage.light.on;
    },
    changeState: function() {
      garage.light.buttonPush();
    },
    buttonPush: function() {
      callParticleFunction("trigLButton", "test");
    },
    setBrightness: function (newBrightness){
      callParticleFunction("changeLB", newBrightness);
    },
    getBrightness: function (){
      return garage.light.brightness;
    },
    setAutoOffTime: function (newTime) {
      callParticleFunction("changeLOT", newTime);
    },
    getAutoOffTime: function() {
      return garage.light.autoOff.time;
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
  garage.particle.callFunction(functionData).then(onSuccess,onFailure);

  updateGarage();
}


function setup() {

  // Get ready to subscribe to the event stream
  function onSuccess(stream) {
    // DONE:  This will "subscribe' to the stream and get the state"
    console.log("getEventStream success");
    stream.on("event", newGarageEvent);

    // NOTE: This is here in the callback to the subscribe --- it will request the state
    //       once successbully subscribed.
  }
  function onFailure(e) { console.log("getEventStream call failed"), console.dir(e) }
  // Subscribe to the stream
  garage.particle.getEventStream( { name: topic, auth: myParticleAccessToken }).then(onSuccess, onFailure)
}

// Observer Function===================================
// sends an update so that the UI can be updated
function updateGarage() {
  if(watch.observerFunc){
    watch.observerFunc(watch);
  }
}

//sets observer as a func and updates
function setObserverFunc(observerFunc){
  watch.observerFunc = observerFunc;
  updateGarage();
}

watch.particle.getEventStream( { name: topic, auth: myParticleAccessToken }).then(
  function (stream) {
    // DONE:  This will "subscribe' to the stream and get the state"
    console.log("getEventStream success");
    stream.on("event", newWatchEvent);

    // NOTE: This is here in the callback to the subscribe --- it will request the state
    //       once successbully subscribed.
  },
  function (e) { console.log("getEventStream call failed"), console.dir(e) });
