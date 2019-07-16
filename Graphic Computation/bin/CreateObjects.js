'use strict';

function createScene() {

    scene = new THREE.Scene();

    bGScreen = createbackground_screen(0,0,0);
    scene.add(bGScreen);
    startSpawnSpaceship(0, 0, 450);
    startSpawnAliens(ALIEN_LINES,ALIEN_COLUMNS);
    startSpawnShields(SHIELD_LINES,SHIELD_COLUMNS);
    startSpawnBounds(FIELDWIDTH/2);
    startSpawnProjectiles(BULLET_MAX, MISSILE_MAX);
    startSpawnDirectionalLight();
    startSpawnPointLights(POINTLIGHT_LINES, POINTLIGHT_COLUMNS);
    startSpawnAmbientLight(ENABLE_AMBIENT_LIGHT);
    startScoreScene();
    if (SHOW_BB)
        startSpawnBBs();
}


function startSpawnSpaceship(x, y, z) {

    spaceship = createSpaceshipV(x, y, z);
    spaceship.rotation.y += Math.PI;
    list_ALL.push(spaceship);
    list_objects.push(spaceship);
    scene.add(spaceship);
}


function startSpawnAliens(lines, columns) {
    var model = new THREE.Box3().setFromObject( createAlienMinion(0, 0, 0) );
	var index = 0;

    var modelspaceX = model.getSize().x + SPACE_BETWEEN_ALIENS_X;    // Tamanho x + metade para espaco entre naves
    var totalspaceX = -(columns * modelspaceX) / 2 - modelspaceX/2; // Formula para comprimento do espaco das naves ao todo

    var modelspaceZ = model.getSize().z + SPACE_BETWEEN_ALIENS_Z;
    var totalspaceZ = -(lines * modelspaceZ) / 2 - modelspaceZ/2 - FIELDHEIGHT/5;

    for (var i = 0; i < lines; i++) {
        totalspaceX = -(columns * modelspaceX) / 2 - modelspaceX/2;
        totalspaceZ += modelspaceZ;
        for (var j = 0; j < columns; j++) {
            totalspaceX += modelspaceX;                                 // Vai incrementando onde nasce a nave
            list_aliens[index] = createAlienMinion(totalspaceX, 0, totalspaceZ);

            if (ALIENS_MOVE)
                list_aliens[index].generateMovement();

            list_ALL.push(list_aliens[index]);
			list_objects.push(list_aliens[index]);
            scene.add(list_aliens[index]);
			index++;
        }
    }
}


function startSpawnShields(lines, columns) {
    var model = new THREE.Box3().setFromObject( createShield(0, 0, 0) );
	var index = 0;

    var modelspaceX = 4.25*model.getSize().x;    // Tamanho x + metade para espaco entre escudos
    var totalspaceX = -(columns * modelspaceX) / 2 - modelspaceX/2;

    var distanceZ = 425;

    for (var i = 0; i < lines; i++) {
        totalspaceX = -(columns * modelspaceX) / 2 - modelspaceX/2;
        distanceZ += -225;
        for (var j = 0; j < columns; j++) {
            totalspaceX += modelspaceX;                                 // Vai incrementando onde nasce o escudo
            list_shields[index] = createShield(totalspaceX, 0, distanceZ);
            list_ALL.push(list_shields[index]);
			list_objects.push(list_shields[index]);
            scene.add(list_shields[index]);
			index++;
        }
    }
}

function startSpawnProjectiles(max_bullets, max_missiles) {
    var projectile = new THREE.Object3D();

    for (var i = 0; i < max_bullets; i++) {
        projectile = createBullet(0, 0, 0);
        list_ALL.push(projectile);
        list_bullets.push(projectile);
    }

    for (i = 0; i < max_missiles; i++) {
        projectile = createMissile(0, 0, 0);
        list_ALL.push(projectile);
        list_missiles.push(projectile);
    }
}


