let pos_CircleX, pos_CircleY;
let pos_CircleAX, pos_CircleAY;
let pos_CircleBX, pos_CircleBY;
let radioA, radioB;
let maxColorValue = 255;
let bMousePressed = false;


let socket;
let joystick3d;
let joystick2d_posx;
let joystick2d_posy;
let distX = 0;
let distY = 0;

//colors
let c_Over;
let c_NotOver;
let blueDark_Color;
let cyanPale_Color;
let whiteBone_Color;
let orangePale_Color;
let strokeGrey_Color;
let background_Color ;
let fillButton_Color;

//DOM
let myCanvas;

//Control vars
let statusMachine = 0; // 0 is waiting , 1 is ready to play , 2 is playing, 3 finish
let counterTimer_Start = 3000;
let minTime_toSendData = 33;
let lastMillis_toSendData = 0;
let counterTime;
let timerLeftMachineStatus = 3;
let idJoystick = -1;
let idCounterServer = -1;
let textToScreen = "";

//==============================================
function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}

//==============================================
function setupColors(){
	c_Over = 200;
	c_NotOver = 120;

	blueDark_Color = color(50, 71, 75);
	cyanPale_Color = color(38, 170, 168);
	whiteBone_Color = color(241, 245, 227);
	orangePale_Color = color(240, 162, 109);
	strokeGrey_Color = blueDark_Color;
	background_Color = cyanPale_Color;
	fillButton_Color = orangePale_Color;	
}

//==============================================
function setup() {
  
 	setupColors();

	myCanvas = createCanvas(window.screen.width, window.screen.height);
	
	frameRate(30); 

	let fs = fullscreen();
    fullscreen(!fs);

    //counter Millis
    counterTimer_fps = millis();

	// document.body registers gestures anywhere on the page
	//var hammer = new Hammer(document.body, options);
	//hammer.get('pinch').set({ enable: false });
	//hammer.get('rotate').set({ enable: false });  
	//hammer.on("pinch", scaleRect);
	//<hammer.on("rotate", rotateRect);

	socket = io.connect('http://localhost:8004');
	//socket = io.connect();

	//Kind of old p5js verision. Does not loadModel, not push()... , not mousePressed inside draw... 
	//joystick3d = loadModel('3dmodels/joystick3d.obj');

	// Callback function
	socket.on('idConnectCounter', data => {
		//stroke(data.color)
		//strokeWeight(data.strokeWidth)
		//line(data.x, data.y, data.px, data.py)
		console.log("Id Counter = " + data.id);
		idCounterServer = data.id;
		if(idCounterServer > 0 && idCounterServer <1){
			idJoystick = idCounterServer; //1?
		}
		else if(idCounterServer > 0 && idCounterServer <3){
			idJoystick = idCounterServer; //1?
		}
		else{
			idJoystick = 0; //1?
		}
		
		console.log("Text = " + data.message);
		textToScreen = data.message;
	})


}


//MAIN DRAW and MACHINE STATUS
//==============================================
function draw() {
	background(background_Color); 

	if(statusMachine == 0){
		drawHello();
		if (mouseIsPressed && idCounterServer < 3) { //Fast way to say only iD1 and Id2 can play
    		stroke(strokeGrey_Color);
    		fill(fillButton_Color);
    		ellipse(mouseX, mouseY, 500, 500);
    		statusMachine++;
  		}

	}
	else if(statusMachine == 1){
		draw321();
		counterTime = int((millis() - counterTimer_Start)/1000);
		if(counterTime > timerLeftMachineStatus){
			statusMachine ++;
			counterTimer_Start = millis();
		}
	}
	else if(statusMachine == 2){ 
		drawJoystick_XY();
		//drawJoystick_A();
		//drawJoystick_B();
	}
	else if(statusMachine == 3){
		drawFinish();
		counterTime = int((millis() - counterTimer_Start)/1000);
		if(counterTime > timerLeftMachineStatus){

			location.reload();
			//statusMachine = 0;
			//timerLeftMachineStatus = millis();
			//setup();
		}
	}
	
	drawMouse();
}

//==============================================
function drawMouse() {
	//Draw Circles
	push();
	if (bMousePressed) {

	} else {
		stroke(100); 
		fill(100);
		ellipse(mouseX, mouseY, 4, 4);
	}
	
	pop();
}

