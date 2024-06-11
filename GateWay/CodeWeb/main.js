// Import the functions you need from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.8.1/firebase-app.js";
import { getAnalytics} from "https://www.gstatic.com/firebasejs/10.8.1/firebase-analytics.js";
import {getDatabase, ref, set, onValue, get } from 'https://www.gstatic.com/firebasejs/10.8.1/firebase-database.js'
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBQIn4S4QqTubmohR0n7DPo7Kpc-6sx--M",
  authDomain: "enviromentmoritering.firebaseapp.com",
  databaseURL: "https://enviromentmoritering-default-rtdb.firebaseio.com",
  projectId: "enviromentmoritering",
  storageBucket: "enviromentmoritering.appspot.com",
  messagingSenderId: "28849715155",
  appId: "1:28849715155:web:121d5b159286c28c766126",
  measurementId: "G-D83D9DRCXM"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const db = getDatabase();

//  console.log(db);

// Lấy giá trị từ các thẻ span
let tempVal = document.getElementById("tempVal").textContent;
let humidVal = document.getElementById("humidVal").textContent;
let pressVal = document.getElementById("pressVal").textContent;


// Khai báo các biến cấu hình
let ConfigTemp = 40; // Giá trị khởi tạo ban đầu 
let ConfigHumid = 80;
let ConfigPress = 1020;

let tempAlert = 0;
let humiAlert = 0;
let pressAlert = 0;

//  Khai báo các biến để lưu trữ phần tử input 
let tempInput = document.querySelector(".input-data input[type='text']");
let humidInput = document.querySelectorAll(".input-data input[type='text']")[1];
let pressInput = document.querySelectorAll(".input-data input[type='text']")[2];

const tempErrorMessage = document.querySelector(".wrapper .input-data .error-message");
const humidErrorMessage = document.querySelectorAll(".wrapper .input-data .error-message")[1];
const pressErrorMessage = document.querySelectorAll(".wrapper .input-data .error-message")[2];


// Khai báo cho chart
var temperature_value = [];
var temperature_time = [];
var dataTem = 0;

var humidity_value = [];
var humidity_time = [];
var dataHumid = 0;

var pressure_value = [];
var pressure_time = [];
var dataPress = 0;

// Khai báo giá trị interval
var timeTemp = 1000;
var timeHumi = 1000;
var timePress = 1000;
var start1 = 1;
var start2 = 1;
var start3 = 1;

// Bat tat sensor
var on_off1 = 1;
var on_off2 = 1;

// Lấy giá trị của firebase
get(ref(db, "Temp")).then((snapshot) => {
  if (snapshot.exists()) {
    if(on_off1 == 1)
      tempVal = snapshot.val();
    else
      tempVal = "OFF";
  } else {
    console.log("No data available");
  }
}).catch((error) => {
  console.error(error);
});

get(ref(db, "Humi")).then((snapshot) => {
  if (snapshot.exists()) {
    if(on_off1 == 1)
      humidVal = snapshot.val();
    else
      humidVal = "OFF";
  } else {
    console.log("No data available");
  }
}).catch((error) => {
  console.error(error);
});

get(ref(db, "Press")).then((snapshot) => {
  if (snapshot.exists()) {
    if(on_off2 == 1)
      pressVal = snapshot.val();
    else
      pressVal = "OFF";
  } else {
    console.log("No data available");
  }
}).catch((error) => {
  console.error(error);
});

// --------------- Graph of Temperature --------------------
const ctx = document.getElementById('TemChart');
var chartTem = new Chart(ctx, {
  type: 'bar',
  data: {
    labels: [],
    datasets: [{
      label: 'Temperature',
      data: [],
      backgroundColor: [
          'rgba(255, 26, 104, 0.2)',
          'rgba(54, 162, 235, 0.2)',
          'rgba(255, 206, 86, 0.2)',
          'rgba(75, 192, 192, 0.2)',
          'rgba(153, 102, 255, 0.2)',
          'rgba(255, 159, 64, 0.2)',
          'rgba(0, 0, 0, 0.2)'
          ],
      borderColor: [
          'rgba(255, 26, 104, 1)',
          'rgba(54, 162, 235, 1)',
          'rgba(255, 206, 86, 1)',
          'rgba(75, 192, 192, 1)',
          'rgba(153, 102, 255, 1)',
          'rgba(255, 159, 64, 1)',
          'rgba(0, 0, 0, 1)'
          ],
      borderWidth: 1
    }]
  },
  options: {
    scales: {
      y: {
        beginAtZero: true
      }
    }
  }
});