function startSpawnBounds(OutOfBonds) {
    var plane = createWall(-OutOfBonds, 0, 0, FIELDWIDTH, FIELDWIDTH/2);  //left
    plane.rotation.z = Math.PI/2;
    plane.makeBB();
    list_ALL.push(plane);
    list_world_obstacles.push(plane);
    scene.add( plane );

    plane = createWall(OutOfBonds, 0, 0, FIELDWIDTH, FIELDWIDTH/2);       //right
    plane.rotation.z = Math.PI / 2;
    plane.makeBB();
    list_ALL.push(plane);
    list_world_obstacles.push(plane);
    scene.add( plane );

    plane = createWall(0, 0, -FIELDHEIGHT/2 - 10, FIELDWIDTH, FIELDWIDTH/2);       //far
    plane.rotation.x = Math.PI/2;
    plane.rotation.y = Math.PI/2;
    plane.makeBB();
    list_ALL.push(plane);
    list_world_obstacles.push(plane);
    scene.add( plane );

    plane = createWall(0, 0, FIELDHEIGHT/2 + 10, FIELDWIDTH, FIELDWIDTH/2);       //near
    plane.rotation.x = Math.PI/2;
    plane.rotation.y = Math.PI/2;
    plane.makeBB();
    list_ALL.push(plane);
    list_world_obstacles.push(plane);
    scene.add( plane );

}

function startSpawnBBs() {
    for (var i in list_objects)
        scene.add(list_objects[i].userData.bbhelper);
}

function startSpawnDirectionalLight() {
    var sphere = new THREE.SphereGeometry( 100, 16, 8 );
    var temp_mat = new THREE.MeshBasicMaterial( { color: DIR_LIGHT_COLOR } );

    directional_light = new THREE.DirectionalLight( DIR_LIGHT_COLOR, DIR_LIGHT_INTENSITY );
    directional_light.position.copy(DIR_LIGHT_POS);
    if (SHOW_SUN)
        directional_light.add(new THREE.Mesh (sphere, temp_mat));
    scene.add(directional_light);
}

function startSpawnPointLights(lines, columns) {
    var width_start = -(FIELDWIDTH / 2) + (FIELDWIDTH / (columns + 1 )) - (columns-1)*POINTLIGHT_EXTRA_X/2;
    var height_start = -(FIELDHEIGHT / 2) + (FIELDHEIGHT / (lines + 1)) - (lines-1)*POINTLIGHT_EXTRA_Z/2;
    var sphere = new THREE.SphereGeometry( 15, 16, 8 );
    var temp_mat = new THREE.MeshBasicMaterial( { color: 0xffffff } );

    var index = 0;
    for (var i = 0; i < columns; i++) {
        for (var j = 0; j < lines; j++) {
            list_pointlights[index] = new THREE.PointLight( POINTLIGHT_COLOUR, POINTLIGHT_INTENSITY, POINTLIGHT_DISTANCE, POINTLIGHT_DECAY );
            list_pointlights[index].position.set( width_start, POINTLIGHT_HEIGHT, height_start );
            if (SHOW_POINTLIGHTS)
                list_pointlights[index].add( new THREE.Mesh(sphere, temp_mat ) );
            height_start += FIELDHEIGHT / (lines + 1) + POINTLIGHT_EXTRA_Z;
            scene.add( list_pointlights[index] );
            index++;
        }
        height_start = -(FIELDHEIGHT / 2) + (FIELDHEIGHT / (lines + 1)) - (lines-1)*POINTLIGHT_EXTRA_Z/2 // Reset
        width_start += FIELDWIDTH / (columns + 1) + POINTLIGHT_EXTRA_X;
    }
}

function startSpawnAmbientLight(value) {

  if (value) {
    ambient_light = new THREE.AmbientLight( AMB_LIGHT_COLOR, AMB_LIGHT_INTENSITY );
    scene.add(ambient_light);
  }

}