//==============================================
function drawHello() {
	push();

	fill(40, 10, 10);
	rect(0,0, window.screen.width, window.screen.height);
	//text("Hello World", 200, 200);
	textSize(40);
	textAlign(CENTER, CENTER);
	stroke(255);
	fill(255);

	if(idJoystick > 0)text("Player "+str(idJoystick), window.screen.width*0.5, window.screen.height*0.5 - 30);
	if(idCounterServer > 0)text("waiting list number..."+str(idCounterServer), window.screen.width*0.5, window.screen.height*0.5 + 30);

	text(textToScreen, window.screen.width*0.5, window.screen.height*0.5 + 90);

	pop();
}

//==============================================
function draw321() {
	push();
	fill(40, 10, 10);
	rect(0,0, window.screen.width, window.screen.height);
	//text("Hello World", 200, 200);
	textSize(40);
	textAlign(CENTER, CENTER);
	stroke(255);
	fill(255);

	let restartTime = timerLeftMachineStatus - counterTime;
	if(restartTime < 0 )restartTime = 0;
	text("We start in .."+nf(str(restartTime, 0, 0)) , window.screen.width*0.5, window.screen.height*0.5);

	pop();
}


//==============================================
function drawFinish() {
	push();

	textSize(40);
	textAlign(CENTER, CENTER);
	stroke(255);
	fill(255);
	//draw finish until wating seconds to restart play if they want
	let restartTime = timerLeftMachineStatus - counterTime;
	if(restartTime < 0 )restartTime = 0;
	text("Thanks for playing! " +nf(str(restartTime, 0, 0)), window.screen.width*0.5, window.screen.height*0.5);

	pop();
}

//==============================================
function setFinishStatus(){
	counterTimer_Start = millis();
	statusMachine = 3;
	sendMachineStatus(statusMachine, idJoystick);
}

//KEY AND MOUSE EVENTS
//==============================================
function mousePressed(){
	bMousePressed = true;
}

//==============================================
function mouseReleased(){
	bMousePressed = false;
}

function touchStarted() {
 /*if (value === 0) {
    bMousePressed = true;
    console.log("touchStarted = " + bMousePressed);
  } else {
    bMousePressed = false;
    console.log("touchStarted = " + bMousePressed);
  }
  */
}

//==============================================
function keyReleased(){

	if (keyCode === ESCAPE) {
		setFinishStatus();
  	} else if (key === '+') {
    	statusMachine++;
    	counterTimer_Start = millis();
  	}
}

//MESSAGES TO SERVER
//==============================================
function sendMachineStatus(x, y) {
  // Make a little object with mouseX and mouseY
  let data = {
    sm: x,
    id: y
  };
  // Send that object to the socket
  socket.emit('statusMachine',data);
}

//==============================================
function sendPosJoystick(x, y) {
  // Make a little object with mouseX and mouseY
  let data = {
  	id: idJoystick,
    x: x,
    y: y
  };
  // Send that object to the socket
  socket.emit('posJoystick',data);
}

//==============================================
function sendClickJoystick(_a, _b) {
  let data = {
  	id: idJoystick,
    a: _a,
    b: _b
  };
  // Send that object to the socket
  socket.emit('clickJoystick',data);
}