// --------------- Graph of Humidity --------------------
const ctx1 = document.getElementById('HumidChart');
var chartHumid = new Chart(ctx1, {
  type: 'bar',
  data: {
    labels: [],
    datasets: [{
      label: 'Humidity',
      data: [],
      backgroundColor: [
          'rgba(255, 26, 104, 0.2)',
          'rgba(54, 162, 235, 0.2)',
          'rgba(255, 206, 86, 0.2)',
          'rgba(75, 192, 192, 0.2)',
          'rgba(153, 102, 255, 0.2)',
          'rgba(255, 159, 64, 0.2)',
          'rgba(0, 0, 0, 0.2)'
          ],
          borderColor: [
              'rgba(255, 26, 104, 1)',
              'rgba(54, 162, 235, 1)',
              'rgba(255, 206, 86, 1)',
              'rgba(75, 192, 192, 1)',
              'rgba(153, 102, 255, 1)',
              'rgba(255, 159, 64, 1)',
              'rgba(0, 0, 0, 1)'
              ],
      borderWidth: 1
    }]
  },
  options: {
    scales: {
      y: {
        beginAtZero: true
      }
    }
  }
});


// --------------- Graph of Pressure --------------------
const ctx2 = document.getElementById('PressChart');
var chartPress = new Chart(ctx2, {
  type: 'bar',
  data: {
    labels: [],
    datasets: [{
      label: 'Pressure',
      data: [],
      backgroundColor: [
          'rgba(255, 26, 104, 0.2)',
          'rgba(54, 162, 235, 0.2)',
          'rgba(255, 206, 86, 0.2)',
          'rgba(75, 192, 192, 0.2)',
          'rgba(153, 102, 255, 0.2)',
          'rgba(255, 159, 64, 0.2)',
          'rgba(0, 0, 0, 0.2)'
          ],
          borderColor: [
              'rgba(255, 26, 104, 1)',
              'rgba(54, 162, 235, 1)',
              'rgba(255, 206, 86, 1)',
              'rgba(75, 192, 192, 1)',
              'rgba(153, 102, 255, 1)',
              'rgba(255, 159, 64, 1)',
              'rgba(0, 0, 0, 1)'
              ],
      borderWidth: 1
    }]
  },
  options: {
    scales: {
      y: {
        beginAtZero: true
      }
    }
  }
});





//  Lắng nghe sự kiện click vào các nút SEND
document.getElementById("SendTemp").addEventListener("click", function (event) {
  // event.preventDefault(); // Ngăn chặn hành động mặc định của thẻ <a>
  // ConfigTemp = parseFloat(tempInput.value); // Lấy giá trị và chuyển thành số floating-point 
  
  // var tempVal = parseFloat(document.getElementById("tempVal").textContent);
  // console.log(ConfigTemp)
  //   // So sánh giá trị tempVal với ConfigTemp và đổi màu box
  // if (tempVal > ConfigTemp) {
  //   document.getElementById("box-temp").style.borderColor  = "red";
  // }
  // else
  // {
  //   document.getElementById("box-temp").style.borderColor  = "#149279";
  // }
  const tempInput = document.querySelector(".input-data input[type='text']");
  const tempErrorMessage = document.querySelector(".input-data .error-message");

  if (isNaN(tempInput.value)) { // Kiểm tra nếu không phải là số (isNaN = is Not a Number)
      tempErrorMessage.style.display = "block"; // Hiện thông báo lỗi
  } else {
    tempErrorMessage.style.display = "none"; // Giấu thông báo lỗi
    ConfigTemp = parseFloat(tempInput.value);
    console.log(ConfigTemp)
  }
});

document.getElementById("SendHumid").addEventListener("click", function (event) {
  event.preventDefault(); 
  if (isNaN(humidInput.value)) { 
    humidErrorMessage.style.display = "block"; 
  } else {
    humidErrorMessage.style.display = "none"; 
    ConfigHumid = parseFloat(humidInput.value); 
  }
});

document.getElementById("SendPress").addEventListener("click", function (event) {
  event.preventDefault();
  
  if (isNaN(pressInput.value)) { 
    pressErrorMessage.style.display = "block";  
  } else {
    pressErrorMessage.style.display = "none";  
    ConfigPress = parseFloat(pressInput.value); 
  }
});


function checkTemp(){
  var tempVal = parseFloat(document.getElementById("tempVal").textContent);  
  // So sánh giá trị tempVal với ConfigTemp và đổi màu box
  if (tempVal > ConfigTemp) {
    document.getElementById("box-temp").style.borderColor  = "red";
    if(tempAlert == 0){
      window.alert("Cảnh báo ! Nhiệt độ vượt ngưỡng " + String(ConfigTemp) + "°C ! Nhiệt độ hiện tại là " + String(tempVal) + "°C");
      tempAlert = 1;
    }
  }
  else
  {
    document.getElementById("box-temp").style.borderColor  = "#149279";
    tempAlert = 0;
  }
}

