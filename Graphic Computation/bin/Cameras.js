'use strict';

function createCamera() {   // Default camera

    var delta1 = ((CAMERA_HEIGHT) * (window.innerWidth/window.innerHeight) - (CAMERA_WIDTH)) / 2;
    var delta2 = ((CAMERA_WIDTH) / (window.innerWidth/window.innerHeight) - (CAMERA_HEIGHT)) / 2;

    if ((CAMERA_WIDTH/CAMERA_HEIGHT) < (window.innerWidth/window.innerHeight))
        camera = new THREE.OrthographicCamera( (CAMERA_WIDTH / -2) - delta1, (CAMERA_WIDTH / 2) + delta1, CAMERA_HEIGHT / 2, CAMERA_HEIGHT / -2, 1 , CAMERA_WIDTH);
    else
        camera = new THREE.OrthographicCamera( CAMERA_WIDTH / -2, CAMERA_WIDTH / 2, (CAMERA_HEIGHT / 2) + delta2, (CAMERA_HEIGHT / -2) - delta2, 1 , CAMERA_WIDTH);

    camera.position.x = 0;
    camera.position.y = 800;
    camera.position.z = 0;

    camera.lookAt(new THREE.Vector3(0,0,0));
    camera.updateProjectionMatrix();
}


function onResize() {   //Permite mudar o tamanho da camera em tempo de acordo com a janela
    var delta,h = window.innerHeight,w = window.innerWidth;
    renderer.setSize(window.innerWidth, window.innerHeight);

    if (whichCamera == 1) {
        renderer.setViewport(0,0,window.innerWidth,window.innerHeight);
        if ((CAMERA_WIDTH/CAMERA_HEIGHT) < (window.innerWidth/window.innerHeight)) {
            delta = ((CAMERA_HEIGHT) * (window.innerWidth/window.innerHeight) - (CAMERA_WIDTH)) / 2;
            camera.left = (CAMERA_WIDTH / -2) - delta;
            camera.right = (CAMERA_WIDTH / 2) + delta;
            camera.top = CAMERA_HEIGHT / 2;
            camera.bottom = CAMERA_HEIGHT / -2;

        }
        else {
            delta = ((CAMERA_WIDTH) / (window.innerWidth/window.innerHeight) - (CAMERA_HEIGHT)) / 2;
            camera.left = CAMERA_WIDTH / -2;
            camera.right = CAMERA_WIDTH / 2;
            camera.top = (CAMERA_HEIGHT / 2) + delta;
            camera.bottom = (CAMERA_HEIGHT / -2) - delta;
        }
    }
    else {
        if (window.innerWidth > window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight))))
            w = window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight)));

        if (window.innerHeight > window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight))))
            h = window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight)));

        renderer.setViewport((window.innerWidth - w)/2 ,(window.innerHeight - h)/2 ,w, h);
    }

    if (1 < (window.innerWidth/window.innerHeight)) {
        delta = ((SCORE_SQ_SIZE) * ((0.1*window.innerWidth)/(0.1*window.innerHeight)) - (SCORE_SQ_SIZE)) / 2;

        cameraShips.left = (SCORE_SQ_SIZE / -2) - delta;
        cameraShips.right = (SCORE_SQ_SIZE / 2) + delta;
        cameraShips.top = SCORE_SQ_SIZE / -2;
        cameraShips.bottom = SCORE_SQ_SIZE / 2;

        cameraAliens.left = (SCORE_SQ_SIZE / -2) - delta;
        cameraAliens.right = (SCORE_SQ_SIZE / 2) + delta;
        cameraAliens.top = SCORE_SQ_SIZE / -2;
        cameraAliens.bottom = SCORE_SQ_SIZE / 2;

    } else {
        delta = ((SCORE_SQ_SIZE) / ((0.1*window.innerWidth)/(0.1*window.innerHeight)) - (SCORE_SQ_SIZE)) / 2;

        cameraShips.left = SCORE_SQ_SIZE / -2;
        cameraShips.right = SCORE_SQ_SIZE / 2;
        cameraShips.top = (SCORE_SQ_SIZE / -2) - delta;
        cameraShips.bottom = (SCORE_SQ_SIZE / 2) + delta;

        cameraAliens.left = SCORE_SQ_SIZE / -2;
        cameraAliens.right = SCORE_SQ_SIZE / 2;
        cameraAliens.top = (SCORE_SQ_SIZE / -2) - delta;
        cameraAliens.bottom = (SCORE_SQ_SIZE / 2) + delta;
    }

    if ((MSG_X/MSG_Y) < (window.innerWidth/ (window.innerHeight / 2))) {
        delta = ((MSG_Y) * ((window.innerWidth/2)/(window.innerHeight/4)) - (MSG_X)) / 2;

        cameraMessage.left = (MSG_X / -2) - delta;
        cameraMessage.right = (MSG_X / 2) + delta;
        cameraMessage.top = MSG_Y / -2;
        cameraMessage.bottom = MSG_Y / 2;
    }
    else {
        delta = ((MSG_X) / ((window.innerWidth/2)/(window.innerHeight/4)) - (MSG_Y)) / 2;

        cameraMessage.left = MSG_X / -2;
        cameraMessage.right = MSG_X / 2;
        cameraMessage.top = (MSG_Y / -2) - delta;
        cameraMessage.bottom = (MSG_Y / 2) + delta;
    }

    camera.updateProjectionMatrix();
    cameraShips.updateProjectionMatrix();
    cameraAliens.updateProjectionMatrix();
    cameraMessage.updateProjectionMatrix();
}