//==============================================
function drawJoystick_XY() {
	
	/////////////
	push();
	
	pos_CircleX = window.screen.width*0.5;//0.25;
	pos_CircleY = window.screen.height*0.5;
	let radioXY = window.screen.width*0.25;
	let delta_radio = radioXY*0.1;	

	fill(10,10,10);
	ellipse(pos_CircleX, pos_CircleY, radioXY, radioXY);
	fill(c_NotOver);
	ellipse(pos_CircleX, pos_CircleY, radioXY-delta_radio, radioXY-delta_radio);

	//dist for mouse interaction
	let distMouse = dist(mouseX, mouseY, pos_CircleX, pos_CircleY);
	
	if(bMousePressed){
		
		if(distMouse < radioXY*0.5){
			fill(c_Over);
			ellipse(pos_CircleX, pos_CircleY, radioXY-delta_radio, radioXY-delta_radio);
			sendClickJoystick(1, 0);
		}
		else {
			fill(c_NotOver);
			ellipse(pos_CircleX, pos_CircleY, radioXY-delta_radio, radioXY-delta_radio);
		}
		
	}

	angleMode(DEGREES);
	let a = atan2(mouseY - pos_CircleY, mouseX - pos_CircleX);

	/*
	pushMatrix();
	translate(pos_CircleX, pos_CircleY);
	rotate(a);
  	rect(-20*8, -5*1, 40*8, 10*1); // Larger rectangle is rotating in degrees
  	popMatrix();
  	*/

	if(distMouse < radioXY*0.5*0.8){
		joystick2d_posx = mouseX;
		joystick2d_posy = mouseY;
		distX = map(dist(mouseX, 0, pos_CircleX, 0), 0, 150, 0, 1);
		distY = map(dist(0, mouseY, 0, pos_CircleY), 0, 150, 0, 1);
		//TODO map 0 range area
		if(distX < 0.1){
			distX = 0;
			joystick2d_posx = pos_CircleX;
		}
		if(distY < 0.1){
			distY = 0;
			joystick2d_posy = pos_CircleY;
		}

		///////////////////////////////////////
		//Signed Direction 
		if(mouseX < pos_CircleX){
			distX = distX * -1;
		}
		if(mouseY < pos_CircleY){
			distY = distY * -1;
		}
		
	}else{
		joystick2d_posx = pos_CircleX;
		joystick2d_posy = pos_CircleY;
		distX = 0;
		distY = 0;
	}

	//socketio emit // TODO : emit each 33 millis or less, but not every frame
	let auxCounter = int(millis() - lastMillis_toSendData);
	if(auxCounter > minTime_toSendData){
		sendPosJoystick(distX, distY);
		//console.log("data send -> sendPosJoystick");
		lastMillis_toSendData = millis();
	}
	

	fill(90,90,90);
	strokeWeight(3);
	ellipse(joystick2d_posx, joystick2d_posy, radioXY*0.3, radioXY*0.3);
	textSize(32);
	//String text2Show = "X";
	fill(0,0,0);
	stroke(210,210,210);
	textAlign(CENTER, CENTER);
	text(nf(str(distX), 0, 2), joystick2d_posx, joystick2d_posy-20);
	text(nf(str(distY), 0, 2), joystick2d_posx, joystick2d_posy+20);

	pop();
}

//==============================================
function drawJoystick_A() {

	/////////////
	push();

	pos_CircleAX = window.screen.width*0.5 + window.screen.width*0.125;
	pos_CircleAY = window.screen.height*0.5;
	radioA = window.screen.width*0.125;
	let delta_radio = radioA*0.1;

	fill(10,10,10);
	ellipse(pos_CircleAX, pos_CircleAY, radioA, radioA);
	fill(c_NotOver);
	ellipse(pos_CircleAX, pos_CircleAY, radioA-delta_radio, radioA-delta_radio);

	if(bMousePressed){
		let distMouse = dist(pos_CircleAX, pos_CircleAY, mouseX, mouseY);
		
		if(distMouse < radioA*0.5){
			fill(c_Over);
		}
		else fill(c_NotOver);

		ellipse(pos_CircleAX, pos_CircleAY, radioA-delta_radio, radioA-delta_radio);
	}
		
	pop();
}

//==============================================
function drawJoystick_B() {

	let gapAB = 50;
	
	/////////////
	push();

	pos_CircleBX = pos_CircleAX + gapAB + radioA;
	pos_CircleBY = window.screen.height*0.5;
	radioB = window.screen.width*0.125;
	let delta_radio = radioB*0.1;

	fill(10,10,10);
	ellipse(pos_CircleBX, pos_CircleBY, radioB, radioB);
	fill(c_NotOver)
	ellipse(pos_CircleBX, pos_CircleBY, radioB-delta_radio, radioB-delta_radio);

	if(bMousePressed){
		let distMouse = dist(pos_CircleBX, pos_CircleBY, mouseX, mouseY);
		//console.log("distMouse A = " + distMouse);
		if(distMouse < radioB*0.5){
			fill(c_Over);
		}
		else fill(c_NotOver);

		ellipse(pos_CircleBX, pos_CircleBY, radioB-delta_radio, radioB-delta_radio);
	}

	pop();
}

