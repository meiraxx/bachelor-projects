// CG 2016/2017 Primeiro Semestre
// Space Invaders - Entrega #4
// 81926 Valter Santos
// 82014 Joao Meira
// 82085 Andre Madeira
// NOTA: E possivel alterar o tipo de background.
// Skybox produz o melhor tipo de background.
// Para alterar: Linha 20, mudar valor para 2.

'use strict';
/** DEBUG **/
var SHOW_BB = 0;
var SHOW_SUN = 0;
var SHOW_POINTLIGHTS = 1;
var ALIENS_MOVE = 1;
var ENABLE_AMBIENT_LIGHT = 1;
var SHOW_SPOTLIGHT = 0;

/** SPAWNING SETTINGS **/
var BACKGROUND = 1; // 1 - Plane | 2 - Skybox
var ALIEN_LINES = 4;	// Default : 2
var ALIEN_COLUMNS = 4;	// Default : 4
var SHIELD_LINES = 1;
var SHIELD_COLUMNS = 4;
var FIELDWIDTH = 1800;
var FIELDHEIGHT = 1750;
var CAMERA_HEIGHT = 2000;
var CAMERA_WIDTH = 3000;
var BULLET_MAX = 40;
var MISSILE_MAX = 2;
var SPACE_BETWEEN_ALIENS_X = 125;
var SPACE_BETWEEN_ALIENS_Z = 125;
var VISIBLE_BONDS = true;
var MSG_X = screen.width / 4;
var MSG_Y = screen.height / 8;

var SCORE_SQ_SIZE = 100; // Mais pequeno, maior
var SCORE_DISTANCES_X = 70;
var SCR_DST_Y = 70;
var SCORE_DISTANCES_Y = 50;
var SCORE_MAX = 20;

/** MATERIALS **/
var ALIEN_MAT_COLOR = 0x2005f1;
var ALIEN_MAT_SPECULAR = 0xffffff;
var ALIEN_MAT_SHININESS = 40;

var SPACESHIP_MAT_SPECULAR = 0xffffff;
var SPACESHIP_MAT_SHININESS = 40;

var SHIELD_MAT_COLOR = 0x4f0606;
var SHIELD_MAT_SPECULAR = 0xffffff;
var SHIELD_MAT_SHININESS = 40;

var WALL_MAT_COLOR = 0xffffff;
var WALL_MAT_SPECULAR = 0xffffff;
var WALL_MAT_SHININESS = 40;

var BULLET_MAT_COLOR = 0xffd375;
var BULLET_MAT_SPECULAR = 0xffffff;
var BULLET_MAT_SHININESS = 40;

var MISSILE_MAT_COLOR = 0xfc0000;
var MISSILE_MAT_SPECULAR = 0xffffff;
var MISSILE_MAT_SHININESS = 40;


/** Lights **/
var AMB_LIGHT_COLOR = 0xbbbbbb;
var AMB_LIGHT_INTENSITY = 0.2;

var DIR_LIGHT_POS = new THREE.Vector3(0, 1600, -800);
var DIR_LIGHT_COLOR = 0xfeffb1;
var DIR_LIGHT_INTENSITY = 5;

var POINTLIGHT_LINES = 3;
var POINTLIGHT_COLUMNS = 2;
var POINTLIGHT_COLOUR = 0xffeb0a;
var POINTLIGHT_INTENSITY = 30;
var POINTLIGHT_DISTANCE = 600;
var POINTLIGHT_DECAY = 2;
var POINTLIGHT_HEIGHT = 250;
var POINTLIGHT_EXTRA_X = 250;
var POINTLIGHT_EXTRA_Z = 100;

var SPOTLIGHT_COLOR = 0xffffff;
var SPOTLIGHT_INTENSITY = 30;
var SPOTLIGHT_DISTANCE = 3*FIELDHEIGHT/4;
var SPOTLIGHT_ANGLE = Math.PI / 6;
var SPOTLIGHT_PENUMBRA = 2;
var SPOTLIGHT_DECAY = 2;

/** MOVEMENT SETTINGS **/
var SPACESHIP_FIRE = false;
var SPACESHIP_ROT_MAX = Math.PI/5;
var SPACESHIP_ROT_IDDLE = Math.PI/350;
var SPACESHIP_SPEED = 50;
var ALIEN_SPEED = 40;
var BULLET_SPEED = 200;
var MISSILE_SPEED = 15;