function updateCameraSpace() {    //Permite trocar entre os 4 tipos de camera usados

    switch (whichCamera) {

        case 3: //camera que segue a nave por tras

            behindPerspectiveFollowCamera();
            break;

        default:
            return;

    }

}

function classicOrthogonalCamera() {
    whichCamera = 1;
    var delta1 = ((CAMERA_HEIGHT) * (window.innerWidth/window.innerHeight) - (CAMERA_WIDTH)) / 2;
    var delta2 = ((CAMERA_WIDTH) / (window.innerWidth/window.innerHeight) - (CAMERA_HEIGHT)) / 2;

    if ((CAMERA_WIDTH/CAMERA_HEIGHT) < (window.innerWidth/window.innerHeight)) {
        camera = new THREE.OrthographicCamera( (CAMERA_WIDTH / -2) - delta1, (CAMERA_WIDTH / 2) + delta1, CAMERA_HEIGHT / 2, CAMERA_HEIGHT / -2, 1 , CAMERA_WIDTH);
    }
    else {
        camera = new THREE.OrthographicCamera( CAMERA_WIDTH / -2, CAMERA_WIDTH / 2, (CAMERA_HEIGHT / 2) + delta2, (CAMERA_HEIGHT / -2) - delta2, 1 , CAMERA_WIDTH);
    }
    renderer.setViewport(0,0,window.innerWidth,window.innerHeight);
    controls = new THREE.OrbitControls( camera );

    camera.position.x = 0;
    camera.position.y = 800;
    camera.position.z = 0;

    camera.lookAt(new THREE.Vector3(0,0,0));
    camera.updateProjectionMatrix();
    if (BACKGROUND == 2) bGScreen.position.set(0,0,0);
}

function behindPerspectiveCamera() {
    whichCamera = 2;
    var delta1 = ((CAMERA_HEIGHT) * (window.innerWidth/window.innerHeight) - (CAMERA_WIDTH)) / 2;
    var delta2 = ((CAMERA_WIDTH) / (window.innerWidth/window.innerHeight) - (CAMERA_HEIGHT)) / 2;
    var w = window.innerWidth;
    var h = window.innerHeight;

    if (window.innerWidth > window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight))))
        w = window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight)));
    if (window.innerHeight > window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight))))
        h = window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight)));
    renderer.setViewport((window.innerWidth - w)/2 ,(window.innerHeight - h)/2 ,w, h);
    camera = new THREE.PerspectiveCamera(90, screen.width/screen.height, 1 , CAMERA_WIDTH);

    controls = new THREE.OrbitControls( camera );


    camera.position.x = 0;
    camera.position.y = 350;
    camera.position.z = 800;

    camera.lookAt(new THREE.Vector3(0,0,0));
    if (BACKGROUND == 2) bGScreen.position.set(0,0,0);
}

function behindPerspectiveFollowCamera() {

    camera.position.x = spaceship.position.x;

    camera.lookAt(spaceship.position);
    if (BACKGROUND == 2) bGScreen.movement();
}
