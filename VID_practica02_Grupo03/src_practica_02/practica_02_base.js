/*
* 
* Practica_02_base.js
* Videojuegos (30262) - Curso 2019-2020
* 
* Parte adaptada de: Alex Clarke, 2016, y Ed Angel, 2015.
* 
*/

// Variable to store the WebGL rendering context
var gl;
var canvas;
//----------------------------------------------------------------------------
// MODEL DATA 
//----------------------------------------------------------------------------

//Define points' position vectors
const cubeVerts = [
    [0.5, 0.5, 0.5, 1], //0
    [0.5, 0.5, -0.5, 1], //1
    [0.5, -0.5, 0.5, 1], //2
    [0.5, -0.5, -0.5, 1], //3
    [-0.5, 0.5, 0.5, 1], //4
    [-0.5, 0.5, -0.5, 1], //5
    [-0.5, -0.5, 0.5, 1], //6
    [-0.5, -0.5, -0.5, 1], //7
];

const wireCubeIndices = [
    //Wire Cube - use LINE_STRIP, starts at 0, 30 vertices
    0, 4, 6, 2, 0, //front
    1, 0, 2, 3, 1, //right
    5, 1, 3, 7, 5, //back
    4, 5, 7, 6, 4, //right
    4, 0, 1, 5, 4, //top
    6, 7, 3, 2, 6, //bottom
];

const cubeIndices = [
    //Solid Cube - use TRIANGLES, starts at 0, 36 vertices
    0, 4, 6, //front
    0, 6, 2,
    1, 0, 2, //right
    1, 2, 3,
    5, 1, 3, //back
    5, 3, 7,
    4, 5, 7, //left
    4, 7, 6,
    4, 0, 1, //top
    4, 1, 5,
    6, 7, 3, //bottom
    6, 3, 2,
];

const pointsAxes = [];
pointsAxes.push([2.0, 0.0, 0.0, 1.0]); //x axis is green
pointsAxes.push([-2.0, 0.0, 0.0, 1.0]);
pointsAxes.push([0.0, 2.0, 0.0, 1.0]); //y axis is red
pointsAxes.push([0.0, -2.0, 0.0, 1.0]);
pointsAxes.push([0.0, 0.0, 2.0, 1.0]); //z axis is blue
pointsAxes.push([0.0, 0.0, -2.0, 1.0]);

const pointsWireCube = [];
for (let i = 0; i < wireCubeIndices.length; i++) {
    pointsWireCube.push(cubeVerts[wireCubeIndices[i]]);
}

const pointsCube = [];
for (let i = 0; i < cubeIndices.length; i++) {
    pointsCube.push(cubeVerts[cubeIndices[i]]);
}

const shapes = {
    wireCube: {Start: 0, Vertices: 30},
    cube: {Start: 0, Vertices: 36},
    axes: {Start: 0, Vertices: 6}
};

const red = [1.0, 0.0, 0.0, 1.0];
const green = [0.0, 1.0, 0.0, 1.0];
const blue = [0.0, 0.0, 1.0, 1.0];
const lightred = [1.0, 0.5, 0.5, 1.0];
const lightgreen = [0.5, 1.0, 0.5, 1.0];
const lightblue = [0.5, 0.5, 1.0, 1.0];
const white = [1.0, 1.0, 1.0, 1.0];

const colorsAxes = [
    green, green, //x
    red, red,     //y
    blue, blue,   //z
];

const colorsWireCube = [
    white, white, white, white, white,
    white, white, white, white, white,
    white, white, white, white, white,
    white, white, white, white, white,
    white, white, white, white, white,
    white, white, white, white, white,
];

const colorsCube = [
    lightblue, lightblue, lightblue, lightblue, lightblue, lightblue,
    lightgreen, lightgreen, lightgreen, lightgreen, lightgreen, lightgreen,
    lightred, lightred, lightred, lightred, lightred, lightred,
    blue, blue, blue, blue, blue, blue,
    red, red, red, red, red, red,
    green, green, green, green, green, green,
];

//----------------------------------------------------------------------------
// OTHER DATA 
//----------------------------------------------------------------------------

var model = new mat4();   		// create a model matrix and set it to the identity matrix
var view = new mat4();   		// create a view matrix and set it to the identity matrix
var projection = new mat4();	// create a projection matrix and set it to the identity matrix