/***  UNIT SPECS  **/
var ALIEN_HEALTH = 1;
var ALIEN_DAMAGE = 1;
var PLAYER_HEALTH = 3;
var PLAYER_DAMAGE = 1;
var SHIELD_HEALTH = 30;
var SHIELD_DAMAGE = 0;
var BULLET_HEALTH = 1;
var BULLET_DAMAGE = 1;
var MISSILE_HEALTH = 1;
var MISSILE_DAMAGE = 3;
/******************************/

var camera, cameraShips, cameraAliens, cameraMessage,
    scene, sceneShip, sceneAlien, sceneMessage, renderer, vector = new THREE.Vector3;
var mesh, geometry, material, wireframes = true;

var spaceship,          //Lista objects;
    spaceshipscore,
    alienscore,
    bGScreen,
    gScreen,
    ambient_light,
    spotLight,
    spotter,
    directional_light,
    list_aliens = [],
    list_shields = [],
    list_bullets = [],
    list_missiles = [],
    list_objects = [],
    list_world_obstacles = [],
    list_ALL = [], // objects + obstacles

    list_collisions = [],
    list_pointlights = [];

var whichCamera = 1, daytime = true, enable_lighting = true, pointlight_status = true;
var time = new THREE.Clock(), globaltime, controls;

function render() {
    renderer.clear();

    if (whichCamera == 1)
        renderer.setViewport(0,0,window.innerWidth,window.innerHeight);
    else {
        var delta,h = window.innerHeight,w = window.innerWidth;
        if (window.innerWidth > window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight)))) {
            w = window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight)));
        }
        if (window.innerHeight > window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight)))) {
            h = window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight)));
        }
        renderer.setViewport((window.innerWidth - w)/2 ,(window.innerHeight - h)/2 ,w, h);
    }

    renderer.render(scene, camera);
    var max = (spaceship.userData.health < SCORE_MAX ) ? spaceship.userData.health : SCORE_MAX;

    for (var i = 0; i < max; i++) {
        renderer.clearDepth();
        renderer.setViewport(SCORE_DISTANCES_X*i*(window.innerWidth/screen.width),((SCR_DST_Y+SCORE_DISTANCES_Y)/1000*window.innerHeight)-(60*(window.innerHeight/screen.height)),(0.1*window.innerWidth), (0.1*window.innerHeight));
        renderer.render(sceneShip, cameraShips);
    }

    var aliencount = (ALIEN_COLUMNS*ALIEN_LINES) - spaceship.userData.score;
    max = (aliencount < SCORE_MAX ) ? aliencount : SCORE_MAX;

    for (i = 0; i < max; i++) {
        renderer.clearDepth();
        renderer.setViewport(SCORE_DISTANCES_X*i*(window.innerWidth/screen.width), (SCR_DST_Y/1000*window.innerHeight)-(60*(window.innerHeight/screen.height)), (0.1*window.innerWidth), (0.1*window.innerHeight));
        renderer.render(sceneAlien, cameraAliens);
    }

    renderer.clearDepth();
    renderer.setViewport(window.innerWidth/4, window.innerHeight*(3/8), window.innerWidth/2, window.innerHeight/4);
    renderer.render(sceneMessage, cameraMessage);
}

function animate() {    //Anima o movimento da nave, atualiza a camera e inicia a renderizaçao

    globaltime = 0;
    if(time.running) {
        globaltime = time.getDelta();
        if (globaltime > 0.000007)   // Prevent frame skips
            globaltime = 0.000007;
    }


    /** UpdatePositions.js **/
        spaceshipNextPosition();

    	updatePositions();

        if (SHOW_BB)
            updateBBs();
    /*************************/

        updateCameraSpace();            // Cameras.js
        render();
        requestAnimationFrame(animate);
}

function init() {

    renderer = new THREE.WebGLRenderer({antialias: true});

    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.autoClear = false;
    document.body.appendChild(renderer.domElement);

    createScene();  // CreateObjects.js
    createCamera(); // Cameras.js
    time.start();

    controls = new THREE.OrbitControls( camera );   //controls/OrbitControls.js
    controls.addEventListener( 'change', render );

    render();

    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("keypress", onKeyPress);

}