function checkHumi(){
  var humidVal = parseFloat(document.getElementById("humidVal").textContent);
  if (humidVal > ConfigHumid) {
    document.getElementById("box-humid").style.borderColor  = "red";
    if(humiAlert == 0){
      window.alert("Cảnh báo ! Độ ẩm vượt ngưỡng " + String(ConfigHumid) + "% ! Độ ẩm hiện tại là " + String(humidVal) + "%");
      humiAlert = 1;
    }
  }
  else
  {
    document.getElementById("box-humid").style.borderColor  = "#149279";
    humiAlert = 0;
  }
}

function checkPress(){
  var pressVal = parseFloat(document.getElementById("pressVal").textContent);
  if (pressVal > ConfigPress) {
    document.getElementById("box-press").style.borderColor  = "red";
    if(pressAlert == 0){
      window.alert("Cảnh báo ! Áp suất vượt ngưỡng " + String(ConfigPress) + " hPa ! Áp suất hiện tại là " + String(pressVal) + " hPa");
      pressAlert = 1;
    }
    
  }
  else
  {
    document.getElementById("box-press").style.borderColor  = "#149279";
    pressAlert = 0;
  }
}

function displayTemp(){
  get(ref(db, "Temp")).then((snapshot) => {
    if (snapshot.exists()) {
      tempVal = snapshot.val();
    } else {
      console.log("No data available");
    }
  }).catch((error) => {
    console.error(error);
  });
  document.getElementById("tempVal").textContent = tempVal;

  var time = new Date();
  var gio = time.getHours();
  var phut = time.getMinutes();
  var giay = time.getSeconds();
  if (gio < 10) 
      gio = "0" + gio;
  if (phut < 10) 
      phut = "0" + phut;
  if (giay < 10) 
      giay = "0" + giay;
  
  if(temperature_time.length == 12){
      temperature_value = temperature_value.slice(1);
      temperature_value.push(tempVal);
      temperature_time = temperature_time.slice(1);
      temperature_time.push(gio + ":" + phut + ":" + giay);
      chartTem.data.labels.unshift(temperature_time[temperature_time.length - 1]);
      chartTem.data.labels.pop();
      chartTem.data.datasets.forEach((dataset) => {
        dataset.data.unshift(temperature_value[temperature_value.length-1]);
      });
      chartTem.data.datasets.forEach((dataset) => {
        dataset.data.pop();
      });

  }
  else{
      temperature_value.push(tempVal);
      temperature_time.push(gio + ":" + phut + ":" + giay);
      chartTem.data.labels.unshift(temperature_time[temperature_time.length - 1]);
      chartTem.data.datasets.forEach((dataset) => {
        dataset.data.unshift(temperature_value[temperature_value.length-1]);
      });
  }
  console.log(temperature_value);
  console.log(temperature_time);
  chartTem.update();
}

function displayHumi(){
  get(ref(db, "Humi")).then((snapshot) => {
    if (snapshot.exists()) {
      humidVal = snapshot.val();
    } else {
      console.log("No data available");
    }
  }).catch((error) => {
    console.error(error);
  });
  document.getElementById("humidVal").textContent = humidVal;

  var time = new Date();
  var gio = time.getHours();
  var phut = time.getMinutes();
  var giay = time.getSeconds();
  if (gio < 10) 
      gio = "0" + gio;
  if (phut < 10) 
      phut = "0" + phut;
  if (giay < 10) 
      giay = "0" + giay;
  
  if(humidity_time.length == 12){
    humidity_value = humidity_value.slice(1);
    humidity_value.push(humidVal);
    humidity_time = humidity_time.slice(1);
    humidity_time.push(gio + ":" + phut + ":" + giay);
    chartHumid.data.labels.unshift(humidity_time[humidity_time.length - 1]);
    chartHumid.data.labels.pop();
    chartHumid.data.datasets.forEach((dataset) => {
        dataset.data.unshift(humidity_value[humidity_value.length-1]);
      });
      chartHumid.data.datasets.forEach((dataset) => {
        dataset.data.pop();
      });

  }
  else{
    humidity_value.push(humidVal);
      humidity_time.push(gio + ":" + phut + ":" + giay);
      chartHumid.data.labels.unshift(humidity_time[humidity_time.length - 1]);
      chartHumid.data.datasets.forEach((dataset) => {
        dataset.data.unshift(humidity_value[humidity_value.length-1]);
      });
  }
  console.log(humidity_value);
  console.log(humidity_time);
  chartHumid.update();
}