var eye, target, up;			// for view matrix

var rotAngle = 0.0;
var rotChange = 0.5;

var program;
var uLocations = {};
var aLocations = {};

var programInfo = {
    program,
    uniformLocations: {},
    attribLocations: {},
};

var objectsToDraw = [
    {
        programInfo: programInfo,
        pointsArray: pointsAxes,
        colorsArray: colorsAxes,
        uniforms: {
            u_colorMult: [1.0, 1.0, 1.0, 1.0],
            u_model: new mat4(),
        },
        primType: "lines",
    },
    {
        programInfo: programInfo,
        pointsArray: pointsWireCube,
        colorsArray: colorsWireCube,
        uniforms: {
            u_colorMult: [1.0, 1.0, 1.0, 1.0],
            u_model: new mat4(),
        },
        primType: "line_strip",
    },
    {
        programInfo: programInfo,
        pointsArray: pointsCube,
        colorsArray: colorsCube,
        uniforms: {
            u_colorMult: [1.0, 1.0, 1.0, 1.0],
            u_model: new mat4(),
        },
        primType: "triangles",
    },
    {
        programInfo: programInfo,
        pointsArray: pointsCube,
        colorsArray: colorsCube,
        uniforms: {
            u_colorMult: [0.5, 0.5, 0.5, 1.0],
            u_model: new mat4(),
        },
        primType: "triangles",
    },
];

function linealCameraMove(vec) {
    let x_no_norm = subtract(target, eye);
    let x = normalize(x_no_norm);
    let l = normalize(cross(x, up));

    let mCB = mat4();
    for (let pCB = 0; pCB < 3; pCB++) {
        mCB[4 * pCB] = x[pCB];
        mCB[4 * pCB + 1] = up[pCB];
        mCB[4 * pCB + 2] = l[pCB];
        mCB[4 * pCB + 3] = eye[pCB];
    }


    let tempEye = mult(mCB, vec);

    eye = vec3(tempEye[0], tempEye[1], tempEye[2]);
    target = add(eye, x_no_norm);
}
function module(v) { val = 0; v.forEach(element => {
    val = val +(element*element)
}); 
    return Math.sqrt(val)
}
function setOrtoCam() {
    //( left, right, bottom, top, near, far )
    ortograph = ortho(-canvas.width / canvas.height , canvas.width / canvas.height,  -canvas.width / canvas.height, canvas.width / canvas.height, 0.1, 10);
    gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, ortograph); // copy projection to uniform value in shader
  }
function updateCamera() {
    view = lookAt(eye, target, up);
    gl.uniformMatrix4fv(programInfo.uniformLocations.view, gl.FALSE, view); // copy view to uniform value in shader
}

fovy = 45.0

function setPerspectiveCam() {  
    //( fovy, aspect, near, far )
    projection = perspective(fovy, canvas.width / canvas.height, 0.1, 100.0);
    gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, projection); // copy projection to uniform value in shader
}

window.addEventListener('keydown', function (event) {
    event.preventDefault();
    const keyName = event.key;
    this.console.log("pulsado " + event.keyCode)

    if (event.keyCode === 107 || event.keyCode === 171) {
        //+
        fovy++;
        if (fovy > 179) {
            fovy = 179;
        }
        setPerspectiveCam();
    } else if (event.keyCode === 109 || event.keyCode === 173) {
        //-
        if (fovy < 1) {
            fovy = 1;
        }
        fovy--;
        setPerspectiveCam();
    }
    if (event.code === "KeyW") {
        console.log("UP");
        linealCameraMove(vec4(1, 0, 0, 1));
    }
    if (event.code == "KeyA") {

        //LEFT
        console.log("LEFT")
        linealCameraMove(vec4(0, 0, -1, 1));

    }
    if (event.code == "KeyS") {
        console.log("DOWN")
        //DOWN
        linealCameraMove(vec4(-1, 0, 0, 1));

    }
    if (event.code == "KeyD") {
        console.log("RIGHT")
        linealCameraMove(vec4(0, 0, 1, 1));
    }
    if (event.code == "KeyO"){
        //O
        setOrtoCam();
    }
    if(event.code == "KeyP"){
        //P
        setPerspectiveCam();
    }
    updateCamera()
});

