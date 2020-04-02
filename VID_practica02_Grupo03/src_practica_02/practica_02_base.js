/*
 * 
 * Practica_02_base.js
 * Videojuegos (30262) - Curso 2019-2020
 * 
 * Parte adaptada de: Alex Clarke, 2016, y Ed Angel, 2015.
 *
 *Autores: Víctor.M Lafuente,Alonso Muñoz,José M. Romero
 *NIAs:747325,745016,740914
 * 
 * 
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
    red, red, //y
    blue, blue, //z
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

var model = new mat4(); // create a model matrix and set it to the identity matrix
var view = new mat4(); // create a view matrix and set it to the identity matrix
var projection = new mat4(); // create a projection matrix and set it to the identity matrix

// Camera:
var eye, target, up; // for view matrix
var perspectiveMode = true;
var fovy = 45.0;

var CAM_ROTATION_SPEED = 1.0;

var lastPostion = undefined;
var clicked = false;

// Cubes:
var rotAngle = 0.0;
var rotChange = 0.5;

var NUM_P2_ORBITING_CUBES = 30;

var program;
var uLocations = {};
var aLocations = {};

var programInfo = {
    program,
    uniformLocations: {},
    attribLocations: {},
};

var objectsToDraw = [{
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

/**
 * Moves the camera to a relative position from the camera.
 * @param vec Relative position from the camera to move.
 */
function linealCameraMove(vec) {
    let x = subtract(target, eye);
    let l = cross(x, up);

    let mCB = createChangeOfBaseMatrix(x, up, l, eye);

    let tempEye = mult(mCB, vec);

    eye = vec3(tempEye[0], tempEye[1], tempEye[2]);
    target = add(eye, x);
}

/**
 * Calculates the module of vector
 * @param v Vector
 * @returns {number}
 */
function module(v) {
    val = 0;
    v.forEach(element => {
        val = val + (element * element)
    });
    return Math.sqrt(val)
}

function updateCamera() {
    view = lookAt(eye, target, up);
    gl.uniformMatrix4fv(programInfo.uniformLocations.view, gl.FALSE, view); // copy view to uniform value in shader
}

/**
 * Change to Perspective cam mode whit field of view.
 */
function setPerspectiveCam() {
    perspectiveMode = true;
    //( fovy, aspect, near, far )
    projection = perspective(fovy, canvas.width / canvas.height, 0.1, 100.0);
    gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, projection); // copy projection to uniform value in shader
}

/**
 * Change to Orthogonal perspective camera mode.
 */
function setOrthoCam() {
    //( left, right, bottom, top, near, far )
    perspectiveMode = false;
    prop = canvas.height / canvas.width;
    ortograph = ortho(-10, 10, -10 * prop, 10 * prop, -10, 1000);

    //ortograph = ortho(-50, 50, -50 * prop, 50 * prop, dis / 10, dis);
    gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, ortograph); // copy projection to uniform value in shader
}


let vectTranslate = vec4(0,0,0,1);

window.addEventListener('keydown', function(event) {
    event.preventDefault();
    const keyName = event.key;
    this.console.log("pulsado " + event.keyCode)

    if ((event.keyCode === 107 || event.keyCode === 171) && (persepectiva)) {
        //+
        fovy++;
        if (fovy > 179) {
            fovy = 179;
        }
        setPerspectiveCam();
    } else if ((event.keyCode === 109 || event.keyCode === 173) && (persepectiva)) {
        //-
        fovy--;
        if (fovy < 1) {
            fovy = 1;
        }
        setPerspectiveCam();
    }
    if (event.code === "KeyW") {
        console.log("UP");
        vectTranslate[0] = 1;
    }
    if (event.code === "KeyA") {

        //LEFT
        console.log("LEFT");
        vectTranslate[2] = -1;

    }
    if (event.code === "KeyS") {
        console.log("DOWN");
        //DOWN
        vectTranslate[0] = -1;
    }
    if (event.code === "KeyD") {
        console.log("RIGHT");
        vectTranslate[2] = 1;
    }
    if (event.code === "KeyO") {
        //O
        setOrthoCam();
    }
    if (event.code === "KeyP") {
        //P
        setPerspectiveCam();
    }
    updateCamera()
});


window.addEventListener('keyup', function(event) {
    event.preventDefault();

    if (event.code === "KeyW") {
        console.log("UP");
        vectTranslate[0] = 0;
    }
    if (event.code === "KeyA") {

        //LEFT
        console.log("LEFT");
        vectTranslate[2] = 0;

    }
    if (event.code === "KeyS") {
        console.log("DOWN");
        //DOWN
        vectTranslate[0] = 0;
    }
    if (event.code === "KeyD") {
        console.log("RIGHT");
        vectTranslate[2] = 0;
    }
//    updateCamera()
});

