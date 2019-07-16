'use strict';

var spaceshipControls = [false, false]  // Necessario lembrar que teclas estao pressioandas

function onKeyPress(e) {
    if(time.running && e.which != 83) {
        switch (e.which) {
            case 66: // b
            case 98:
                if (SPACESHIP_FIRE == false) {
                    spaceship.fire();
                    SPACESHIP_FIRE = true;
                }
        }
    }
}

function onKeyDown(e) {     //Controlos da nave, dos wireframes e da camera
    var delta1 = ((CAMERA_HEIGHT) * (window.innerWidth/window.innerHeight) - (CAMERA_WIDTH)) / 2;
    var delta2 = ((CAMERA_WIDTH) / (window.innerWidth/window.innerHeight) - (CAMERA_HEIGHT)) / 2;
    var w = window.innerWidth;
    var h = window.innerHeight;

    if(e.which == 83) { //s
        if (time.running && !(gScreen.visible == true && gScreen.userData.txt != 'PAUSE')) {
          gScreen.changeTxt('PAUSE');
          gScreen.visible = true;
          time.running = !time.running;
        }
        else if (!time.running && !(gScreen.visible == true && gScreen.userData.txt != 'PAUSE')) {
          gScreen.changeTxt('GAME_OVER');
          gScreen.visible = false;
          time.running = !time.running;
        }
      }
      else if (e.which == 82){  //r
        resetAux();
        classicOrthogonalCamera();
        gScreen.changeTxt('GAME_OVER');
        gScreen.visible = false;
        startSpawnSpaceship(0, 0, 450);
        startSpawnAliens(ALIEN_LINES,ALIEN_COLUMNS);
        startSpawnShields(SHIELD_LINES,SHIELD_COLUMNS);
        startSpawnProjectiles(BULLET_MAX, MISSILE_MAX);
        if(SHOW_BB)
            startSpawnBBs();
        time.running = true;
      }
    if(e.which != 83) {
        switch (e.which) {

        	case 37: // left arrow
                spaceshipControls[0] = true;
        		spaceship.userData.direction = 1;
        		break;

        	case 39: // right arrow
                spaceshipControls[1] = true;
        		spaceship.userData.direction = 2;
        		break;

    // ***************************** CAMERAS *********************************** //

            case 49: // 1
                classicOrthogonalCamera();
                gScreen.changeTxt(gScreen.userData.txt);
                break;

            case 50: // 2
                behindPerspectiveCamera();
                gScreen.changeTxt(gScreen.userData.txt);
                break;

            case 51: // 3
                whichCamera = 3;
                if (window.innerWidth > window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight))))
                    w = window.innerHeight*(screen.width/(screen.height-(window.outerHeight-window.innerHeight)));
                if (window.innerHeight > window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight))))
                    h = window.innerWidth / (screen.width/(screen.height-(window.outerHeight-window.innerHeight)));
                renderer.setViewport((window.innerWidth - w)/2 ,(window.innerHeight - h)/2 ,w, h);
                camera = new THREE.PerspectiveCamera(90, screen.width/screen.height, 1 , CAMERA_WIDTH);
                controls.enabled = false;

                camera.position.y = 100;
                camera.position.z = 600;

                gScreen.changeTxt(gScreen.userData.txt);
                break;


    // *****************************  *********************************** //

            case 65: // a
            case 97: // numpad1
                for (var i = 0; i < list_ALL.length; i++)
            	     wireframize(list_ALL[i]);
                wireframes = !wireframes;
                break;

            case 72: // h
            if (enable_lighting)
                spotLight.visible = !spotLight.visible;
            break;

            case 67: // C
            case 99:
                switchPointlights(1);
                break;

            case 71: // G
            case 103:
                switchShaders();
                break;

            case 76: // l
            case 108:
                switchLighting();
                break;

            case 78: // N
            case 110:
                switchDayNight(1);
                break;
        }
    }
}

