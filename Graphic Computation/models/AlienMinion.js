'use strict';

function addMidSide(obj, x, y, z) {

    geometry = new THREE.BoxGeometry(6, 10, 18);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addSmallSide(obj, x, y, z) {

    geometry = new THREE.BoxGeometry(6, 20, 12);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addBigSide(obj, x, y, z) {

    geometry = new THREE.BoxGeometry(6, 30, 30);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addSmallCube(obj, x, y, z) {

    geometry = new THREE.BoxGeometry(6, 20, 6);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addBigBody(obj, x, y, z) {

    geometry = new THREE.BoxGeometry(30, 40, 12);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addSmallBody(obj, x, y, z) {

    geometry = new THREE.BoxGeometry(18, 40, 12);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addUnderSide(obj, x, y, z) {;

    geometry = new THREE.BoxGeometry(12, 20, 6);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function createAlienMinion(x, y, z) {

    material = new THREE.MeshPhongMaterial({
            color: ALIEN_MAT_COLOR,
            specular: ALIEN_MAT_SPECULAR,
            shininess: ALIEN_MAT_SHININESS,
            shading: THREE.SmoothShading
        });
    var phongMat = material;
        phongMat.needsUpdate = true;
    var lambMat = new THREE.MeshLambertMaterial({
            color: ALIEN_MAT_COLOR,
            shading: THREE.SmoothShading
    });
    var baseMat = new THREE.MeshBasicMaterial({
            color: ALIEN_MAT_COLOR
    });

    var current = phongMat;
    var base = false;

/****************************************************/
/************** Create Hierarchy ********************/
    var bigBody = new THREE.Object3D();
        addBigBody(bigBody, 0, 0, 0);

        var leftSide = new THREE.Object3D();
            addBigSide(leftSide, -18, 0, -3);

            var leftMouth = new THREE.Object3D();
                addUnderSide(leftMouth, -9, 0, 15);

            var leftWing = new THREE.Object3D();
                addSmallSide(leftWing, -24, 0, -6);
                addMidSide(leftWing, -30, 0, 3);

            leftSide.add(leftMouth);
            leftSide.add(leftWing);

        bigBody.add(leftSide);

        var rightSide = new THREE.Object3D();
            addBigSide(rightSide, 18, 0, -3);

            var rightMouth = new THREE.Object3D();
                addUnderSide(rightMouth, 9, 0, 15);

            var rightWing = new THREE.Object3D();
                addSmallSide(rightWing, 24, 0, -6);
                addMidSide(rightWing, 30, 0, 3);

            rightSide.add(rightMouth);
            rightSide.add(rightWing);

        bigBody.add(rightSide);

    var smallBody = new THREE.Object3D();
        addSmallBody(smallBody, 0, 0, -12);

        var leftAntenna = new THREE.Object3D();
            addSmallSide(leftAntenna, -12, 0, -18);
            addSmallCube(leftAntenna, -18, 0, -27);

        var rightAntenna = new THREE.Object3D();
            addSmallSide(rightAntenna, 12, 0, -18);
            addSmallCube(rightAntenna, 18, 0, -27);

        smallBody.add(leftAntenna);
        smallBody.add(rightAntenna);

/****************************************************/
/****************************************************/

	var alienMinion = new THREE.Object3D();

    alienMinion.add(bigBody);
    alienMinion.add(smallBody);


    var bbhelper = new THREE.BoundingBoxHelper(alienMinion, phongMat);
    var model = new THREE.Box3().setFromObject(alienMinion);
    var radius = Math.sqrt( Math.pow( (model.getSize().x ) / 2, 2) +
                            Math.pow( (model.getSize().z ) / 2, 2));

    alienMinion.userData = {
        oldposition: new THREE.Vector3,
        speedX: 0,
        speedZ: 0,

        BB: model,
        radius: radius,
        bbhelper: bbhelper,

        health: ALIEN_HEALTH,
        damage: ALIEN_DAMAGE,
        type: "alien",
        colision_block: true,
        had_collision: false
    };

alienMinion.changeMats = function() {
    if (base == true)
        return;

    (current == phongMat) ? current = lambMat : current = phongMat;

  alienMinion.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = current;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

alienMinion.baseMat = function() {
    (base == false) ? mesh = baseMat : mesh = current;
    base = !base;

  alienMinion.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = mesh;
        node.material.wireframe = !wireframes;
      }
    }
  );
};


alienMinion.movement = function (time) {
    alienMinion.userData.oldposition = alienMinion.position.clone();
    alienMinion.userData.had_collision = false;
    alienMinion.translateX(ALIEN_SPEED*15000*time*alienMinion.userData.speedX);
    alienMinion.translateZ(ALIEN_SPEED*15000*time*alienMinion.userData.speedZ);
};

alienMinion.collision = function(obj) {

    alienMinion.position.setX(alienMinion.userData.oldposition.x);
    alienMinion.position.setY(alienMinion.userData.oldposition.y);
    alienMinion.position.setZ(alienMinion.userData.oldposition.z);

    if (obj.userData.type != "alien")
        alienMinion.userData.health -= obj.userData.damage;

    if (alienMinion.userData.had_collision)
        return;

    alienMinion.userData.had_collision = true;

    if (obj.userData.type == "wall") {
        if(obj.position.x != 0)
            alienMinion.userData.speedX *= -1;
        else
            alienMinion.userData.speedZ *= -1;
    } else {
        alienMinion.userData.speedX *= -1;
        alienMinion.userData.speedZ *= -1;
    }

};

alienMinion.generateMovement = function () {
    alienMinion.userData.speedX = Math.random()*2 - 1;
    alienMinion.userData.speedZ = Math.random()*2 - 1;
};

	alienMinion.position.set(x, y, z);

    return alienMinion;
}
