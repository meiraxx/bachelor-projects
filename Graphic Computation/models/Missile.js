'use strict';

function addMissileShell(obj, x, y, z) {

    mesh = new THREE.Mesh(new THREE.CylinderGeometry( 2, 2, 8, 32 ), material);
    mesh.position.set(x, y, z);
    mesh.rotation.x = Math.PI/2;

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addMissileHead(obj, x, y, z) {

    mesh = new THREE.Mesh(new THREE.CylinderGeometry( 2, 0.2, 6, 32 ), material);
    mesh.position.set(x, y, z);
    mesh.rotation.x = Math.PI/2;

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addMissileBack(obj, x, y, z) {

    mesh = new THREE.Mesh(new THREE.BoxGeometry( 4, 1, 3), material);
    mesh.position.set(x, y, z);

    obj.add(mesh);
}


function createMissile(x, y, z) {

    material = new THREE.MeshPhongMaterial({
            color: MISSILE_MAT_COLOR,
            specular: MISSILE_MAT_SPECULAR,
            shininess: MISSILE_MAT_SHININESS,
            shading: THREE.SmoothShading
        });
    var phongMat = material;
        phongMat.needsUpdate = true;
    var lambMat = new THREE.MeshLambertMaterial({
            color: MISSILE_MAT_COLOR,
            shading: THREE.SmoothShading
    });
    var baseMat = new THREE.MeshBasicMaterial({
            color: MISSILE_MAT_COLOR
    });

    var current = phongMat;
    var base = false;


    var missileShell = new THREE.Object3D();
        addMissileShell(missileShell, 0, 0, 0);
        addMissileHead(missileShell, 0, 0, -7);
        addMissileBack(missileShell, 0, 0, 4.5);
        var missileBack = new THREE.Object3D();
        addMissileBack(missileBack, 0, 0, 4.5);
        missileBack.rotation.z = Math.PI/2;
        missileShell.add(missileBack);

    var missile = new THREE.Object3D();
    missile.add(missileShell);

    var bbhelper = new THREE.BoundingBoxHelper(missile, material);
    var model = new THREE.Box3().setFromObject(missile);
    var radius = Math.sqrt( Math.pow( (model.getSize().x ) / 2, 2) +
                            Math.pow( (model.getSize().z ) / 2, 2));

    missile.userData = {
        oldposition: new THREE.Vector3,
        speedZ: MISSILE_SPEED,
        available: true,

        radius: radius,
        BB: model,
        bbhelper: bbhelper,

        health: MISSILE_HEALTH,
        damage: MISSILE_DAMAGE,
        type: "ally",
        colision_block: false,
        had_collision: false

    };

missile.changeMats = function() {
  if (base == true)
    return;

    (current == phongMat) ? current = lambMat : current = phongMat;

  missile.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = current;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

missile.baseMat = function() {
    (base == false) ? mesh = baseMat : mesh = current;
    base = !base;

  missile.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = mesh;
        node.material.wireframe = !wireframes;
      }
    }
  );
}

missile.movement = function (time) {
    missile.userData.oldposition = missile.position.clone();
    missile.rotateZ(15000*time);
    missile.translateZ(-10000*time*missile.userData.speedZ);
    missile.userData.speedZ += missile.userData.speedZ/20;
};

missile.collision = function(obj) {

    if (obj.userData.type != "player" && obj.userData.type != "ally") {
        missile.userData.health--;
        obj.userData.health -= missile.userData.damage;
    }

    if (obj.userData.type == "wall")
        missile.userData.health = 0;

    if (missile.userData.health <= 0)
        missile.userData.available = true;

};

missile.reset = function (vector) {
    missile.position.set(vector.x, vector.y, vector.z);
    missile.userData.health = 1;
    missile.userData.available = false;
    missile.userData.speedZ = MISSILE_SPEED;
};

    missile.position.set(x, y, z);

    return missile;
}