lastPostion = undefined;
clicked = false;
window.addEventListener("mousedown", function (event) {
    event.preventDefault();
    if(event.button == 0){
        clicked = true;
        lastPostion = [event.clientX, event.clientY];
    }
});
window.addEventListener("mouseup", function (event) {
    event.preventDefault();
    if(event.button == 0){
        clicked = false;
    }
});

window.addEventListener("mousemove", function (event) {
    event.preventDefault();
    if (!clicked) {
        return;
    }
    position = [event.clientX, event.clientY];
    directionMov = [position[0] - lastPostion[0], position[1] - lastPostion[1]];
    //Rotar camara

    lastPostion = position;
    console.log("(X,Y): " + directionMov[0] + ", " + directionMov[1])

    lastPostion = position;
    let x = subtract(target, eye);
    let xNorm = normalize(x);
    let l = normalize(cross(x, up));

    let mCB = mat4();
    for (let pCB = 0; pCB < 3; pCB++) {
        mCB[4 * pCB] = xNorm[pCB];
        mCB[4 * pCB + 1] = up[pCB];
        mCB[4 * pCB + 2] = l[pCB];
        mCB[4 * pCB + 3] = eye[pCB];
    }
    console.log(mCB);

    distance = Math.sqrt(Math.pow(x[0], 2) + Math.pow(x[1], 2) + Math.pow(x[2], 2));
    let targetInOrigin = vec4(distance, 0, 0, 1);

    targetInOrigin = mult(rotate(0.5,
        normalize(cross(normalize(vec3(targetInOrigin[0], targetInOrigin[1], targetInOrigin[2])),
            normalize(vec3(0, directionMov[1], -directionMov[0]))))), targetInOrigin);
    console.log(targetInOrigin);
    let tempTarget = mult(mCB, targetInOrigin);

    console.log(tempTarget);
    target = vec3(tempTarget[0], tempTarget[1], tempTarget[2]);
    console.log(target);

    updateCamera();
});


function generateColors() {
    color = [Math.random(), Math.random(), Math.random(), 1];
    colorCube = []
    for (i = 0; i < 6 * 6; i++) {
        colorCube.push(color);
    }
    return colorCube;
}

function getRandomArbitrary(min, max) {
    return Math.random() * (max - min) + min;
}

function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

class Cube {
    constructor(init_pos, color) {
        this.initPos = init_pos;
        this.rotationAxis = cross(init_pos, vec3(getRandomArbitrary(-1, 1), getRandomArbitrary(-1, 1), getRandomArbitrary(-1, 1)));
        this.rotationDirection = getRndInteger(0, 1) === 0 ? -1 : 1;

        this.programInfo = programInfo;
        this.pointsArray = pointsCube;
        this.colorsArray = color;
        this.uniforms = {
            u_colorMult: [0.5, 0.5, 0.5, 1.0],
            u_model: new mat4(),
        };
        this.primType = "triangles";
        this.uniforms.u_model = translate(init_pos[0], init_pos[1], init_pos[2]);
    }
}

//----------------------------------------------------------------------------
// Initialization function
//----------------------------------------------------------------------------

window.onload = function init() {

    // Set up a WebGL Rendering Context in an HTML5 Canvas
    canvas = document.getElementById("gl-canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    if (!gl) {
        alert("WebGL isn't available");
    }

    //  Configure WebGL
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST);

    for (let i = 0; i < 30; i++) {
        objectsToDraw.push(new Cube(vec3(getRandomArbitrary(-5, 5), getRandomArbitrary(-5, 5), getRandomArbitrary(-5, 5)), generateColors()));
    }

    setPrimitive(objectsToDraw);

    // Set up a WebGL program
    // Load shaders and initialize attribute buffers
    program = initShaders(gl, "vertex-shader", "fragment-shader");

    // Save the attribute and uniform locations
    uLocations.model = gl.getUniformLocation(program, "model");
    uLocations.view = gl.getUniformLocation(program, "view");
    uLocations.projection = gl.getUniformLocation(program, "projection");
    uLocations.colorMult = gl.getUniformLocation(program, "colorMult");
    aLocations.vPosition = gl.getAttribLocation(program, "vPosition");
    aLocations.vColor = gl.getAttribLocation(program, "vColor");

    programInfo.uniformLocations = uLocations;
    programInfo.attribLocations = aLocations;
    programInfo.program = program;

    gl.useProgram(programInfo.program);

    // Set up viewport
    gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);

    // Set up camera
    // Projection matrix
    projection = perspective(45.0, canvas.width / canvas.height, 0.1, 100.0);
    gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, projection); // copy projection to uniform value in shader
    // View matrix (static cam)
    eye = vec3(-5.0, 5.0, 10.0);
    target = vec3(0.0, 0.0, 0.0);
    up = vec3(0.0, 1.0, 0.0);
    view = lookAt(eye, target, up);
    gl.uniformMatrix4fv(programInfo.uniformLocations.view, gl.FALSE, view); // copy view to uniform value in shader

    requestAnimFrame(render)
};