function onKeyUp(e) {   //Tecla libertada

    if (e.which == 37)
        spaceshipControls[0] = false;   // Esquerda nao pressionado
    if (e.which == 39)
        spaceshipControls[1] = false;   // Direita nao pressionado
    if(e.which == 66 || e.which == 98)
        SPACESHIP_FIRE = false;
    if (spaceshipControls[0] == false && spaceshipControls[1] == false) // Se nenhuma direccao, esta iddle
        spaceship.userData.direction = 0;
}


function leftArrow() {  //Move a nave para a esqurda quando a seta esqurda e premida

	var accel = SPACESHIP_SPEED*Math.pow(Math.abs(spaceship.rotation.z),1.5);

    if (spaceship.rotation.z >= -Math.abs(SPACESHIP_ROT_MAX)) // Se nao esta com a max rotacao
        spaceship.userData.rotation = -Math.PI/100;                 //  entao aumenta a rotacao

    (spaceship.rotation.z < 0)  ? (
        spaceship.userData.speedX = -accel                       //  Se a rotacao for para a esquerda, vai para a esquerda
    ) : (
        spaceship.userData.rotation = -Math.PI/50,                // Se esta a ir na outra direccao, muda rapidamente para esta
        spaceship.userData.speedX = accel
    )
}

function rightArrow() { //Move a nave para a direita quando a seta direita e premida

	var accel = SPACESHIP_SPEED*Math.pow(Math.abs(spaceship.rotation.z),1.5);

    if (spaceship.rotation.z <= Math.abs(SPACESHIP_ROT_MAX))
        spaceship.userData.rotation = Math.PI/100;

    (spaceship.rotation.z > 0) ? (
        spaceship.userData.speedX = accel
    ) : (
        spaceship.userData.rotation = Math.PI/50,
        spaceship.userData.speedX = -accel
    )
}

function iddle() {  //Retorna a nave a sua rotaçao original

	var accel = SPACESHIP_SPEED*Math.pow(Math.abs(spaceship.rotation.z),2);

    if (accel < SPACESHIP_ROT_IDDLE) {
        spaceship.userData.speedX = 0;
        spaceship.userData.rotation = -spaceship.rotation.z;
        return;
    }

    (spaceship.rotation.z > 0) ? (
        spaceship.userData.speedX = accel,
        spaceship.userData.rotation = -SPACESHIP_ROT_IDDLE
    ) : (
        spaceship.userData.speedX = -accel,
        spaceship.userData.rotation = SPACESHIP_ROT_IDDLE
    )

}


function switchPointlights(number) {
    if (enable_lighting) {
        for (var i in list_pointlights) {
                list_pointlights[i].visible = !list_pointlights[i].visible;
        }
        if (number == 1) // Clicou em switch pointlights
            pointlight_status = !pointlight_status;
    }
}

function switchDayNight(number) {
    if (enable_lighting) {
        (directional_light.visible) ? directional_light.visible = false : directional_light.visible = true;   // Trocar dia/noite
        if (number == 1)
          daytime = !daytime;
    }
}


function switchLighting() {
    if (enable_lighting) {      // Se esta ligado, desligar
        if (pointlight_status)  // Saber o estado das pointlights antes de desligar a luz
            switchPointlights(2);   // Envia "2" para nao alterar o estado das pointlights
        if (daytime)
          switchDayNight(2);
        if (ENABLE_AMBIENT_LIGHT)
          ambient_light.visible = false;
        enable_lighting = false;
    }

    else {
        enable_lighting = true;
        if  (pointlight_status)
            switchPointlights(2);
        if (daytime)
          switchDayNight(2);
          if (ENABLE_AMBIENT_LIGHT)
            ambient_light.visible = true;
    }

    for (var i = 0; i < list_ALL.length; i++)
        list_ALL[i].baseMat();

}

function switchShaders() {

    for (var i = 0; i < list_ALL.length; i++)
        list_ALL[i].changeMats();


}

function resetAux() {
  list_aliens = [];
  list_shields = [];
  list_bullets = [];
  list_missiles = [];

  for (var i = 0; i < list_objects.length; i++) {
      scene.remove(list_objects[i].userData.bbhelper);
      scene.remove(list_objects[i]);
  }
  list_objects = [];
  scene.remove(spotter);
  scene.remove(spotLight);
}