function displayPress(){
  get(ref(db, "Press")).then((snapshot) => {
    if (snapshot.exists()) {
      pressVal = snapshot.val();
    } else {
      console.log("No data available");
    }
  }).catch((error) => {
    console.error(error);
  });
  document.getElementById("pressVal").textContent = pressVal;

  var time = new Date();
  var gio = time.getHours();
  var phut = time.getMinutes();
  var giay = time.getSeconds();
  if (gio < 10) 
      gio = "0" + gio;
  if (phut < 10) 
      phut = "0" + phut;
  if (giay < 10) 
      giay = "0" + giay;
  
  if(pressure_time.length == 12){
    pressure_value = pressure_value.slice(1);
    pressure_value.push(pressVal);
    pressure_time = pressure_time.slice(1);
      pressure_time.push(gio + ":" + phut + ":" + giay);
      chartPress.data.labels.unshift(pressure_time[pressure_time.length - 1]);
      chartPress.data.labels.pop();
      chartPress.data.datasets.forEach((dataset) => {
        dataset.data.unshift(pressure_value[pressure_value.length-1]);
      });
      chartPress.data.datasets.forEach((dataset) => {
        dataset.data.pop();
      });

  }
  else{
    pressure_value.push(pressVal);
    pressure_time.push(gio + ":" + phut + ":" + giay);
    chartPress.data.labels.unshift(pressure_time[pressure_time.length - 1]);
    chartPress.data.datasets.forEach((dataset) => {
        dataset.data.unshift(pressure_value[pressure_value.length-1]);
      });
  }
  console.log(pressure_value);
  console.log(pressure_time);
  chartPress.update();
}


// Get references to the canvas and the overlay
const pressChartCanvas = document.getElementById("PressChart");
const humidChartCanvas = document.getElementById("HumidChart");
const tempChartCanvas = document.getElementById("TemChart");
const sensorOffOverlay3 = document.querySelector(".sensor-off-overlay-3");
const sensorOffOverlay2 = document.querySelector(".sensor-off-overlay-2");
const sensorOffOverlay1 = document.querySelector(".sensor-off-overlay-1");


let sensor_on_off1 = 1; // Initially assume sensor is on

let sensor_on_off2 = 1; // Initially assume sensor is on




function toggleSensorDisplay2() {
  if (sensor_on_off2 === 0) {
    pressChartCanvas.style.display = "none";
    sensorOffOverlay2.style.display = "flex";
  } else {
    pressChartCanvas.style.display = "block"; // Or inline-block
    sensorOffOverlay2.style.display = "none";
  }
}


function toggleSensorDisplay1() {
  if (sensor_on_off1 === 0) {
    humidChartCanvas.style.display = "none";
    tempChartCanvas.style.display = "none";
    sensorOffOverlay1.style.display = "flex";
    sensorOffOverlay3.style.display = "flex";
  } else {
    humidChartCanvas.style.display = "block"; // Or inline-block
    tempChartCanvas.style.display = "block"; 
    sensorOffOverlay1.style.display = "none";
    sensorOffOverlay3.style.display = "none";
  }
}
toggleSensorDisplay1() ;
toggleSensorDisplay2() ;

setInterval(toggleSensorDisplay1, 100);
setInterval(toggleSensorDisplay2, 100);
setInterval(checkTemp, 100);
setInterval(checkHumi, 100);
setInterval(checkPress, 100);

var intervalTemp;
var intervalHumi;
var intervalPress;


onValue(ref(db, "timeTemp"), (snapshot) => {
  timeTemp = snapshot.val();
  if(start1){
    start1 = 0;
    intervalTemp = setInterval(displayTemp, timeTemp);
  }
  else{
    clearInterval(intervalTemp);
    intervalTemp = setInterval(displayTemp, timeTemp);
  }
});

onValue(ref(db, "timeHumi"), (snapshot) => {
  timeHumi = snapshot.val();
  if(start2){
    start2 = 0;
    intervalHumi = setInterval(displayHumi, timeHumi);
  }
  else{
    clearInterval(intervalHumi);
    intervalHumi = setInterval(displayHumi, timeHumi);
  }
});

onValue(ref(db, "timePress"), (snapshot) => {
  timePress = snapshot.val();
  if(start3){
    start3 = 0;
    intervalPress = setInterval(displayPress, timePress)
  }
  else{
    clearInterval(intervalPress);
    intervalPress = setInterval(displayPress, timePress);
  }
});


onValue(ref(db, "sensor1"), (snapshot) => {
  sensor_on_off1 = snapshot.val();
  if(sensor_on_off1 == 1)
    on_off1 = 1;
  else
    on_off1 = 0;
});

onValue(ref(db, "sensor2"), (snapshot) => {
  sensor_on_off2 = snapshot.val();
  if(sensor_on_off2 == 1)
    on_off2 = 1;
  else
    on_off2 = 0;
});