window.addEventListener("mousedown", function (event) {
    event.preventDefault();
    if (event.button === 0) {
        clicked = true;
        lastPostion = [event.clientX, event.clientY];
    }
});
window.addEventListener("mouseup", function (event) {
    event.preventDefault();
    if (event.button === 0) {
        clicked = false;
    }
});

window.addEventListener("mousemove", function (event) {
    event.preventDefault();
    if (!clicked) {
        return;
    }
    let position = [event.clientX, event.clientY];
    let directionMov = [position[0] - lastPostion[0], position[1] - lastPostion[1]];

    lastPostion = position;
    console.log("(X,Y): " + directionMov[0] + ", " + directionMov[1]);

    // Save last position if user makes more movements
    lastPostion = position;

    // Move target to center position to rotate:
    let x = subtract(target, eye);
    let distance = module(x);
    let targetInOrigin = vec4(distance, 0, 0, 1);

    // Rotate target as user input says:
    let rotationAxis = normalize(cross(normalize(vec3(targetInOrigin[0], targetInOrigin[1], targetInOrigin[2])),
             normalize(vec3(0, directionMov[1], -directionMov[0]))));
    targetInOrigin = mult(rotate(CAM_ROTATION_SPEED, rotationAxis), targetInOrigin);

    // Return target to its position with a change of base:
    let l = normalize(cross(x, up));
    let m = createChangeOfBaseMatrix(x, up, l, eye);

    let tempTarget = mult(m, targetInOrigin);
    target = vec3(tempTarget[0], tempTarget[1], tempTarget[2]);
    
    updateCamera();
});


/**
 * Creates a change of base matrix transformation.
 * @param xAxis
 * @param yAxis
 * @param zAxis
 * @param originPoint
 * @returns {[]} Change of Base Matrix
 */
function createChangeOfBaseMatrix(xAxis, yAxis, zAxis, originPoint) {
    xAxis = normalize(xAxis);
    yAxis = normalize(yAxis);
    zAxis = normalize(zAxis);

    let mCB = mat4();
    for (let pCB = 0; pCB < 3; pCB++) {
        mCB[4 * pCB]     =  xAxis[pCB];
        mCB[4 * pCB + 1] =  yAxis[pCB];
        mCB[4 * pCB + 2] =  zAxis[pCB];
        mCB[4 * pCB + 3] =  originPoint[pCB];
    }

    return mCB;
}

/**
 * Generates a list of one color for each triangle from a cube. The color is generated randomly.
 * @returns {[]|*[]}
 */
function generateColors() {
    color = [Math.random(), Math.random(), Math.random(), 1];
    colorCube = [];
    for (i = 0; i < 6 * 6; i++) {
        colorCube.push(color);
    }
    return colorCube;
}

/**
 * Returns a random float value with a uniform distribution between min and max.
 * @param min Float. Minimum return value.
 * @param max Float. Maximum return value.
 * @returns Random float value.
 */
function getRndFloat(min, max) {
    return Math.random() * (max - min) + min;
}

/**
 * Returns a random integer with a uniform distribution between min and max (included).
 * @param min Integer. Minimum return value.
 * @param max Integer. Maximum return value.
 * @returns Random int value.
 */
function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

class Cube {
    constructor(init_pos, color) {
        // Rotation around itself, creates a random axis to rotate:
        this.itselfRotationAxis = vec3(getRndFloat(-1, 1), getRndFloat(-1, 1), getRndFloat(-1, 1));
        // Rotation around center, creates a perpendicular axis that goes through the center (0,0,0).
        this.rotationAxis = cross(init_pos, vec3(getRndFloat(-1, 1), getRndFloat(-1, 1), getRndFloat(-1, 1)));
        // Set random rotation dir (clockwise or counterclockwise).
        this.rotationDirection = getRndFloat(-2, 2);
        this.rotationDirectionThemselves = getRndFloat(-5, 5);

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

    // Add the orbiting cubes to the list of objects:
    for (let i = 0; i < NUM_P2_ORBITING_CUBES; i++) {
        objectsToDraw.push(new Cube(vec3(getRndFloat(-5, 5), getRndFloat(-5, 5), getRndFloat(-5, 5)), generateColors()));
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
    // Translate camera is needed:
    linealCameraMove(vectTranslate);
    updateCamera();

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

    // For the rest objects in the list:
    for (let i = 4; i < objectsToDraw.length; i++) {
        // Rotate then around themselves:
        objectsToDraw[i].uniforms.u_model = mult(objectsToDraw[i].uniforms.u_model,
            rotate(objectsToDraw[i].rotationDirectionThemselves,
                objectsToDraw[i].itselfRotationAxis));

        // Rotate them around the center position (0,0,0):
        objectsToDraw[i].uniforms.u_model = mult(rotate(objectsToDraw[i].rotationDirection,
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