function startScoreScene() {

    sceneShip = new THREE.Scene() // Cena onde esta nave do score
    spaceshipscore = createSpaceshipV(0, 0, 0);
    spaceshipscore.scale.set(0.3,0.3,0.3);
    spaceshipscore.baseMat();
    sceneShip.add(spaceshipscore);

    sceneAlien = new THREE.Scene() // Cena onde esta alien do score
    alienscore = createAlienMinion(0, 0, 0);
    alienscore.scale.set(0.3,0.3,0.3);
    alienscore.baseMat();
    alienscore.rotation.x = Math.PI;
    sceneAlien.add(alienscore);

    sceneMessage = new THREE.Scene() // Cena onde esta as mensagens
    gScreen = createScreen(0, 0, 0);
    gScreen.visible = false;
    gScreen.rotation.y = Math.PI;
    gScreen.rotation.z = Math.PI;
    sceneMessage.add(gScreen);

    if (1 < (window.innerWidth/window.innerHeight)) {
        var delta = ((SCORE_SQ_SIZE) * ((0.1*window.innerWidth)/(0.1*window.innerHeight)) - (SCORE_SQ_SIZE)) / 2;
        cameraShips = new THREE.OrthographicCamera(SCORE_SQ_SIZE/-2 - delta, SCORE_SQ_SIZE/2 + delta, SCORE_SQ_SIZE/-2, SCORE_SQ_SIZE/2, -100, 100);
        cameraAliens = new THREE.OrthographicCamera(SCORE_SQ_SIZE/-2 - delta, SCORE_SQ_SIZE/2 + delta, SCORE_SQ_SIZE/-2, SCORE_SQ_SIZE/2, -100, 100);
    } else {
        var delta = ((SCORE_SQ_SIZE) / ((0.1*window.innerWidth)/(0.1*window.innerHeight)) - (SCORE_SQ_SIZE)) / 2;
        cameraShips = new THREE.OrthographicCamera(SCORE_SQ_SIZE/-2, SCORE_SQ_SIZE/2, SCORE_SQ_SIZE/-2 - delta, SCORE_SQ_SIZE/2 + delta, -100, 100);
        cameraAliens = new THREE.OrthographicCamera(SCORE_SQ_SIZE/-2, SCORE_SQ_SIZE/2, SCORE_SQ_SIZE/-2 - delta, SCORE_SQ_SIZE/2 + delta, -100, 100);
    }

    if ((MSG_X/MSG_Y) < (window.innerWidth/(window.innerHeight/2))) {
        delta = ((MSG_Y) * ((window.innerWidth/2)/(window.innerHeight/4)) - (MSG_X)) / 2;
        cameraMessage = new THREE.OrthographicCamera(MSG_X/-2 - delta, MSG_X/2 + delta, MSG_Y/-2, MSG_Y/2, -600, 600);
    } else {
        delta = ((MSG_X) / ((window.innerWidth/2)/(window.innerHeight/4)) - (MSG_Y)) / 2;
        cameraMessage = new THREE.OrthographicCamera(MSG_X/-2, MSG_X/2, MSG_Y/-2 - delta, MSG_Y/2 + delta, -600, 600);
    }
    cameraShips.position.x = 0;
    cameraShips.position.y = SCORE_SQ_SIZE/2;
    cameraShips.position.z = 0;
    cameraShips.lookAt(new THREE.Vector3(0,0,0));
    cameraShips.updateProjectionMatrix();

    cameraAliens.position.x = 0;
    cameraAliens.position.y = SCORE_SQ_SIZE/2;
    cameraAliens.position.z = 0;
    cameraAliens.lookAt(new THREE.Vector3(0,0,0));
    cameraAliens.updateProjectionMatrix();

    cameraMessage.position.x = 0;
    cameraMessage.position.y = SCORE_SQ_SIZE;
    cameraMessage.position.z = 0;
    cameraMessage.lookAt(new THREE.Vector3(0, 0, 0));
    cameraMessage.updateProjectionMatrix();
}
