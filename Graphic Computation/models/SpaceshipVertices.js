'use strict';

function addDeckV (obj, x, y, z) {

    geometry = new THREE.Geometry();

    geometry.vertices.push(new THREE.Vector3(4.5,4.5,-10));
    geometry.vertices.push(new THREE.Vector3(4.5,-4.5,-10));
    geometry.vertices.push(new THREE.Vector3(-4.5,-4.5,-10));
    geometry.vertices.push(new THREE.Vector3(-4.5,4.5,-10));
    geometry.vertices.push(new THREE.Vector3(0,0,10));


    geometry.faces.push(new THREE.Face3(2,3,0));
    geometry.faces.push(new THREE.Face3(0,1,2));
    geometry.faces.push(new THREE.Face3(4,1,0));
    geometry.faces.push(new THREE.Face3(4,2,1));
    geometry.faces.push(new THREE.Face3(4,3,2));
    geometry.faces.push(new THREE.Face3(4,0,3));


    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material);
  	mesh.position.set(x, y, z);

    for ( var i in mesh.geometry.faces) {
        mesh.geometry.faces[i].color.setHex(0xff0000);
    }

    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addMainBodyV(obj, x, y, z) {

    geometry = new THREE.Geometry();

    geometry.vertices.push(new THREE.Vector3(7.5,5,30));
    geometry.vertices.push(new THREE.Vector3(7.5,5,-30));
    geometry.vertices.push(new THREE.Vector3(7.5,-5,30));
    geometry.vertices.push(new THREE.Vector3(7.5,-5,-30));
    geometry.vertices.push(new THREE.Vector3(-7.5,5,-30));
    geometry.vertices.push(new THREE.Vector3(-7.5,5,30));
    geometry.vertices.push(new THREE.Vector3(-7.5,-5,-30));
    geometry.vertices.push(new THREE.Vector3(-7.5,-5,30));

    geometry.faces.push(new THREE.Face3(0,2,1));
    geometry.faces.push(new THREE.Face3(2,3,1));
    geometry.faces.push(new THREE.Face3(4,6,5));
    geometry.faces.push(new THREE.Face3(6,7,5));
    geometry.faces.push(new THREE.Face3(4,5,1));
    geometry.faces.push(new THREE.Face3(5,0,1));
    geometry.faces.push(new THREE.Face3(7,6,2));
    geometry.faces.push(new THREE.Face3(6,3,2));
    geometry.faces.push(new THREE.Face3(5,7,0));
    geometry.faces.push(new THREE.Face3(7,2,0));
    geometry.faces.push(new THREE.Face3(1,3,4));
    geometry.faces.push(new THREE.Face3(3,6,4));

    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    for ( var i in mesh.geometry.faces) {
        mesh.geometry.faces[i].color.setHex(0xb26801);
    }
    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addMainBodyUpper1V(obj, x, y, z) {

    geometry = new THREE.Geometry();

    geometry.vertices.push(new THREE.Vector3(5.5,1,25));
    geometry.vertices.push(new THREE.Vector3(5.5,1,-25));
    geometry.vertices.push(new THREE.Vector3(5.5,-1,35));
    geometry.vertices.push(new THREE.Vector3(5.5,-1,-25));
    geometry.vertices.push(new THREE.Vector3(-5.5,1,-25));
    geometry.vertices.push(new THREE.Vector3(-5.5,1,25));
    geometry.vertices.push(new THREE.Vector3(-5.5,-1,-25));
    geometry.vertices.push(new THREE.Vector3(-5.5,-1,35));

    geometry.faces.push(new THREE.Face3(0,2,1));
    geometry.faces.push(new THREE.Face3(2,3,1));
    geometry.faces.push(new THREE.Face3(4,6,5));
    geometry.faces.push(new THREE.Face3(6,7,5));
    geometry.faces.push(new THREE.Face3(4,5,1));
    geometry.faces.push(new THREE.Face3(5,0,1));
    geometry.faces.push(new THREE.Face3(7,6,2));
    geometry.faces.push(new THREE.Face3(6,3,2));
    geometry.faces.push(new THREE.Face3(5,7,0));
    geometry.faces.push(new THREE.Face3(7,2,0));
    geometry.faces.push(new THREE.Face3(1,3,4));
    geometry.faces.push(new THREE.Face3(3,6,4));

    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    for ( var i in mesh.geometry.faces) {
        mesh.geometry.faces[i].color.setHex(0xff0000);
    }
    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addMainBodyUpper2V(obj, x, y, z) {

    geometry = new THREE.Geometry();

    geometry.vertices.push(new THREE.Vector3(4.5,2,10));
    geometry.vertices.push(new THREE.Vector3(4.5,2,-10));
    geometry.vertices.push(new THREE.Vector3(4.5,-2,20));
    geometry.vertices.push(new THREE.Vector3(4.5,-2,-10));
    geometry.vertices.push(new THREE.Vector3(-4.5,2,-10));
    geometry.vertices.push(new THREE.Vector3(-4.5,2,10));
    geometry.vertices.push(new THREE.Vector3(-4.5,-2,-10));
    geometry.vertices.push(new THREE.Vector3(-4.5,-2,20));

    geometry.faces.push(new THREE.Face3(0,2,1));
    geometry.faces.push(new THREE.Face3(2,3,1));
    geometry.faces.push(new THREE.Face3(4,6,5));
    geometry.faces.push(new THREE.Face3(6,7,5));
    geometry.faces.push(new THREE.Face3(4,5,1));
    geometry.faces.push(new THREE.Face3(5,0,1));
    geometry.faces.push(new THREE.Face3(7,6,2));
    geometry.faces.push(new THREE.Face3(6,3,2));
    geometry.faces.push(new THREE.Face3(5,7,0));
    geometry.faces.push(new THREE.Face3(7,2,0));
    geometry.faces.push(new THREE.Face3(1,3,4));
    geometry.faces.push(new THREE.Face3(3,6,4));

    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    for ( var i in mesh.geometry.faces) {
        mesh.geometry.faces[i].color.setHex(0x000000);
    }
    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addMainBodyCilV(obj, x, y, z) {

    geometry = new THREE.Geometry();

    for(var i = 0; i < 5; i++) {
        geometry.vertices.push(new THREE.Vector3(Math.sin(((2*i) * Math.PI)/5), 2, Math.cos(((2*i) * Math.PI)/5)));
        geometry.vertices.push(new THREE.Vector3(Math.sin(((2*i) * Math.PI)/5), -2, Math.cos(((2*i) * Math.PI)/5)));
    }

    geometry.vertices.push(new THREE.Vector3(0,2.5,0));

    geometry.faces.push(new THREE.Face3(0,1,3));
    geometry.faces.push(new THREE.Face3(0,3,2));
    geometry.faces.push(new THREE.Face3(2,3,5));
    geometry.faces.push(new THREE.Face3(2,5,4));
    geometry.faces.push(new THREE.Face3(4,5,7));
    geometry.faces.push(new THREE.Face3(4,7,6));
    geometry.faces.push(new THREE.Face3(6,7,9));
    geometry.faces.push(new THREE.Face3(6,9,8));
    geometry.faces.push(new THREE.Face3(8,9,1));
    geometry.faces.push(new THREE.Face3(8,1,0));
    geometry.faces.push(new THREE.Face3(10,0,2));
    geometry.faces.push(new THREE.Face3(10,2,4));
    geometry.faces.push(new THREE.Face3(10,4,6));
    geometry.faces.push(new THREE.Face3(10,6,8));
    geometry.faces.push(new THREE.Face3(10,8,0));
    geometry.faces.push(new THREE.Face3(1,5,3));
    geometry.faces.push(new THREE.Face3(1,7,5));
    geometry.faces.push(new THREE.Face3(1,9,7));


    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    for ( var i in mesh.geometry.faces) {
        mesh.geometry.faces[i].color.setHex(0xff0000);
    }
    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addSideBodyV(obj, x, y, z) {

    geometry = new THREE.Geometry();

    geometry.vertices.push(new THREE.Vector3(2.5,3,20));
    geometry.vertices.push(new THREE.Vector3(2.5,3,-20));
    geometry.vertices.push(new THREE.Vector3(2.5,-3,20));
    geometry.vertices.push(new THREE.Vector3(2.5,-3,-20));
    geometry.vertices.push(new THREE.Vector3(-2.5,3,-20));
    geometry.vertices.push(new THREE.Vector3(-2.5,3,20));
    geometry.vertices.push(new THREE.Vector3(-2.5,-3,-20));
    geometry.vertices.push(new THREE.Vector3(-2.5,-3,20));

    geometry.faces.push(new THREE.Face3(0,2,1));
    geometry.faces.push(new THREE.Face3(2,3,1));
    geometry.faces.push(new THREE.Face3(4,6,5));
    geometry.faces.push(new THREE.Face3(6,7,5));
    geometry.faces.push(new THREE.Face3(4,5,1));
    geometry.faces.push(new THREE.Face3(5,0,1));
    geometry.faces.push(new THREE.Face3(7,6,2));
    geometry.faces.push(new THREE.Face3(6,3,2));
    geometry.faces.push(new THREE.Face3(5,7,0));
    geometry.faces.push(new THREE.Face3(7,2,0));
    geometry.faces.push(new THREE.Face3(1,3,4));
    geometry.faces.push(new THREE.Face3(3,6,4));

    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    for ( var i in mesh.geometry.faces) {
        mesh.geometry.faces[i].color.setHex(0xff0000);
    }

    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addWingV(obj, x, y, z, rot_x, rot_z) {

    geometry = new THREE.Geometry();

    geometry.vertices.push(new THREE.Vector3(20,0.5,7.5));
    geometry.vertices.push(new THREE.Vector3(20,0.5,-7.5));
    geometry.vertices.push(new THREE.Vector3(21,-0.5,7.5));
    geometry.vertices.push(new THREE.Vector3(21,-0.5,-7.5));
    geometry.vertices.push(new THREE.Vector3(-20,0.5,-7.5));
    geometry.vertices.push(new THREE.Vector3(-20,0.5,7.5));
    geometry.vertices.push(new THREE.Vector3(-20,-0.5,-7.5));
    geometry.vertices.push(new THREE.Vector3(-20,-0.5,7.5));
    geometry.vertices.push(new THREE.Vector3(20,10,7.5));
    geometry.vertices.push(new THREE.Vector3(20,10,-7.5));

    geometry.faces.push(new THREE.Face3(0,8,1));
    geometry.faces.push(new THREE.Face3(9,1,8));
    geometry.faces.push(new THREE.Face3(2,3,9));
    geometry.faces.push(new THREE.Face3(2,9,8));
    geometry.faces.push(new THREE.Face3(0,2,8));
    geometry.faces.push(new THREE.Face3(1,9,3));
    geometry.faces.push(new THREE.Face3(4,6,5));
    geometry.faces.push(new THREE.Face3(6,7,5));
    geometry.faces.push(new THREE.Face3(4,5,1));
    geometry.faces.push(new THREE.Face3(5,0,1));
    geometry.faces.push(new THREE.Face3(7,6,2));
    geometry.faces.push(new THREE.Face3(6,3,2));
    geometry.faces.push(new THREE.Face3(5,7,0));
    geometry.faces.push(new THREE.Face3(7,2,0));
    geometry.faces.push(new THREE.Face3(1,3,4));
    geometry.faces.push(new THREE.Face3(3,6,4));

    geometry.computeFaceNormals();

    mesh = new THREE.Mesh(geometry, material.clone());
    mesh.position.set(x, y, z);
    mesh.rotation.x = rot_x;
    mesh.rotation.z = rot_z;

    for (var i = 0; i < 6; i++) {
        mesh.geometry.faces[i].color.setHex(0xff0000);
    }

    for (i = 6; i < 16; i++) {
        mesh.geometry.faces[i].color.setHex(0xb26801);
    }

    geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function spawnSpotLight(spaceship) {
    vector.addVectors(spaceship.position, spaceship.children[0].children[1].position);
    vector.z += 8;
    vector.y += 3;

    spotLight = new THREE.SpotLight(SPOTLIGHT_COLOR,
                                    SPOTLIGHT_INTENSITY,
                                    SPOTLIGHT_DISTANCE,
                                    SPOTLIGHT_ANGLE,
                                    SPOTLIGHT_PENUMBRA,
                                    SPOTLIGHT_DECAY);

    spotLight.position.copy(vector);
    spotLight.target.position.set( spaceship.position.x, spaceship.position.y, spaceship.position.z - 300 );
    scene.add( spotLight );
    scene.add( spotLight.target);
    spotLight.target.updateMatrixWorld();
    if (SHOW_SPOTLIGHT) {
        spotter = new THREE.SpotLightHelper( spotLight);
        scene.add( spotter);
    }

    spotLight.movement = function (time, pos1, pos2) {
        vector.subVectors(pos1, pos2);
        spotLight.translateX(vector.x);
        spotLight.translateZ(vector.z);
        spotLight.updateMatrixWorld();
        spotLight.target.translateX(vector.x);
        spotLight.target.translateZ(vector.z);
        spotLight.target.updateMatrixWorld();

    }
}

function createSpaceshipV(x, y, z) {

    material = new THREE.MeshPhongMaterial({
            specular: SPACESHIP_MAT_SPECULAR,
            shininess: SPACESHIP_MAT_SHININESS,
            shading: THREE.SmoothShading,
            vertexColors: THREE.FaceColors
        });
    var phongMat = material;
        phongMat.needsUpdate = true;
    var lambMat = new THREE.MeshLambertMaterial({
            shading: THREE.SmoothShading,
            vertexColors: THREE.FaceColors
    });
    var baseMat = new THREE.MeshBasicMaterial({
            vertexColors: THREE.FaceColors
    });

    var current = phongMat;
    var base = false;


/****************************************************/
/************** Create Hierarchy ********************/
    var MainBody = new THREE.Object3D();
        addMainBodyV(MainBody, 0, 0, 0);
        addMainBodyUpper1V(MainBody, 0, 6, -5);
        addMainBodyUpper2V(MainBody, 0, 9, -17.5);
        addDeckV(MainBody, 0, 0, 40);

	//***** Left side *****
    var SideLeft = new THREE.Object3D();
    	addSideBodyV(SideLeft, 10, 0, -5);

        // Cils
        var SideLeftCils = new THREE.Object3D();
            addMainBodyCilV(SideLeftCils, 10, 5, -22);
            addMainBodyCilV(SideLeftCils, 10, 5, -16);
            addMainBodyCilV(SideLeftCils, 10, 5, -10);

        // Wings
        var SideLeftWings = new THREE.Object3D();
            addWingV(SideLeftWings, 30, 8, -10, 0, Math.PI/8);
            addWingV(SideLeftWings, 30, -8, -10, Math.PI, Math.PI/8);


    SideLeft.add(SideLeftCils);
    SideLeft.add(SideLeftWings);


	// ***** Right side *****
    var SideRight = new THREE.Object3D();
        addSideBodyV(SideRight, -10, 0, -5);

        // Cils
        var SideRightCils = new THREE.Object3D();
            addMainBodyCilV(SideRightCils, -10, 5, -22);
            addMainBodyCilV(SideRightCils, -10, 5, -16);
            addMainBodyCilV(SideRightCils, -10, 5, -10);

        var SideRightWings = new THREE.Object3D();
            addWingV(SideRightWings, -30, 8, -10, Math.PI, (-7*Math.PI)/8);
            addWingV(SideRightWings, -30, -8, -10, 0, (-7*Math.PI)/8);

    SideRight.add(SideRightCils);
    SideRight.add(SideRightWings);

/****************************************************/
/****************************************************/

    // *** Spaceship main node ***
    var spaceship = new THREE.Object3D();
    spaceship.add(MainBody);
    spaceship.add(SideLeft);
    spaceship.add(SideRight);

    var bbhelper = new THREE.BoundingBoxHelper(spaceship, material);
    var model = new THREE.Box3().setFromObject(spaceship);
    var radius = Math.sqrt( Math.pow( (model.getSize().x ) / 2, 2) +
                            Math.pow( (model.getSize().z ) / 2, 2));

    spaceship.userData = {
        oldposition: new THREE.Vector3,
        direction: 0,
        speedX: 0,
        rotation: 0,

        radius: radius,
        BB: model,
        bbhelper: bbhelper,

        score: 0,
        health: PLAYER_HEALTH,
        damage: PLAYER_DAMAGE,
        type: "player",
        colision_block: true,
        had_collision: false
    };

spaceship.changeMats = function() {
  if (base == true)
    return;

    (current == phongMat) ? current = lambMat : current = phongMat;

  spaceship.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = current;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

spaceship.baseMat = function() {
    (base == false) ? mesh = baseMat : mesh = current;
    base = !base;

  spaceship.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = mesh;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

spaceship.movement = function (time) {
    spaceship.userData.oldposition = spaceship.position.clone();
    spaceship.position.x += 150000*time*spaceship.userData.speedX;
    spaceship.rotation.z += 150000*time*spaceship.userData.rotation;
    spotLight.movement(time, spaceship.position, spotLight.position);
};

spaceship.collision = function(obj) {

        if (obj.userData.type != "ally")
            spaceship.userData.health -= obj.userData.damage;
        if (obj.userData.type == "wall") {
            if(spaceship.position.x > 0)
                spaceship.position.x = FIELDWIDTH/2 - 65;
            else
                spaceship.position.x = -FIELDWIDTH/2 + 65;
        }

    spotLight.movement(time, spaceship.position, spotLight.position);

        if (spaceship.userData.health <= 0) {
          scene.remove(spotLight);
        }
};

spaceship.fire = function () {

    if (spaceship.userData.health <= 0)
        return;

    for (var i = 0; i < list_bullets.length; i++)
        if (list_bullets[i].userData.available) {
            vector.addVectors(spaceship.position, spaceship.children[1].children[0].position);
            list_bullets[i].reset(vector);

            scene.add(list_bullets[i]);
            list_objects.push(list_bullets[i]);
            break;
        }

    for (i; i < list_bullets.length; i++)
        if (list_bullets[i].userData.available) {
            vector.addVectors(spaceship.position, spaceship.children[2].children[0].position);
            list_bullets[i].reset(vector);

            scene.add(list_bullets[i]);
            list_objects.push(list_bullets[i]);
            break;
        }

    for (i = 0; i < list_missiles.length; i++)
        if (list_missiles[i].userData.available) {
            vector.addVectors(spaceship.position, spaceship.children[1].children[0].position);
            vector.setX(vector.x+30);
            list_missiles[i].reset(vector);

            scene.add(list_missiles[i]);
            list_objects.push(list_missiles[i]);
            break;
        }

    for (i ; i < list_missiles.length; i++)
        if (list_missiles[i].userData.available) {
            vector.addVectors(spaceship.position, spaceship.children[2].children[0].position);
            vector.setX(vector.x-30);
            list_missiles[i].reset(vector);

            scene.add(list_missiles[i]);
            list_objects.push(list_missiles[i]);
            break;
        }
};

	spaceship.position.set(x, y, z);

    if (list_aliens.length == 0)
        spawnSpotLight(spaceship);
    return spaceship;
}