//----------------------------------------------------------------------------
// Rendering Event Function
//----------------------------------------------------------------------------

function render() {

    gl.clear(gl.DEPTH_BUFFER_BIT | gl.COLOR_BUFFER_BIT);

    //----------------------------------------------------------------------------
    // MOVE STUFF AROUND
    //----------------------------------------------------------------------------

    let ejeY = vec3(0.0, 1.0, 0.0);
    let R = rotate(rotAngle, ejeY);

    objectsToDraw[2].uniforms.u_model = translate(1.0, 1.0, 3.0);
    objectsToDraw[2].uniforms.u_model = mult(objectsToDraw[2].uniforms.u_model, R);

    objectsToDraw[3].uniforms.u_model = translate(1.0, 0.0, 3.0);
    objectsToDraw[3].uniforms.u_model = mult(R, objectsToDraw[3].uniforms.u_model);


    for (let i = 4; i < objectsToDraw.length; i++) {
        objectsToDraw[i].uniforms.u_model = mult(rotate(rotChange * objectsToDraw[i].rotationDirection,
            objectsToDraw[i].rotationAxis), objectsToDraw[i].uniforms.u_model);
    }

    //----------------------------------------------------------------------------
    // DRAW
    //----------------------------------------------------------------------------

    objectsToDraw.forEach(function (object) {


        gl.useProgram(object.programInfo.program);

        // Setup buffers and attributes
        setBuffersAndAttributes(object.programInfo, object.pointsArray, object.colorsArray);

        // Set the uniforms
        setUniforms(object.programInfo, object.uniforms);

        // Draw
        gl.drawArrays(object.primitive, 0, object.pointsArray.length);
    });

    rotAngle += rotChange;

    requestAnimationFrame(render);

}

//----------------------------------------------------------------------------
// Utils functions
//----------------------------------------------------------------------------

function setPrimitive(objectsToDraw) {

    objectsToDraw.forEach(function (object) {
        switch (object.primType) {
            case "lines":
                object.primitive = gl.LINES;
                break;
            case "line_strip":
                object.primitive = gl.LINE_STRIP;
                break;
            case "triangles":
                object.primitive = gl.TRIANGLES;
                break;
            default:
                object.primitive = gl.TRIANGLES;
        }
    });
}

function setUniforms(pInfo, uniforms) {
    // Copy uniform model values to corresponding values in shaders
    gl.uniform4f(pInfo.uniformLocations.colorMult, uniforms.u_colorMult[0], uniforms.u_colorMult[1], uniforms.u_colorMult[2], uniforms.u_colorMult[3]);
    gl.uniformMatrix4fv(pInfo.uniformLocations.model, gl.FALSE, uniforms.u_model);
}

function setBuffersAndAttributes(pInfo, ptsArray, colArray) {
    // Load the data into GPU data buffers
    // Vertices
    var vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(ptsArray), gl.STATIC_DRAW);
    gl.vertexAttribPointer(pInfo.attribLocations.vPosition, 4, gl.FLOAT, gl.FALSE, 0, 0);
    gl.enableVertexAttribArray(pInfo.attribLocations.vPosition);

    // Colors
    var colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(colArray), gl.STATIC_DRAW);
    gl.vertexAttribPointer(pInfo.attribLocations.vColor, 4, gl.FLOAT, gl.FALSE, 0, 0);
    gl.enableVertexAttribArray(pInfo.attribLocations.vColor);
}
